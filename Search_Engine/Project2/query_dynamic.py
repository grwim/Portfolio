import glob
import numpy as np
from py2casefold import casefold
from bs4 import BeautifulSoup, Comment
import codecs
from nltk.corpus import stopwords
import re
import math
from nltk.corpus import stopwords
import pdb
from nltk.stem import *
import sys
import os
import time


"""
For each query, if the phrase terms are common (high document frequency) send the query to the phrase index,
otherwsie send it to the proximity index
(Note: make sure you are DEMONSTRATING that both Phrase index and Proximity index are utilized
for query processing during this project and they are functional). If not enough documents found then use single term index or stem index (your choice).
Make sure you configuration uses at least two index if not all the four for each query.
 Provide your analysis. You can set some threshold for the number of retrieved documents.
"""


# yeah, so have to make different query procesing functions for different types of indexes
    # --> stem, phrase, positional? or do i?

# positional --> if two terms are adjacent in the same document, then...
# perhaps only do similiarty calc on 'matching' documents --> those that have query terms that are adjecent in the same document

# NEED TO: update obtainQuries for stem in query.py

# ----------------------------------------------------------------------------------

def tokenize(text):
    tokens = text.split()
    return tokens

def removeStopWords(tokenList):
    stop = set(stopwords.words('english'))
    tokenList = [i for i in tokenList if i not in stop]
    return tokenList

def textNorm(text):

        tempList = tuple()
        tempList = re.subn(r'blank;', ' ', str(text))
        text = tempList[0]

        tempList = tuple()
        tempList = re.subn(r'&sect;', '', str(text))
        text = tempList[0]

            # &amp
        tempList = tuple()
        tempList = re.subn(r'&amp;', '', str(text))
        text= tempList[0]

        tempList = tuple()
        tempList = re.subn(r'\xc2', '', str(text))
        text = tempList[0]

        tempList = tuple()
        tempList = re.subn(r'hyph', '-', str(text))
        text = tempList[0]

        tempList = tuple()
        tempList = re.subn(r'\(\w*\)', '', str(text))  # (b)(7)(i)
        text = tempList[0]

        tempList = tuple()
        tempList = re.subn(r'\(', '', str(text))  # (
        text = tempList[0]

        tempList = tuple()
        tempList = re.subn(r'\)', '', str(text))  # )
        text = tempList[0]

        tempList = tuple()
        tempList = re.subn(r'_', '', str(text))  # _
        text = tempList[0]

        tempList = tuple()
        tempList = re.subn(r',', '', str(text))  # ,
        text = tempList[0]

        tempList = tuple()
        tempList = re.subn(r';', '', str(text))  # ;
        text = tempList[0]

        tempList = tuple()
        tempList = re.subn(r'\\x[\w\.]+', '', str(text))  #   \xc2, xa7685.201 etc.
        text = tempList[0]

        tempList = tuple()
        tempList = re.subn(r'Ph.D.|Ph.D|Phd|PhD|PHD', 'phd', str(text))  #   phd
        text = tempList[0]

        # 'part, ' 'F'
        tempList = tuple()
        tempList = re.subn(r'\s[A-Z]\s', '', str(text))  # Letters that demark sections of text
        text = tempList[0]

        # 'Standard', '(c)', 'states', ':', '[A', 'pipeline', ']',
        tempList = tuple()
        tempList = re.subn(r'\([a-zA-Z1-9]\)', '', str(text))  # (c)
        text = tempList[0]

        tempList = tuple()
        tempList = re.subn(r'\[[A-Za-z1-9]', '', str(text))  # '[A',
        text = tempList[0]

        tempList = tuple()
        tempList = re.subn(r'[A-Za-z1-9]\]', '', str(text))  #  ']',
        text = tempList[0]

        tempList = tuple()
        tempList = re.subn(r'=', '', str(text))  #  '=',
        text = tempList[0]

        tempList = tuple()
        tempList = re.subn(r'``', '', str(text))  #  '``',
        text = tempList[0]

        tempList = tuple()
        tempList = re.subn(r'`', '', str(text))  #  '`',
        text = tempList[0]

        tempList = tuple()
        tempList = re.subn(r'`', '', str(text))  #  '`',
        text = tempList[0]

        tempList = tuple()
        tempList = re.subn(r'\/', '', str(text))  #  '/',
        text = tempList[0]

        tempList = tuple()
        tempList = re.subn(r';', '', str(text))  #  ';',
        text = tempList[0]

        tempList = tuple()
        tempList = re.subn(r'\:', '', str(text))  #  ':',
        text = tempList[0]

        # text = re.sub(r'(\.)(?!\d\w)', '', str(text))  # remove end-of sentance periods ---> any period that's not followed by a digit or letter

    # HYPHENATED TERMS
        # common prefix list: 'pre', 'post', 're', 'anti', 'de', 'dis', 'en', 'fore', 'in', 'semi', 'sub', 'super', 'trans', 'under',  'black', 'white', 'blue', 'red', 'green', 'brown'
        tempList = tuple()
        text = re.sub(r"(pre|re|un|semi|post|anti|de|dis|en|fore|in|sub|super|trans|under|black|white|blue|red|green|brown)-([a-zA-z]+)", "\g<1>\g<2> \g<2>", text)

        text = re.sub(r"\b(?:([a-zA-z]+)-([a-zA-z]+)-([a-zA-z]+))", "\g<1> \g<2> \g<3> \g<1>\g<2>\g<3>", text)
        # keep terms w/ common prefixes (pre, post, re, etc.) as both 'preprocessing' AND 'processing'
        # else, if 'black-tie' --> 'black', 'tie', 'blacktie'
        # else, up to three hyphens --> part-of-speach --> 'part', 'speach', 'partofspeach'
        text = re.sub(r"\b(?:([a-zA-z]+)-([a-zA-z]+))", "\g<1> \g<2> \g<1>\g<2>", text)

    # APLHA-DIGIT  --> U.S., U.S.A., U.S.C., F-16, I-20,

        # if three or more letters, store as seperate terms
        text = re.sub(r'\b(?:([a-zA-z]{3,})-([\d]+))', '\g<1> \g<1>\g<2>', text) # CDC-50

    # DIGIT-ALHPA
        text = re.sub(r'\b(?:([\d]+)-([a-zA-z]{3,}))', '\g<1>\g<2> \g<2>', text) # 50-CDC

        # otherwise, merely remove the hyphen
        text = re.sub(r'-', '', text)

        # digit formats --> 100.00 | 1,000,000 | 1000 -->

        # 100.00 --> 100
        text = re.sub(r'([\d]+)\.[0]+', '\g<1>', text )

        # 1,000, etc.
        text = re.sub(r',', '', text)

        text = re.sub(r'\.', '', str(text)) # remove periods (takes care of acronyms and end of sentance periods, file extenions etc.)

        text = re.sub(r'[^\x00-\x7F]+',' ', text) # remove non-ascii

        # MM/DD/yyyy
        text = re.sub(r'\b(?:([\d]{2})\/([\d]{2})\/([\d]{4}))', '\g<1>-\g<2>-\g<3>', text)
        # MM-DD-YYYY
        # Month Name, DD YYYY
        # MMM-DD-YYYY

        # CASE-FOLDING
        text = text.lower()

        return text

def obtainQueries_stem(filename): # returns a list of normalized querys (tokenized) from the query file
    stemmer = PorterStemmer()

    file_stream = codecs.open(filename, 'r', 'ascii')
    soup = BeautifulSoup(file_stream, 'html.parser')

    query_list = []

    title_expression  = re.compile('^<title> Topic: ([\w -]+)' )
    for text in soup.find_all('title'):

        result = re.findall(title_expression, str(text)) #extract title fields
        result = result[0].strip()  # remove trailing and leading spaces

        # normalization here
        result = textNorm(result) # normalize text
        tokens = tokenize(result)

        tokens = [stemmer.stem(token).encode('ascii', 'ignore') for token in tokens]

        query_list.append(tokens)
        # print title
    return query_list

def obtainQueries_phrase(filename, phraseSize): # returns a list of normalized querys (tokenized) from the query file
    file_stream = codecs.open(filename, 'r', 'ascii')
    soup = BeautifulSoup(file_stream, 'html.parser')

    query_list = []

    title_expression  = re.compile('^<title> Topic: ([\w -]+)' )
    for text in soup.find_all('title'):

        result = re.findall(title_expression, str(text)) #extract title fields
        result = result[0].strip()  # remove trailing and leading spaces

        # normalization here
        result = textNorm(result) # normalize text
        tokens = tokenize(result)

        if phraseSize == 2:
            temp = []
            for i in range(len(tokens)):
                if i < len(tokens)-1:
                    phrase = tokens[i] + ' ' + tokens[i+1]
                    temp.append(phrase)
            tokens = temp

        elif phraseSize == 3:
            temp = []
            for i in range(len(tokens)):
                if i < len(tokens)-2:
                    phrase = tokens[i] + ' ' + tokens[i+1] + ' ' + tokens[i+2]
                    temp.append(phrase)
            tokens = temp


        query_list.append(tokens)
        # print title
    return query_list

# positional --> if two terms are adjacent in the same document, then...
# perhaps only do similiarty calc on 'matching' documents --> those that have query terms that are adjecent in the same document

# def vectorSpace_EVAL_POSITIONAL(lexicon_fName, postingList_fName, query_path, docMap, results_file):
#     runID = 'VECTORSPACE'

#     rem_stopWords = True

#     queryResults = []

#     # open query file, process queries, tokineze, create list
#     queryList = obtainQueries(query_path) # generic
#     if rem_stopWords:
#         for query in queryList:
#             query = removeStopWords(query)


#     # CREATE INVERTED INDEX
#     index = {}
#     numDoc = 0
#     with open(lexicon_fName) as f:
#         fLines  = f.readlines()
#         for i in range(len(fLines) - 1) :
#             term = [x.strip() for x in fLines[i].split(',')]   # need to convert from string to list
#             myList = []
#             myList.append(term[1])
#             index[term[0]] = myList
#         # numDoc = int(float(fLines[-1]))  # TOTAL NUMBER DOCUMENTS ?

#     # part two of creating inverted index --> obtain posting lists and append to lexicon to created inverted index
#     with open(postingList_fName) as f:
#         fLines  = f.readlines()
#         for line in fLines:
#             term = [x.strip() for x in line.split(',')]   # need to convert from string to list
#             postingList_term = []
#             postingList_term.append(term[1])
#             postingList_term.append(term[2])
#             index[term[0]].append(postingList_term)

#     for query in queryList:
#         top_100 = vectorSpace2(query, index, docMap)
#         line = []
#         line.append(query)
#         line.append(top_100)
#         queryResults.append(line)

#     createTrecFile(queryResults, query_path, runID, results_file)

# def vectorSpace_EVAL(lexicon_fName, postingList_fName, query_path, docMap, results_file):
#     runID = 'VECTORSPACE'

#     rem_stopWords = True

#     queryResults = []

#     # open query file, process queries, tokineze, create list
#     queryList = obtainQueries(query_path) # generic
#     if rem_stopWords:
#         for query in queryList:
#             query = removeStopWords(query)


#     # CREATE INVERTED INDEX
#     index = {}
#     numDoc = 0
#     with open(lexicon_fName) as f:
#         fLines  = f.readlines()
#         for i in range(len(fLines) - 1) :
#             term = [x.strip() for x in fLines[i].split(',')]   # need to convert from string to list
#             myList = []
#             myList.append(term[1])
#             index[term[0]] = myList
#         # numDoc = int(float(fLines[-1]))  # TOTAL NUMBER DOCUMENTS ?

#     # part two of creating inverted index --> obtain posting lists and append to lexicon to created inverted index
#     with open(postingList_fName) as f:
#         fLines  = f.readlines()
#         for line in fLines:
#             term = [x.strip() for x in line.split(',')]   # need to convert from string to list
#             postingList_term = []
#             postingList_term.append(term[1])
#             postingList_term.append(term[2])
#             index[term[0]].append(postingList_term)

#     for query in queryList:
#         top_100 = vectorSpace2(query, index, docMap)
#         line = []
#         line.append(query)
#         line.append(top_100)
#         queryResults.append(line)

#     createTrecFile(queryResults, query_path, runID, results_file)

def obtainDocMap(output_dir):
    docMap_name = output_dir + 'docMap'
    docMap = {}

    with open(docMap_name, 'r') as file:
        num_lines = sum(1 for line in file)
        file.seek(0,0)
        count = 0
        while count < num_lines:
            lineString = file.readline()
            line = [x.strip() for x in lineString.split(',')]   # need to convert from string to list
            # print line

            if line[0] not in docMap.keys():
                # print currTriple[1], ' added to document dictionary'
                docMap[line[0]] = line[1]
            else:
                print 'ERROR in obtainDocMap() --> duplicate doc ID encountered'

            count += 1

    return docMap

def get_top100(Scores):
    top_100 = []

    d_view = [ (v,k) for k,v in Scores.iteritems() ]
    d_view.sort(reverse=True) # natively sort tuples by first element

    count = 0
    for v, k in d_view:
        if count < 100:
            # print k, v
            tempList = []
            tempList.append(k)
            tempList.append(v)
            top_100.append(tempList)
            count += 1

    return top_100

def createTrecFile(queryResults, query_path, runID, results_file):

    # need to obtain query_id for each query
    file_stream = codecs.open(query_path, 'r', 'ascii')
    soup = BeautifulSoup(file_stream, 'html.parser')

    queryID_list = []

    title_expression  = re.compile('^<num> Number: ([\d]+)' )
    for text in soup.find_all('num'):

        queryID = re.findall(title_expression, str(text)) #extract title fields
        queryID = queryID[0].strip()  # remove trailing and leading spaces
        queryID_list.append(queryID)
        # print queryID

    # trec_fName = runID + '.trec'
    with open(results_file, 'w') as f:
        rank = 0

        queryCount = 0
        for query in queryResults: # for each query

            # for top 100 documents corresponding to a given query:

            rank = 0
            for results in query[1:]: # ignore first index, which is query tokens


                # for result in results:

                    # obtain variables:
                queryID = queryID_list[queryCount]
                iter_ = 0
                # pdb.set_trace()
                docno = results[0]
                sim = results[1]

                # query_id, iter, docno, rank, sim, run_id
                string = str(queryID) + ' ' + str(iter_) + ' ' + docno + ' ' + str(rank) + ' ' + str(sim) + ' ' + runID + '\n'

                f.write(string)

                # write line:


                rank += 1

            queryCount += 1

def probabalistic(query, index, docDict):

    k_1 = 1.2
    k_2 = 500
    b = 0.75

    numDoc = len(docDict)

    N = len(docDict) # total number of documents in a collection

    # print 'numDoc: ', numDoc, ', size docDict: ', len(docDict)

    SC_Doc_i = 0

    sum_docLengths = 0
    for k, v in docDict.iteritems():
        sum_docLengths += int(v)
    avgdl = sum_docLengths / float(len(docDict))

    Scores = {}

    # for docID, D_size in docDict.iteritems():
    for term in query:

        if term in index:
            n = int(index[term][0]) # number of documents that contain the query term
            w = math.log10((N - n +0.5) / (n + 0.5))

            qtf_j = 0 # obtain qtf_j
            for queryTerm in query:
                if (term == queryTerm):
                    qtf_j += 1

            postingList = []
            for x in range(len(index[term]) - 1):         # obtain posting list l
                # print index[term]
                postingList.append(index[term][x+1])

            # print 'postingList: ', term, ' ', postingList
            # obtain df
            df_term_i = int(float(index[term][0]))
            # print 'df_term_i: ', df_term_i

            # obtain idf
            idf_term_i = math.log10(numDoc / df_term_i)
            # print 'idf_term_i: ', idf_term_i

            tf_ij = 0
            for entry in postingList:

                doc_i_length = int(docDict[entry[0]])
                # if entry[0] == Doc_i_Name:
                tf_ij = int(float(entry[1]))
                # print 'tf_ij: ', tf_ij

                if entry[0] not in Scores:  # CHECK THIS
                    Scores[entry[0]] = 0

                # else:
                    # increase value by appropriate amount
                numer_big = (k_1 + 1) * tf_ij
                denom_big = tf_ij + k_1 * (1 - b + b * (doc_i_length / float(avgdl) ))

                # print 'numer_big: ', numer_big
                # print 'denom_big: ', denom_big

                Scores[entry[0]] += w * (numer_big / denom_big) * (((k_2 + 1) * qtf_j) / float( (k_2 + qtf_j) ) )

                # postingList = []
                # for x in range(len(index[term]) - 1):         # obtain posting list l
                #     # print index[term]
                #     postingList.append(index[term][x+1])

    # pdb.set_trace()

    top100 = get_top100(Scores)

    return top100

def docID(p):
    return p[0]

def positions(p):
    return p[2:]

# returns dict containing docIDs as keys for all docs that contain query terms that are adjacent
def positional_interesect(PL_1, PL_2, k):

    answer = {}

    PL_1_INDEX = 0
    PL_2_INDEX = 0

    p1 = PL_1[PL_1_INDEX]
    p2 = PL_2[PL_2_INDEX]

    pp1_index = 0
    pp2_index = 0

    for term1 in PL_1:
        for term2 in PL_2:

            if docID(term1) == docID(term2):
                # L = []
                pp1 = positions(term1)
                pp2 = positions(term2)

                for position1 in pp1:
                    for position2 in pp2:
                        if abs(int(position1) - int(position2)) <= k:
                            answer[docID(term1)] = True

    return answer

def probabalistic_EVAL(lexicon_fName, postingList_fName, docMap, results_file, query):
    rem_stopWords = True
    runID = 'PROBABALISTIC'

    queryResults = []

    # open query file, process queries, tokineze, create list
    # queryList = obtainQueries(query_path) # generic
    if rem_stopWords:
        # for query in queryList:
         query = removeStopWords(query)



    # pdb.set_trace()



    # CREATE INVERTED INDEX
    index = {}
    numDoc = 0
    with open(lexicon_fName) as f:
        fLines  = f.readlines()
        for i in range(len(fLines) - 1) :
            term = [x.strip() for x in fLines[i].split(',')]   # need to convert from string to list
            myList = []
            myList.append(term[1])
            index[term[0]] = myList
        # numDoc = int(float(fLines[-1]))  # TOTAL NUMBER DOCUMENTS ?

    # part two of creating inverted index --> obtain posting lists and append to lexicon to created inverted index
    with open(postingList_fName) as f:
        fLines  = f.readlines()
        for line in fLines:
            term = [x.strip() for x in line.split(',')]   # need to convert from string to list
            postingList_term = []
            postingList_term.append(term[1])
            postingList_term.append(term[2])
            index[term[0]].append(postingList_term)

    top_100 = probabalistic(query, index, docMap)
    # line = []
    #     line.append(query)
    #     line.append(top_100)
    #     queryResults.append(line)

    # createTrecFile(queryResults, query_path, runID, results_file)

    return top_100


def probabalistic_positional(query, index, docDict):

    # we have a list of all the documents (prepcoessing, in dict form)

    # for each doc, we do a calculation, store simliairty score and doc id on list
    # sort list, return list

    # need to figure out: qtf_j, D (size of document i?),

# 1- qtf_j refers to the frequency of j th term in the query .

# 2- |D| is the length of the document. D represents the document itself.
    k_1 = 1.2
    k_2 = 500
    b = 0.55

    numDoc = len(docDict)

    N = len(docDict) # total number of documents in a collection

    # print 'numDoc: ', numDoc, ', size docDict: ', len(docDict)

    SC_Doc_i = 0

    sum_docLengths = 0
    for k, v in docDict.iteritems():
        sum_docLengths += int(v)
    avgdl = sum_docLengths / float(len(docDict))

    Scores = {}

    docs_AdjacentQueryTerms = {}

    # pdb.set_trace()

    # postingList1 =

    # postingList2 =

     # only use terms that appear next to each other (that is, they form a word n-gram) to calculate the score of a document.

     # go through query terms and build Dict of documents that have query terms appearing next to each other

     # check against this dictionary to determine whether the score of that document should be calculated

    # for docID, D_size in docDict.iteritems():
    for term_index in range(len(query)):

        term = query[term_index]

        if (term_index + 1) < len(query):
            termNext = query[term_index + 1]

            if term in index.keys() and termNext in index.keys():

                # obtain  posting list for both
                PL_1 = index[term][1:]
                PL_2 = index[termNext][1:]

                intersectionDocDict = positional_interesect(PL_1, PL_2, 1)

                # pdb.set_trace()

                # check all other terms;

                # pdb.set_trace()

                # for docID in intersectionDocDict.iteritems():

                if term in index:
                    n = int(index[term][0]) # number of documents that contain the query term
                    w = math.log10((N - n +0.5) / (n + 0.5))

                    qtf_j = 0 # obtain qtf_j
                    for queryTerm in query:
                        if (term == queryTerm):
                            qtf_j += 1

                    postingList = []
                    for x in range(len(index[term]) - 1):         # obtain posting list l
                        # print index[term]
                        postingList.append(index[term][x+1])

                    # print 'postingList: ', term, ' ', postingList
                    # obtain df
                    df_term_i = int(float(index[term][0]))
                    # print 'df_term_i: ', df_term_i

                    # obtain idf
                    idf_term_i = math.log10(numDoc / df_term_i)
                    # print 'idf_term_i: ', idf_term_i

                    tf_ij = 0
                    for entry in postingList:

                        docID = entry[0]
                        doc_i_length = int(docDict[docID])
                        # if entry[0] == Doc_i_Name:
                        tf_ij = int(float(entry[1]))
                        # print 'tf_ij: ', tf_ij

                        if docID not in Scores and docID in intersectionDocDict.keys():  # CHECK THIS
                            Scores[docID] = 0

                        # else:
                            # increase value by appropriate amount
                        numer_big = (k_1 + 1) * tf_ij
                        denom_big = tf_ij + k_1 * (1 - b + b * (doc_i_length / float(avgdl) ))

                        # print 'numer_big: ', numer_big
                        # print 'denom_big: ', denom_big

                        if docID in intersectionDocDict.keys():

                            Scores[docID] += w * (numer_big / denom_big) * (((k_2 + 1) * qtf_j) / float( (k_2 + qtf_j) ) )
                        # pdb.set_trace()

                    # postingList = []
                    # for x in range(len(index[term]) - 1):         # obtain posting list l
                    #     # print index[term]
                    #     postingList.append(index[term][x+1])

        else: # fewer than three terms

            if term in index:
                    n = int(index[term][0]) # number of documents that contain the query term
                    w = math.log10((N - n +0.5) / (n + 0.5))

                    qtf_j = 0 # obtain qtf_j
                    for queryTerm in query:
                        if (term == queryTerm):
                            qtf_j += 1

                    postingList = []
                    for x in range(len(index[term]) - 1):         # obtain posting list l
                        # print index[term]
                        postingList.append(index[term][x+1])

                    # print 'postingList: ', term, ' ', postingList
                    # obtain df
                    df_term_i = int(float(index[term][0]))
                    # print 'df_term_i: ', df_term_i

                    # obtain idf
                    idf_term_i = math.log10(numDoc / df_term_i)
                    # print 'idf_term_i: ', idf_term_i

                    tf_ij = 0
                    for entry in postingList:

                        docID = entry[0]
                        doc_i_length = int(docDict[docID])
                        # if entry[0] == Doc_i_Name:
                        tf_ij = int(float(entry[1]))
                        # print 'tf_ij: ', tf_ij

                        if docID not in Scores: #and docID in intersectionDocDict.keys():  # CHECK THIS
                            Scores[docID] = 0

                        # else:
                            # increase value by appropriate amount
                        numer_big = (k_1 + 1) * tf_ij
                        denom_big = tf_ij + k_1 * (1 - b + b * (doc_i_length / float(avgdl) ))

                        # print 'numer_big: ', numer_big
                        # print 'denom_big: ', denom_big

                        # if docID in intersectionDocDict.keys():

                        Scores[docID] += w * (numer_big / denom_big) * (((k_2 + 1) * qtf_j) / float( (k_2 + qtf_j) ) )



    top100 = get_top100(Scores)

    return top100

def probabalistic_EVAL_positional(index, docMap, results_file, query):
    rem_stopWords = True
    runID = 'PROBABALISTIC'

    queryResults = []

    # open query file, process queries, tokineze, create list
    # queryList = obtainQueries(query_path) # generic
    if rem_stopWords:
        # for query in queryList:
        query = removeStopWords(query)


    # CREATE INVERTED INDEX
    index = {}
    numDoc = 0
    with open(lexicon_fName) as f:
        fLines  = f.readlines()
        for i in range(len(fLines) - 1) :
            term = [x.strip() for x in fLines[i].split(',')]   # need to convert from string to list
            myList = []
            myList.append(term[1])
            index[term[0]] = myList
        # numDoc = int(float(fLines[-1]))  # TOTAL NUMBER DOCUMENTS ?

    # part two of creating inverted index --> obtain posting lists and append to lexicon to created inverted index
    with open(postingList_fName) as f:
        fLines  = f.readlines()
        for line in fLines:
            term = [x.strip() for x in line.split(',')]   # need to convert from string to list


            postingList_term = []
            postingList_term.append(term[1])
            postingList_term.append(term[2])

            for location in term[3:]:
                postingList_term.append(location)

            # pdb.set_trace()

            index[term[0]].append(postingList_term)

    top_100 = probabalistic_positional(query, index, docMap)
    # line = []
    #     line.append(query)
    #     line.append(top_100)
    #     queryResults.append(line)

    # createTrecFile(queryResults, query_path, runID, results_file)

    return top_100





# ----------------------------------------------------------------------------------

# gonna need to change probabalistic_EVAL to take a single query at a time, rather than a queryLIst

# queryPath = '/Users/Konrad/Desktop/Mini-Trec-Data/QueryFile/queryfile.txt'

# queryList1 = obtainQueries_single(queryPath)
# queryList2 = obtainQueries_stem(queryPath)
# queryList3 = obtainQueries_phrase(queryPath, 2)

# print
# print 'queryList1'
# for term in queryList1:
#     print term

# print
# print 'queryList2'
# for term in queryList2:
#     print term

# print
# print 'queryList3'
# for term in queryList3:
#     print term



if (len(sys.argv) != 4):
    print "ERROR: invalid number of arguments: ", len(sys.argv) - 1

else:

    time1 = time.clock()

        #         """
    #         Same as static query processing, except that you need to dynamically change the index you are querying based on the thresholds (please carefully read assignment instructions). Takes 3 arguments.
    #         1) [index-directory-path] takes the path to the directory where you store your index files (the [output] of the "build index" step).
    #         2) [query-file-path] path to the query file
    #         3) [results-file] is the path to the results file, this file will be run with trec_eval to get the performance of your system. Please have your program create directories if necessary.
    #         """

#    python query_dynamic.py /tmp/my-indexes/  /Users/Konrad/Desktop/Mini-Trec-Data/QueryFile/queryfile.txt /Users/Konrad/Desktop/Project2/query_dynamic.txt
    index_directory_path = sys.argv[1]
    query_file_path = sys.argv[2]
    results_file_path = sys.argv[3]

    frequency_threshold_DF = 20
    frequency_threshold_Returned_Docs = 20

    docMap = obtainDocMap(index_directory_path)

    results_dir = re.findall(r'(^\/.+\/).+', results_file_path, flags=0) # get path for directory
    results_dir = results_dir[0]

    results_fName = re.findall(r'^\/.+\/(.+)', results_file_path, flags=0) # get path for directory
    results_fName = results_fName[0]

    if not os.path.exists(results_dir): # check if dirctory exists, create if it doesn't
        os.makedirs(results_dir)

    lexicon_fName = index_directory_path
    postingList_fName = index_directory_path

    # determine lexicon_fName and postingList_fName based on index type --> in if statements

    lexicon_fName += 'tripleList_Disk_LEXICON_PHRASE2'
    postingList_fName += 'tripleList_Disk_FINAL_PHRASE2'
    index_phrase = {}
    numDoc = 0
    with open(lexicon_fName) as f:
        fLines  = f.readlines()
        for i in range(len(fLines) - 1) :
            term = [x.strip() for x in fLines[i].split(',')]   # need to convert from string to list
            myList = []
            myList.append(term[1])
            index_phrase[term[0]] = myList
        # numDoc = int(float(fLines[-1]))  # TOTAL NUMBER DOCUMENTS ?

    # part two of creating inverted index --> obtain posting lists and append to lexicon to created inverted index
    with open(postingList_fName) as f:
        fLines  = f.readlines()
        for line in fLines:
            term = [x.strip() for x in line.split(',')]   # need to convert from string to list
            postingList_term = []
            postingList_term.append(term[1])
            postingList_term.append(term[2])
            index_phrase[term[0]].append(postingList_term)


    lexicon_fName = index_directory_path
    postingList_fName = index_directory_path
    lexicon_fName += 'tripleList_Disk_LEXICON_STEM'
    postingList_fName += 'tripleList_Disk_FINAL_STEM'
    index_stem = {}
    numDoc = 0
    with open(lexicon_fName) as f:
        fLines  = f.readlines()
        for i in range(len(fLines) - 1) :
            term = [x.strip() for x in fLines[i].split(',')]   # need to convert from string to list
            myList = []
            myList.append(term[1])
            index_stem[term[0]] = myList
        # numDoc = int(float(fLines[-1]))  # TOTAL NUMBER DOCUMENTS ?

    # part two of creating inverted index --> obtain posting lists and append to lexicon to created inverted index
    with open(postingList_fName) as f:
        fLines  = f.readlines()
        for line in fLines:
            term = [x.strip() for x in line.split(',')]   # need to convert from string to list
            postingList_term = []
            postingList_term.append(term[1])
            postingList_term.append(term[2])
            index_stem[term[0]].append(postingList_term)



    queryList_phrase = obtainQueries_phrase(query_file_path, 2)
    queryList_stem = obtainQueries_stem(query_file_path)

    # START HERE --> use pdb to look at structure of both query lists --> want phrase to have multiple terms per query
    # pdb.set_trace()

    # NEED TO do choice of index on a per query term bases      !!!!
    # pdb.set_trace()

    queryResults = []

    stemCount = 0
    phraseCount = 0
    positionalCount = 0

    # pdb.set_trace

    for query_term_list_index in range(len(queryList_phrase)):

        top_100 = []

        query_stem =  queryList_stem[query_term_list_index]
        if queryList_phrase[query_term_list_index]:
            queries_phrased_list = queryList_phrase[query_term_list_index]

            # pdb.set_trace()

            df_sum = 0
            for query_term in queries_phrased_list:
                # print query_term
                # pdb.set_trace()
                if query_term in index_phrase.keys():
                    # pdb.set_trace()
                    df_sum += int(index_phrase[query_term][0])

            # print 'df_sum: ', df_sum

        if df_sum > frequency_threshold_DF:
        # phrase index
            phraseCount += 1

            # print 'Sum of document frequences for phrases in query: ', df_sum, ', meets threshold: ', frequency_threshold, '. Using positional index.'
            top_100 = probabalistic(queries_phrased_list, index_phrase, docMap)
            num_retrieved_docs = len(top_100)
            # print 'num_retrieved_docs, phrase: ', num_retrieved_docs

        # check for number of retreived documents --> if too few or no the, switch to single index

            if num_retrieved_docs < frequency_threshold_Returned_Docs:
                phraseCount -= 1
                stemCount += 1

                # query_single = queryList_single[query_term_list_index][0]
                # print 'Too few retreived docs: ', num_retrieved_docs, '. Switching to single index.'

                top_100 = probabalistic(query_stem, index_stem, docMap)

    #  load index

        else:
        # proximity
            # print 'Sum of document frequences for phrases in query: ', df_sum, ', does not meet threshold: ', frequency_threshold, '. Switching to phrase index.'
            positionalCount += 1

            # query_single = queryList_single[query_term_list_index][0]
            # queryList = obtainQueries_positional(query_file_path)



            top_100 = probabalistic_positional(query_stem, index_stem, docMap)
            num_retrieved_docs = len(top_100)
            # print 'positional, num_retrieved_docs: ', num_retrieved_docs
            # pdb.set_trace()

            if num_retrieved_docs < frequency_threshold_Returned_Docs:
                positionalCount -= 1
                stemCount += 1

                # print 'Too few retreived docs: ', num_retrieved_docs, '. Switching to single index.'
                # query_single = queryList_single[query_term_list_index][0]

                top_100 = probabalistic(query_stem, index_stem, docMap)

        queryResults.append(top_100)

    runID = 'dynamic'

    createTrecFile(queryResults, query_file_path, runID, results_file_path)

    time1End = time.clock()

    print 'phrase index usage: ', phraseCount
    print 'positional index usage: ', positionalCount
    print 'stem index usage: ', stemCount

    print 'Created ', results_fName
    print 'time: ', (time1End - time1)




        # check for number of retreived documents --> if too few or no the, switch to stem index

    # determine whether phrase terms are common (high df)
        # if do, phrase index

        # else, send to proximity

    # CHECK:
    #  check for the number of retrieved documents.
    #  If you are retrieving no or too few documents you should switch your index dynamically and the query to the single or stem index.

