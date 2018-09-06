import glob
import numpy as np
from py2casefold import casefold
from bs4 import BeautifulSoup, Comment
import codecs
from nltk.corpus import stopwords
import re
import math
import pdb
import sys
import os
import time
from nltk.stem import *

    #         """
    #         Static query processing. Runs the specified retrieval model on the specified index type. Accepts 5 arguments.
    #         1) [index-directory-path] takes the path to the directory where you store your index files (the [output] of the "build index" step).
    #         2) [query-file-path] path to the query file
    #         3) [retrieval-model] can one of the following: "cosine", "bm25", "lm"
    #         4) [index-type] one of the following: "single", "stem"
    #         5) [results-file] is the path to the results file, this file will be run with trec_eval to get the performance of your system. Please have your program create directories if necessary.
    #         """

# ------------------------------------------------------------------------------------------------------------


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

# read list of queries from query file
# are tagged, and need to be pre-processed so as to identify query terms

# for usage of stem-index, need to stem query terms at time of query processing (Porter again)

# use the title part of the queries for retrieving document

# need to identify *special terms* in the same way that I have identified them in the documents.

# in the same way that phrases are generated in the collection, create phrases from the query terms.

def obtainQueries_single(filename): # returns a list of normalized querys (tokenized) from the query file
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
        query_list.append(tokens)
        # print title
    return query_list

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

# returns dictionary with docIDs as keys, and the number of tokens corresponding to the docID as values
# def writeDocMap(path, stopWords):  # THIS SHOULD BE PART OF INDEX CONSTRUCTION

#     docMapName = 'docMap'
#     docNo_list = []
#     token_list = []

#     for filename in glob.glob(path):

#         file_stream = codecs.open(filename, 'r', 'utf-8-sig')
#         soup = BeautifulSoup(file_stream, 'html.parser')

#         docDict = {}
#         stop = set(stopwords.words('english'))

#         docNo_expression  = re.compile('^<docno> (.*) </docno>$' )
#         for docNo in soup.find_all('docno', string=True):
#             # APPEND TO LIST
#             result = re.findall(docNo_expression, str(docNo)) # remove <doc> tags
#             docNo_list.append(result[0])

#         text_expression  = re.compile('^<text>\s+(.*)\s+</text>$' )
#         for text in soup.find_all('text'):
#             # REMOVE COMMENTS
#             comments = soup.findAll(text=lambda text:isinstance(text, Comment))
#             [comment.extract() for comment in comments]
#             # print text
#             # result = re.findall(text_expression, str(text)) # remove <doc> tags

#             # REMOVE HTML delimiters

#             text = re.sub('<[\w][^>]*>', '', str(text))
#             text = re.sub('<\/?[\w][^>]*>', '', str(text))

#             text = textNorm(text)

#             tokens = text.split()

#             if stopWords:
#                 tokens = [i for i in tokens if i not in stop]  # REMOVE STOP WORDS

#             # text = unicode(text, errors='ignore')
#             token_list.append(tokens)

#         for x in range(len(docNo_list)):
#             docDict[docNo_list[x]] = len(token_list[x])

#         with open(docMapName, 'w') as f:
#                 for key, value in docDict.iteritems():
#                     line = str(key) + ', ' + str(value) + '\n'
#                     print line
#                     f.write(line)

        # - END TOKENIZATION -

def obtainDocMap():
    docMap_name = 'docMap'
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

# def stem(listOfText):

#     return listOfText

def tokenize(text):
    tokens = text.split()
    return tokens

# def phrase():

def removeStopWords(tokenList):
    stop = set(stopwords.words('english'))
    tokenList = [i for i in tokenList if i not in stop]
    return tokenList

# END PREPROCESSING

# BEGIN relevance ranking

# query processing

# vector space model using cosine measure - use normlized tf-df
    # each dimension represents tf-idf for on term
    # documents are ranked by closeness to the query. Closeness is determined by a simliarity score calculation
    # use terms weichts to rank the relevance
        # parameters in calculating a weight for a document term or query term:
        # tf --> number of times a term i appears in document j (tf [tj])
        # df --> number of documents a term i appears in, (df i)
        # idf --> a discriminating measure for a term i in collection (how descriminating term i is)
            # idf i = log10(n / df i)  where n is the number of documents

        # weights for term components --> classic to use tf.idf
        # good one: ( LOOK UP ON SLIDES )
         # dij is the frequency of term j in document i.

# returns score for all documents (sorte, for a given query

def vectorSpace(query, index, docDict):

    Scores = {}

    # N = len(docDict) # total number of documents in a collection

    for qTerm in query:
        # calculate w, tq
        # w = math.log10((N - n +0.5) / (n + 0.5))

        if qTerm in index:

            qtf = 0 # obtain qtf_j
            for queryTerm in query:
                if (qTerm == queryTerm):
                    qtf += 1

            # n = int(index[qTerm][0])

            df_qTerm = int(index[qTerm][0])

            # obtain idf
            idf_qTerm = math.log10(len(docDict) / float(df_qTerm))
            # w = math.log10((N - n +0.5) / (n + 0.5))

            # obtain posting list for qTerm
            # postingList = [ index[x] for x in range(len(index[term_i]) - 1) ] # skip the first element, which is df
            postingList = []
            for x in range(len(index[qTerm]) - 1):         # obtain posting list l
                # print index[term_i]
                postingList.append(index[qTerm][x+1])

            # obtain weight_denom for current term (by summation over all items in posting list)
            w_currDoc_denom = 0
            for pair in postingList:
                tf_currDoc = int(pair[1])
                w_currDoc_denom += math.pow( ((math.log(tf_currDoc) + 1.0) * idf_qTerm), 2 )

            for pair in postingList:
                currDoc = pair[0]

                if currDoc not in Scores:
                    Scores[currDoc] = 0

                tf_currDoc = int(pair[1])

                w_currDoc_numer = (math.log(tf_currDoc) + 1.0) * idf_qTerm

                w_currDoc = w_currDoc_numer / w_currDoc_denom

                Scores[pair[0]] += tf_currDoc * w_currDoc # * qtf

    for docID, docLength in Scores.iteritems():
        # pdb.set_trace()
        Scores[docID] = Scores[docID] / float(docDict[docID])

    top100 = get_top100(Scores)

    return top100



# probabalistic model - use BM25
# returns the score for one document
def probabalistic(query, index, docDict):

    # we have a list of all the documents (prepcoessing, in dict form)

    # for each doc, we do a calculation, store simliairty score and doc id on list
    # sort list, return list

    # need to figure out: qtf_j, D (size of document i?),

# 1- qtf_j refers to the frequency of j th term in the query .

# 2- |D| is the length of the document. D represents the document itself.
    k_1 = 1.2
    k_2 = 500
    b = 0.55

    # numDoc = len(docDict)

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

            # # print 'postingList: ', term, ' ', postingList
            # # obtain df
            # df_term_i = int(float(index[term][0]))
            # # print 'df_term_i: ', df_term_i

            # # obtain idf
            # idf_term_i = math.log10(N / df_term_i)
            # # print 'idf_term_i: ', idf_term_i

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

    top100 = get_top100(Scores)

    return top100

# language model
def language(query, index, docDict): # Dirichlet
    # print 'test'

    # index = {}
    # numDoc = 0
    # with open(lexicon_fName) as f:
    #     fLines  = f.readlines()
    #     for i in range(len(fLines) - 1) :
    #         term = [x.strip() for x in fLines[i].split(',')]   # need to convert from string to list
    #         myList = []
    #         myList.append(term[1])
    #         # if term[1] > 1:
    #         index[term[0]] = myList
    #     numDoc = int(float(fLines[-1]))  # TOTAL NUMBER OF DOCUMENTS

    # with open(postingList_fName) as f:
    #     fLines  = f.readlines()
    #     for line in fLines:
    #         term = [x.strip() for x in line.split(',')]   # need to convert from string to list
    #         postingList_term = []
    #         postingList_term.append(term[1])
    #         postingList_term.append(term[2])
    #         index[term[0]].append(postingList_term)

    # pdb.set_trace()
    Scores = {}

    # mew = average document length
    sizeSum = 0
    for docID, docSize in docDict.iteritems():
        sizeSum += int(docSize)
    mew = sizeSum / len(docDict)

    # mew = 1

    #  set to extremes
    # CHANGE TO 2000?

    # C = total number of terms in collection
    C = len(index)

    # for docID, D_size in docDict.iteritems():
    for Qtoken in query:
        if Qtoken in index:

            postingList = []
            for x in range(len(index[Qtoken]) - 1):         # obtain posting list l
                # print index[term]
                postingList.append(index[Qtoken][x+1])

            tf_Qtoken_i_C = sum([int(Qtoken[1]) for Qtoken in postingList])

            for entry in postingList:

                D_size = int(docDict[entry[0]])
                # if entry[0] == Doc_i_Name:
                tf_Qtoken_i_D = int(float(entry[1]))
                # print 'tf_ij: ', tf_ij

                docID_pl = entry[0]
                # pdb.set_trace()
                if docID_pl not in Scores:
                    Scores[docID_pl] = 0

            # obtain tf_Qtoken_i_D --> term freq of a given document
            # obtain tf_Qtoken_i_C --> sum all doc freqs in a given posting list
                numerator = tf_Qtoken_i_D + mew * (tf_Qtoken_i_C / float(C) )
                denominator = ( int(D_size) + mew)

                # pdb.set_trace()

                # pdb.set_trace()
                Scores[docID_pl] += numerator / float( denominator)


    # go through all scores and take log of them
    for docID, score in Scores.iteritems():
        Scores[docID] = math.log10(Scores[docID])
        # print score

    top100 = get_top100(Scores)

    return top100
    # return Scores


# P(Q | Model D) *= P( q_i | D ) for all q_i in Q

# Maximum liklihood (ML) estimate, defined as:
    # Tf of query term appearing in doc
    # divided by document length
    # P( q_i | D ) = (tf of query term i appearing in doc + mew * (tf of query term i appearing in entire collection ) / ( number of terms in collection )) / ( divided by doc length + mew )

# P


def printScores(Scores):
    d_view = [ (v,k) for k,v in Scores.iteritems() ]
    d_view.sort(reverse=True) # natively sort tuples by first element
    for v,k in d_view:
        print k, v


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


            for results in query[1:]: # ignore first index, which is query tokens

                rank = 0

                # if not results:
                #     pdb.set_trace()

                for result in results:

                    # obtain variables:
                    queryID = queryID_list[queryCount]
                    iter_ = 0
                    docno = result[0]
                    # pdb.set_trace()
                    sim = result[1]

                    # query_id, iter, docno, rank, sim, run_id
                    string = str(queryID) + ' ' + str(iter_) + ' ' + docno + ' ' + str(rank) + ' ' + str(sim) + ' ' + runID + '\n'

                    f.write(string)

                    # write line:


                    rank += 1

            queryCount += 1

# parameters:
        # lexicon_fName
        # postingList_fName
        # query_path
        # docMap --> list of all documents in collection, with sizes of each file
# returns:
    # list of top 100 documents for each query
def vectorSpace_EVAL(lexicon_fName, postingList_fName, docMap, results_file, queryList, query_path):
    runID = 'VECTORSPACE'

    rem_stopWords = True

    queryResults = []



    # open query file, process queries, tokineze, create list
    # queryList = obtainQueries(query_path) # generic
    if rem_stopWords:
        for query in queryList:
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
            index[term[0]].append(postingList_term)

    for query in queryList:
        top_100 = vectorSpace(query, index, docMap)
        line = []
        line.append(query)
        line.append(top_100)
        queryResults.append(line)

    createTrecFile(queryResults, query_path, runID, results_file)

    # return queryResults

def probabalistic_EVAL(lexicon_fName, postingList_fName, docMap, results_file, queryList, query_path):
    rem_stopWords = True
    runID = 'PROBABALISTIC'

    queryResults = []

    # open query file, process queries, tokineze, create list
    # queryList = obtainQueries(query_path) # generic
    if rem_stopWords:
        for query in queryList:
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
            index[term[0]].append(postingList_term)

    for query in queryList:
        top_100 = probabalistic(query, index, docMap)
        line = []
        line.append(query)
        line.append(top_100)
        queryResults.append(line)

    createTrecFile(queryResults, query_path, runID, results_file)

    # return queryResults


# START HERE --> need to update similiarty calcualtion functions for probablistic and language

def language_EVAL(lexicon_fName, postingList_fName, docMap, results_file, queryList, query_path):
    rem_stopWords = True

    runID = 'LANGUAGE'

    queryResults = []

    # open query file, process queries, tokineze, create list
    # queryList = obtainQueries(query_path) # generic



    if rem_stopWords:
        for query in queryList:
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

    # pdb.set_trace()

    # count = 0
    for query in queryList:
        top_100 = language(query, index, docMap)
        line = []
        line.append(query)
        line.append(top_100)
        queryResults.append(line)
        # count += 1
        # if count == 25:
        #     pdb.set_trace()


    createTrecFile(queryResults, query_path, runID, results_file)

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




# ------------------------------------------------------------------------------------------------------------

# query_static [index-directory-path] [query-file-path] [retrieval-model] [index-type] [results-file]

# python query.py ./indexes/my-indexes/ ./data/queryfile.txt bm25 stem ./results/results-bm25-stem.txt


# python query.py /tmp/my-indexes/  /Users/Konrad/Desktop/Mini-Trec-Data/QueryFile/queryfile.txt cosine single /Users/Konrad/Desktop/Project2/results-cosine-single.txt
# --
# python query.py /tmp/my-indexes/  /Users/Konrad/Desktop/Mini-Trec-Data/QueryFile/queryfile.txt bm25 single /Users/Konrad/Desktop/Project2/results-bm25-single.txt
# python query.py /tmp/my-indexes/  /Users/Konrad/Desktop/Mini-Trec-Data/QueryFile/queryfile.txt lm single /Users/Konrad/Desktop/Project2/results-lm-single.txt
# python query.py /tmp/my-indexes/  /Users/Konrad/Desktop/Mini-Trec-Data/QueryFile/queryfile.txt cosine stem /Users/Konrad/Desktop/Project2/results-cosine-stem.txt
# python query.py /tmp/my-indexes/  /Users/Konrad/Desktop/Mini-Trec-Data/QueryFile/queryfile.txt bm25 stem /Users/Konrad/Desktop/Project2/results-bm25-stem.txt
# python query.py /tmp/my-indexes/  /Users/Konrad/Desktop/Mini-Trec-Data/QueryFile/queryfile.txt lm stem /Users/Konrad/Desktop/Project2/results-lm-stem.txt



#       ./trec_eval qrel.txt VECTORSPACE.trec


# /Users/Konrad/Desktop/Mini-Trec-Data/BigSample/ single /tmp/my-indexes/


if (len(sys.argv) != 6):
    print "ERROR: invalid number of arguments: ", len(sys.argv) - 1

else:

    index_directory_path = sys.argv[1]
    query_file_path = sys.argv[2]
    retrieval_mode = sys.argv[3]
    index_type = sys.argv[4]
    results_file_path = sys.argv[5]

    docMap = obtainDocMap(index_directory_path)

    # (^\/.+\/)(.+)

    results_dir = re.findall(r'(^\/.+\/).+', results_file_path, flags=0) # get path for directory
    results_dir = results_dir[0]

    results_fName = re.findall(r'^\/.+\/(.+)', results_file_path, flags=0) # get path for directory
    results_fName = results_fName[0]

    if not os.path.exists(results_dir): # check if dirctory exists, create if it doesn't
        os.makedirs(results_dir)

    lexicon_fName = index_directory_path
    postingList_fName = index_directory_path

    # determine lexicon_fName and postingList_fName based on index type

    queryList = []

    if (index_type == "single"):

        time1 = time.clock()
        lexicon_fName += 'tripleList_Disk_LEXICON_SINGLE'
        postingList_fName += 'tripleList_Disk_FINAL_SINGLE'
        queryList = obtainQueries_single(query_file_path)

    elif (index_type == "stem"):

        time1 = time.clock()
        lexicon_fName += 'tripleList_Disk_LEXICON_STEM'
        postingList_fName += 'tripleList_Disk_FINAL_STEM'
        queryList = obtainQueries_stem(query_file_path)

    else:
        print 'ERROR: invalid index_type: ', index_type


    if (retrieval_mode == "cosine"):

        # add path to directory
        vectorSpace_EVAL(lexicon_fName, postingList_fName, docMap, results_file_path, queryList, query_file_path)
        time1End = time.clock()
        print 'Created ', results_fName
        print 'time: ', (time1End - time1)

    elif (retrieval_mode == "bm25"):

        probabalistic_EVAL(lexicon_fName, postingList_fName, docMap, results_file_path, queryList, query_file_path)
        time1End = time.clock()
        print 'Created ', results_fName
        print 'time: ', (time1End - time1)

    elif (retrieval_mode == "lm"):

        language_EVAL(lexicon_fName, postingList_fName, docMap, results_file_path, queryList, query_file_path)
        time1End = time.clock()
        print 'Created ', results_fName
        print 'time: ', (time1End - time1)
    else:
        # error
        print 'ERROR: invalid retrieval_mode: ', retrieval_mode



