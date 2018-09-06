
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

stemmer = PorterStemmer()

# DESIGN: interface --> (only one type of classifer and index) reduction, expansion etc.

# concerns:
# runtime
# whether I'm returning too many top documents

# implement pseudo relevance (experiment with N and T)
# reducing: query threshhold, using IDF to reduce terms in query



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


def obtainQueries_stem_long(filename):
    stemmer = PorterStemmer()

    file_stream = codecs.open(filename, 'r', 'ascii')
    soup = BeautifulSoup(file_stream, 'html.parser')

    query_list = []

    title_expression  = re.compile(r'^<narr> Narrative: \s+([\w\s\S]*)\s+')

    for text in soup.find_all('narr'):

        result = re.findall(title_expression, str(text)) #extract title fields

        result = result[0].strip()  # remove trailing and leading spaces

        # normalization here
        result = textNorm(result) # normalize text
        tokens = tokenize(result)

        tokens = [stemmer.stem(token).encode('ascii', 'ignore') for token in tokens]

        query_list.append(tokens)
        # print title
    return query_list

def obtainQueries_stem(filename): # returns a list of normalized querys (tokenized) from the query file
    stemmer = PorterStemmer()

    file_stream = codecs.open(filename, 'r', 'ascii')
    soup = BeautifulSoup(file_stream, 'html.parser')

    query_list = []

    title_expression  = re.compile(r'^<title> Topic: ([\w -]+)' )
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

# probabalistic model - use BM25
# returns the score for one document
def probabalistic(query, index, docDict, N_top):

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

    topN = get_topN(Scores, N_top)

    return topN



def printScores(Scores):
    d_view = [ (v,k) for k,v in Scores.iteritems() ]
    d_view.sort(reverse=True) # natively sort tuples by first element
    for v,k in d_view:
        print k, v


def get_topN(Scores, N):
    top_N = []

    d_view = [ (v,k) for k,v in Scores.iteritems() ]
    d_view.sort(reverse=True) # natively sort tuples by first element

    count = 0
    for v, k in d_view:
        if count < N:
            # print k, v
            tempList = []
            tempList.append(k)
            tempList.append(v)
            top_N.append(tempList)
            count += 1

    return top_N

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

                for result in results:

                    # obtain variables:
                    queryID = queryID_list[queryCount]
                    iter_ = 0
                    docno = result[0]
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

    # return queryResults

def create_index(lexicon_fName, postingList_fName):
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

    return index

def probabalistic_EVAL(lexicon_fName, postingList_fName, docMap, results_file, queryList, query_path, N, createTrec):
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
        top_N = probabalistic(query, index, docMap, N)
        line = []
        line.append(query)
        line.append(top_N)
        queryResults.append(line)

    if createTrec:
        createTrecFile(queryResults, query_path, runID, results_file)
    else:
        return queryResults, index

    #

    # return queryResults


# START HERE --> need to update similiarty calcualtion functions for probablistic and language

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

def obtain_docID_text(output_dir):
    docMap_name = output_dir + 'docID_text'
    docID_text = {}

    with open(docMap_name, 'r') as file:
        num_lines = sum(1 for line in file)
        file.seek(0,0)
        count = 0
        while count < num_lines:
            lineString = file.readline()
            line = [x.strip() for x in lineString.split(',')]   # need to convert from string to list
            # print line

            if line[0] not in docID_text.keys():
                # print currTriple[1], ' added to document dictionary'
                docID_text[line[0]] = line[1:]
            else:
                print 'ERROR in obtain_docID_text() --> duplicate doc ID encountered'

            count += 1

    return docID_text




# ------------------------------------------------------------------------------------------------------------

# query_static [index-directory-path] [query-file-path] [retrieval-model] [index-type] [results-file]

# python query.py ./indexes/my-indexes/ ./data/queryfile.txt bm25 stem ./results/results-bm25-stem.txt


# python query.py /tmp/my-indexes/  /Users/Konrad/Desktop/Mini-Trec-Data/QueryFile/queryfile.txt cosine single /Users/Konrad/Desktop/Project2/results-cosine-single.txt
# --
# python query.py /tmp/my-indexes/  /Users/Konrad/Desktop/Mini-Trec-Data/QueryFile/queryfile.txt bm25 single /Users/Konrad/Desktop/Project2/results-bm25-single.txt
# python query.py /tmp/my-indexes/  /Users/Konrad/Desktop/Mini-Trec-Data/QueryFile/queryfile.txt lm single /Users/Konrad/Desktop/Project2/results-lm-single.txt
# python query.py /tmp/my-indexes/  /Users/Konrad/Desktop/Mini-Trec-Data/QueryFile/queryfile.txt cosine stem /Users/Konrad/Desktop/Project2/results-cosine-stem.txt



# python expan_reduc.py /tmp/my-indexes/  /Users/Konrad/Desktop/Mini-Trec-Data/QueryFile/queryfile.txt bm25 stem /Users/Konrad/Desktop/Project3_K_Rauscher/results-bm25-stem.txt /Users/Konrad/Desktop/Mini-Trec-Data/BigSample/




# python expan_reduc.py /tmp/my-indexes/  /Users/Konrad/Desktop/Mini-Trec-Data/QueryFile/queryfile.txt lm stem /Users/Konrad/Desktop/Project2/results-lm-stem.txt



#       ./trec_eval qrel.txt VECTORSPACE.trec


# /Users/Konrad/Desktop/Mini-Trec-Data/BigSample/ single /tmp/my-indexes/

# given a list of doc ids, return a dict containg all of the terms for specific docIDs

# REDO: get docDict once, then itterate theough for all docIDs and get all corresponding text

def getRelevent_docIDs_text(docIDList, docDict):
    dict_IDs_text = {}

    for docID in docIDList:
        dict_IDs_text[docID] = docDict[docID]

    return dict_IDs_text



# def getDocDict(path, rem_stopWords):

#     Lexicon = {}

#     postingList_Dict = {}  # term as key value, [docID, docfreq] as value
#     # global doc map
#     docDict = {}

#     for filename in glob.glob(path):
#         # print 'opening: ', filename
#         file_stream = codecs.open(filename, 'r', 'utf-8-sig')
#         soup = BeautifulSoup(file_stream, 'html.parser')

#         docNo_list = []
#         text_list = []

#         # isValidDocID = []

#         docNo_expression  = re.compile('^<docno> (.*) </docno>$' )

#         for docNo in soup.find_all('docno', string=True):
#             # APPEND TO LIST
#             result = re.findall(docNo_expression, str(docNo)) # remove <doc> tags
#             docID = result[0]

#             # if docID in docIDList:

#             docNo_list.append(docID)

#                 # isValidDocID.append(True)
#             # else:
#                 # isValidDocID.append(False)
#             # if ( str() )
#             # NEED to not append if docID not in docIDlist.
#             # also need way of keeping track for text appending


#         text_expression  = re.compile('^<text>\s+(.*)\s+</text>$' )
#         count = 0
#         for text in soup.find_all('text'):
#             # REMOVE COMMENTS
#             # if (isValidDocID[count]): # if theDocID corresponding to this body of text is in the docIDList

#             comments = soup.findAll(text=lambda text:isinstance(text, Comment))
#             [comment.extract() for comment in comments]
#             # print text
#             # result = re.findall(text_expression, str(text)) # remove <doc> tags

#             # REMOVE HTML delimiters

#             text = re.sub('<[\w][^>]*>', '', str(text))
#             text = re.sub('<\/?[\w][^>]*>', '', str(text))

#             text = textNorm(text)

#             # print text
#             # text = unicode(text, errors='ignore')
#             text_list.append(text)
#             count += 1

#         for x in range(len(docNo_list)):
#             # - TOKENIZATION -
#             stop = set(stopwords.words('english'))
#             # docDict[docNo_list[x]] = text_list[x] #\

#             tokens = text_list[x].split()

#             if rem_stopWords:
#                 tokens = [i for i in tokens if i not in stop]  # REMOVE STOP WORDS

#             tokens = [stemmer.stem(token) for token in tokens]   # STEM

#             docDict[docNo_list[x]] = tokens

#     return docDict
#     # SO AT THIS POINT docDict contains all doc IDs and their corresponding text fields that are tokenized
#             # postingList_Dict = {}  # term as key value, [docID, docfreq] as value


def get_top_T_terms(T, docID_to_text_DICT, index, docMap ):
    # NEED: idf, number of document in relevent set having term t

    # compile all terms into one list
    list_of_all_terms = []
    for docID, terms in docID_to_text_DICT.iteritems():
        list_of_all_terms.append(terms)

    list_of_all_terms = list_of_all_terms[0]

    list_of_all_terms = [ x for x in list_of_all_terms if not any(c.isdigit() for c in x) ]

    list_of_all_terms = [x.encode('ascii', 'ignore') for x in list_of_all_terms]

    scored_terms = []
    already_scored = []
    # for each term in list_of_all_terms, get idf, n and multiply --> make list of tuples (term, score)
    # n is the number of documents in relvent set having term t
    for term in list_of_all_terms:

        if (term in index and term not in already_scored) :

            already_scored.append(term)
            df_term = int(index[term][0])
            idf_term = math.log10(len(docMap)) / float(df_term)

            n = 0

            for docID in docID_to_text_DICT:
                if term in docID_to_text_DICT[docID]:
                    n += docID_to_text_DICT[docID].count(term)

            two_term = []
            two_term.append(term)
            two_term.append(idf_term * n)
            scored_terms.append(two_term)

    scored_terms = sorted(scored_terms, key=lambda x:x[1], reverse = True)

    scored_terms = scored_terms[0:T]


    top_T_terms = [row[0] for row in scored_terms]

    return top_T_terms



# CHANGE so that merely returns list of expanded queries
def pseudo_relevance_expansion(lexicon_fName, postingList_fName, docMap, results_file, queryList, query_path, N, T, trec_files_directory_path, index_directory_path):

    # WANT TO BE ABLE TO SPECIFY N, T

    # identify "good" (N top-ranked) documents
    queryResults, index = probabalistic_EVAL(lexicon_fName, postingList_fName, docMap, results_file, queryList, query_path, N, False)

    docID_text = obtain_docID_text(index_directory_path)

    queryList_expanded = []
    top_T_terms = []

    for queryInfo in queryResults:

        expanded_query = queryInfo[0]

        if len(queryInfo[1]) > 0: # if query has corresponding documents

            list_of_doc_IDs =  [  term[0] for term in queryInfo[1:][0] ]
            # identify all terms from the N top-ranked documents.

            docID_to_text_DICT = getRelevent_docIDs_text(list_of_doc_IDs, docID_text)



            # select the "good" (T top feedback terms)
            top_T_terms = get_top_T_terms(T, docID_to_text_DICT, index, docMap)

            # Merge the feedback terms with the original query

            for term in top_T_terms:
                expanded_query.append(term)

        queryList_expanded.append(expanded_query)

    return queryList_expanded

    # Identify the top ranked documents for the modified queries through relevance ranking


# use idf threshold
def query_reduction(queryList, index, docDict, idf_threshhold):

    queryList_reduced = []

    for query in queryList:
        # have check so that never drops below one queury term?
        # keep term with highest idf for this
        highest_idf = 0
        best_term = ""

        query_reduced = []
        alreadyAdded = []

        for qTerm in query:
            if qTerm in index:
                df_term = int(index[qTerm][0])
                idf_term = math.log10(len(docDict)) / float(df_term)

                if idf_term > highest_idf:
                    highest_idf = idf_term
                    best_term = qTerm

                if (idf_term > idf_threshhold) and qTerm not in alreadyAdded:
                    query_reduced.append(qTerm)
                    alreadyAdded.append(qTerm)

        if best_term not in query_reduced: # make sure that at least one term is in query, and that that term is has the highest idf
            query_reduced.append(best_term)

        queryList_reduced.append(query_reduced)

    return queryList_reduced



if (len(sys.argv) != 8):
    print "ERROR: invalid number of arguments: ", len(sys.argv) - 1

else:

    index_directory_path = sys.argv[1]
    query_file_path = sys.argv[2]
    retrieval_mode = sys.argv[3]
    index_type = sys.argv[4]
    results_file_path = sys.argv[5]
    trec_files_directory_path = sys.argv[6]
    trec_files_directory_path += "fr*"  # for glob
    query_type = sys.argv[7]

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
    # NEED TO: be able to select which type of queries will be selected from document --> short (Title) or long (Narrative)
    #  "none", "expand", "filter", and "expand-filter"

    if (index_type == "single"):

        # THROW ERROR
        print 'ERROR: invalid index_type: ', index_type

    elif (index_type == "stem"):

        # THROW ERROR
        lexicon_fName += 'tripleList_Disk_LEXICON_STEM'
        postingList_fName += 'tripleList_Disk_FINAL_STEM'
        queryList = obtainQueries_stem(query_file_path)

    else:
        print 'ERROR: invalid index_type: ', index_type

    if (retrieval_mode == "cosine"):

        # add path to directory
        # THROW ERROR
        print 'ERROR: invalid retrieval_mode: ', retrieval_mode

    elif (retrieval_mode == "bm25"):

        # probabalistic_EVAL(lexicon_fName, postingList_fName, docMap, results_file_path, queryList, query_file_path)
        N = 2 # number of documents to expand from
        T = 12 # number of queury expansion terms to add to original queury
        createTrec = True
        topN = 100
        idf_threshhold = .001

        if (query_type == "none"):
            probabalistic_EVAL(lexicon_fName, postingList_fName, docMap, results_file_path, queryList, query_file_path, topN, createTrec)
            print 'Created ', results_fName

        elif (query_type == "expand"):
            queryList_expanded = pseudo_relevance_expansion(lexicon_fName, postingList_fName, docMap, results_file_path, queryList, query_file_path, N, T, trec_files_directory_path, index_directory_path)
            # for item in queryList_expanded:
            #     print item
            probabalistic_EVAL(lexicon_fName, postingList_fName, docMap, results_file_path, queryList_expanded, query_file_path, topN, createTrec)
            print 'Created ', results_fName

        elif (query_type == "filter"):
            # need to get index to pass
            # need to use long queries (narrative)

            index = create_index(lexicon_fName, postingList_fName)

            queryList = obtainQueries_stem_long(query_file_path)

            queryList_reduced = query_reduction(queryList, index, docMap, idf_threshhold)
            # for item in queryList_reduced:
            #     print item
            probabalistic_EVAL(lexicon_fName, postingList_fName, docMap, results_file_path, queryList_reduced, query_file_path, topN, createTrec)
            print 'Created ', results_fName

        elif (query_type == "expand-filter"):
            N = 2 # number of documents to expand from
            T = 12 # number of queury expansion terms to add to original queury
            idf_threshhold = .001

            index = create_index(lexicon_fName, postingList_fName)
            queryList_expanded = pseudo_relevance_expansion(lexicon_fName, postingList_fName, docMap, results_file_path, queryList, query_file_path, N, T, trec_files_directory_path, index_directory_path)
            queryList_reduced_expanded = query_reduction(queryList_expanded, index, docMap, idf_threshhold)

            for item in queryList_reduced_expanded:
                print item
            probabalistic_EVAL(lexicon_fName, postingList_fName, docMap, results_file_path, queryList_reduced_expanded, query_file_path, topN, createTrec)


            print 'Created ', results_fName
            # print 'time: ', (time1End - time1)

        elif (retrieval_mode == "lm"):

            # THROW ERROR
            print 'ERROR: invalid retrieval_mode: ', retrieval_mode

        else:
            # error
            print 'ERROR: invalid retrieval_mode: ', retrieval_mode



