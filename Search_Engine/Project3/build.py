from nltk.tokenize import RegexpTokenizer
import collections
import glob
import pandas as pd
import numpy as np
import itertools
from py2casefold import casefold
import time
from bs4 import BeautifulSoup, Comment
import codecs
from nltk.corpus import stopwords
import re
from HTMLParser import HTMLParser
import pdb
import os
import sys
from nltk.stem import *


    #     Builds the index, accepts 3 arguments:
    #     1) [trec-files-directory-path] the directory containing the raw documents (e.g. fr940104.0)
    #     2) [index-type] can be one of the following: "single", "stem", "phrase", "positional"
    #     3) [output-dir] the directory where your index and lexicon files will be written (please make your program create it if doesn't exist).
    #     """


    # 1 /Users/Konrad/Desktop/Mini-Trec-Data(1)/BigSample/
    # 2 single
    # 3 /tmp/my-indexes/

    # python build.py /Users/Konrad/Desktop/Mini-Trec-Data/BigSample/ single /tmp/my-indexes/
    # python build.py /Users/Konrad/Desktop/Mini-Trec-Data/BigSample/ stem /tmp/my-indexes/
    # python build.py /Users/Konrad/Desktop/Mini-Trec-Data/BigSample/ phrase /tmp/my-indexes/
    # python build.py /Users/Konrad/Desktop/Mini-Trec-Data/BigSample/ positional /tmp/my-indexes/




# ------------------------------------------------------------------------------------------------------------



docIDs_dict = {}
terms_dict = {}

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

# --- FUNC DEF ---
# input: path for directory containing files to be processed
# output: write temp files to directory that this .py file is located in,
# containing triples (term, doc id, frequency) for each distintict term in each document, sorted by term and doc id.
# the number of triples in each temp file is limited by memory_constraint
# returns: a dictionary (key: docID, keyValue: tokens for that docID) of tokens for each data file, in the form of a list
def preProcessing(filename, memory_constraint, stopWords):
    print 'opening: ', filename
    file_stream = codecs.open(filename, 'r', 'utf-8-sig')
    soup = BeautifulSoup(file_stream, 'html.parser')

    docNo_list = []
    text_list = []

    docNo_expression  = re.compile('^<docno> (.*) </docno>$' )
    for docNo in soup.find_all('docno', string=True):
        # APPEND TO LIST
        result = re.findall(docNo_expression, str(docNo)) # remove <doc> tags
        docNo_list.append(result[0])

    text_expression  = re.compile('^<text>\s+(.*)\s+</text>$' )
    for text in soup.find_all('text'):
        # REMOVE COMMENTS
        comments = soup.findAll(text=lambda text:isinstance(text, Comment))
        [comment.extract() for comment in comments]
        # print text
        # result = re.findall(text_expression, str(text)) # remove <doc> tags

        # REMOVE HTML delimiters

        text = re.sub('<[\w][^>]*>', '', str(text))
        text = re.sub('<\/?[\w][^>]*>', '', str(text))

        text = textNorm(text)

        # print text
        # text = unicode(text, errors='ignore')
        text_list.append(text)

    docDict = {}
    for x in range(len(docNo_list)):
        docDict[docNo_list[x]] = text_list[x]


    # - TOKENIZATION -
    stop = set(stopwords.words('english'))

    for x in range(len(docDict)):
        # docDict[docNo_list[x]] = text_list[x] #\

        pattern = re.compile(r'[\w]?\\x[\w]*')
        tokens = docDict[docNo_list[x]].split()
        if stopWords:
            tokens = [i for i in tokens if i not in stop]  # REMOVE STOP WORDS
        docDict[docNo_list[x]] = tokens

    # - END TOKENIZATION -
    # import pdb; pdb.set_trace()
    return docDict



def constructLexicon(indexName, lexName, output_dir):

    Lexicon = {}
    Documents = {} # to keep track of # documents

    lexName = output_dir + lexName
    indexName = output_dir + indexName

    # pdb.set_trace()

    with open(indexName, 'r') as file:
        num_lines = sum(1 for line in file)
        file.seek(0,0)
        count = 0
        while count < num_lines:
            currTriple_string = file.readline()
            currTriple = [x.strip() for x in currTriple_string.split(',')]   # need to convert from string to list

            if currTriple[1] not in Documents.keys():
                # print currTriple[1], ' added to document dictionary'
                Documents[currTriple[1]] = True

            if currTriple[0] in Lexicon.keys():
                Lexicon[currTriple[0]] += 1 # increment df
            else:
                Lexicon[currTriple[0]] = 1 # assign doc freq of one
            count += 1


    lexiconList = []
    with open(lexName, 'w') as lexiconFile: # write lexicon to disk, with df
        for key, value in Lexicon.iteritems():
            term = []
            term.append(key)
            term.append(value)
            lexiconList.append(term)

        lexiconList = sorted(lexiconList)

        for item in lexiconList:
            # print item[0]
            line = str(item[0]) + ', ' + str(item[1]) + '\n'
            lexiconFile.write(line)
        lexiconFile.write(str(len(Documents))) # append # documents at end

    values = [v for v in Lexicon.values()] # convert all values to list, then perform operations on the list

    print ' '
    print '--- Index creation completed ---'
    print ' '
    print 'Index statistics:'
    # mean df
    print 'mean df: ', np.mean(values)

    # median df
    print 'median df: ', np.median(values)

    # max df
    print 'max df: ', np.max(values)

    # min df
    print 'min df: ', np.min(values)

    # print 'number of documents: ', len(Documents)
    print 'number of terms:', len(Lexicon)

    print 'number of documents: ', str(len(Documents))



def makeFinal_SINGLE(path, rem_stopWords, output_dir):
    # global posting list
    Lexicon = {}

    postingList_Dict = {}  # term as key value, [docID, docfreq] as value
    # global doc map
    docDict = {}

    for filename in glob.glob(path):
        print 'opening: ', filename
        file_stream = codecs.open(filename, 'r', 'utf-8-sig')
        soup = BeautifulSoup(file_stream, 'html.parser')

        docNo_list = []
        text_list = []

        docNo_expression  = re.compile('^<docno> (.*) </docno>$' )
        for docNo in soup.find_all('docno', string=True):
            # APPEND TO LIST
            result = re.findall(docNo_expression, str(docNo)) # remove <doc> tags
            docNo_list.append(result[0])

        text_expression  = re.compile('^<text>\s+(.*)\s+</text>$' )
        for text in soup.find_all('text'):
            # REMOVE COMMENTS
            comments = soup.findAll(text=lambda text:isinstance(text, Comment))
            [comment.extract() for comment in comments]
            # print text
            # result = re.findall(text_expression, str(text)) # remove <doc> tags

            # REMOVE HTML delimiters

            text = re.sub('<[\w][^>]*>', '', str(text))
            text = re.sub('<\/?[\w][^>]*>', '', str(text))

            text = textNorm(text)

            # print text
            # text = unicode(text, errors='ignore')
            text_list.append(text)

        for x in range(len(docNo_list)):
            # - TOKENIZATION -
            stop = set(stopwords.words('english'))
            # docDict[docNo_list[x]] = text_list[x] #\

            tokens = text_list[x].split()

            if rem_stopWords:
                tokens = [i for i in tokens if i not in stop]  # REMOVE STOP WORDS

            docDict[docNo_list[x]] = tokens

    # SO AT THIS POINT docDict contains all doc IDs and their corresponding text fields that are tokenized
            # postingList_Dict = {}  # term as key value, [docID, docfreq] as value

    print 'number of docIDs: ', len(docDict)
    # for each docID
    for docID, text_field in docDict.iteritems():

        alreadyProcessed = {}
        # for each text_token corresponding to the current docID's text field:
        for text_token in text_field:
            if text_token not in alreadyProcessed:
                alreadyProcessed[text_token] = True

                # iterate through text field, increment tf for that term
                term_freq_currDoc = 0
                for text_token_iter in text_field:
                    if text_token_iter == text_token:
                        term_freq_currDoc += 1


                # if term not already in postingList_Dict
                if text_token not in postingList_Dict:
                    # create new element in postingList_Dict, using docID and tf
                    postingList = []
                    postingList_term = [docID, term_freq_currDoc]
                    postingList.append(postingList_term)
                    postingList_Dict[text_token] = postingList

                else:
                    # append docID and tf to posting list of current term

                    postingList_term = [docID, term_freq_currDoc]
                    postingList_Dict[text_token].append(postingList_term)

        # print text_token, ' ', postingList_Dict[text_token]

    # PRINT TO FILE
    pl_fName = 'tripleList_Disk_FINAL_SINGLE'
    output_dir += pl_fName

    with open(output_dir, 'w') as f:
        for key in sorted(postingList_Dict):

            if ((len(postingList) > 1) or (  postingList_Dict[key][0][1] > 1  )  ):  # ignore terms that occur in only one document or only occur once

                postingList = postingList_Dict[key]

                # for each posting list term, write a line to file: term, docID, tf
                for term in postingList:
                    pl_string = str(key) + ', ' + term[0] + ', ' + str(term[1]) + '\n'
                    f.write(pl_string)
                    # print 'HERE'
                    # pdb.set_trace()


def makeFinal_POSITIONAL(path, remove_stopWords, output_dir):
    # global posting list
    Lexicon = {}

    postingList_Dict = {}  # term as key value, [docID, docfreq] as value
    # global doc map
    docDict = {}

    for filename in glob.glob(path):
        print 'opening: ', filename
        file_stream = codecs.open(filename, 'r', 'utf-8-sig')
        soup = BeautifulSoup(file_stream, 'html.parser')

        docNo_list = []
        text_list = []

        docNo_expression  = re.compile('^<docno> (.*) </docno>$' )
        for docNo in soup.find_all('docno', string=True):
            # APPEND TO LIST
            result = re.findall(docNo_expression, str(docNo)) # remove <doc> tags
            docNo_list.append(result[0])

        text_expression  = re.compile('^<text>\s+(.*)\s+</text>$' )
        for text in soup.find_all('text'):
            # REMOVE COMMENTS
            comments = soup.findAll(text=lambda text:isinstance(text, Comment))
            [comment.extract() for comment in comments]
            # print text
            # result = re.findall(text_expression, str(text)) # remove <doc> tags

            # REMOVE HTML delimiters

            text = re.sub('<[\w][^>]*>', '', str(text))
            text = re.sub('<\/?[\w][^>]*>', '', str(text))

            text = textNorm(text)

            # print text
            # text = unicode(text, errors='ignore')
            text_list.append(text)

        for x in range(len(docNo_list)):
            # - TOKENIZATION -
            stop = set(stopwords.words('english'))
            # docDict[docNo_list[x]] = text_list[x] #\

            tokens = text_list[x].split()

            if remove_stopWords:
                tokens = [i for i in tokens if i not in stop]  # REMOVE STOP WORDS

            docDict[docNo_list[x]] = tokens

    # SO AT THIS POINT docDict contains all doc IDs and their corresponding text fields that are tokenized
            # postingList_Dict = {}  # term as key value, [docID, docfreq] as value


    print 'number of docIDs: ', len(docDict)
    # for each docID
    for docID, text_field in docDict.iteritems():

        alreadyProcessed = {}
        # for each text_token corresponding to the current docID's text field:


        for text_token in text_field:
            if text_token not in alreadyProcessed: # for each unique token in text_field
                alreadyProcessed[text_token] = True

                # iterate through text field, increment tf for that term
                term_position = 0 # POSITIONAL counter
                term_freq_currDoc = 0
                positions = []
                for text_token_iter in text_field:
                    if text_token_iter == text_token:
                        term_freq_currDoc += 1
                        positions.append(term_position)
                    term_position += 1


                # if term not already in postingList_Dict
                if text_token not in postingList_Dict:
                    # create new element in postingList_Dict, using docID and tf
                    postingList = []
                    postingList_term = [docID, term_freq_currDoc, positions]
                    postingList.append(postingList_term)
                    postingList_Dict[text_token] = postingList

                else:
                    # append docID and tf to posting list of current term

                    postingList_term = [docID, term_freq_currDoc, positions]
                    postingList_Dict[text_token].append(postingList_term)

        # print text_token, ' ', postingList_Dict[text_token]


    pl_fName = 'tripleList_Disk_FINAL_POSITIONAL'
    output_dir += pl_fName


    # PRINT TO FILE
    with open(output_dir, 'w') as f:
        for key in sorted(postingList_Dict):

            postingList = postingList_Dict[key]

            if ((len(postingList) >  1) or (  postingList_Dict[key][0][1] > 1  )  ):  # ignore terms that occur in only one document or only occur once

                # print 'here'
                # for each posting list term, write a line to file: term, docID, tf
                for term in postingList:

                    positions_string = str(term[2][0])

                    for position_index in range(len(term[2])):
                        if position_index != 0:
                            positions_string += ', ' + str(term[2][position_index])

                    pl_string = str(key) + ', ' + term[0] + ', ' + str(term[1]) + ', ' + positions_string + '\n'

                    f.write(pl_string)
                # print 'HERE'
                # pdb.set_trace()

    # print 'number of terms: ', len(postingList_Dict)

def makeFinal_STEM(path, remove_stopWords, output_dir):
    stemmer = PorterStemmer()
       # global posting list
    Lexicon = {}

    postingList_Dict = {}  # term as key value, [docID, docfreq] as value
    # global doc map
    docDict = {}

    for filename in glob.glob(path):
        print 'opening: ', filename
        file_stream = codecs.open(filename, 'r', 'utf-8-sig')
        soup = BeautifulSoup(file_stream, 'html.parser')

        docNo_list = []
        text_list = []

        docNo_expression  = re.compile('^<docno> (.*) </docno>$' )
        for docNo in soup.find_all('docno', string=True):
            # APPEND TO LIST
            result = re.findall(docNo_expression, str(docNo)) # remove <doc> tags
            docNo_list.append(result[0])

        text_expression  = re.compile('^<text>\s+(.*)\s+</text>$' )
        for text in soup.find_all('text'):
            # REMOVE COMMENTS
            comments = soup.findAll(text=lambda text:isinstance(text, Comment))
            [comment.extract() for comment in comments]
            # print text
            # result = re.findall(text_expression, str(text)) # remove <doc> tags

            # REMOVE HTML delimiters

            text = re.sub('<[\w][^>]*>', '', str(text))
            text = re.sub('<\/?[\w][^>]*>', '', str(text))

            text = textNorm(text)

            # print text
            # text = unicode(text, errors='ignore')
            text_list.append(text)

        for x in range(len(docNo_list)):
            # - TOKENIZATION -
            stop = set(stopwords.words('english'))
            # docDict[docNo_list[x]] = text_list[x] #\

            tokens = text_list[x].split()



            if remove_stopWords:
                tokens = [i for i in tokens if i not in stop]  # REMOVE STOP WORDS

            tokens = [stemmer.stem(token) for token in tokens]   # STEM

            docDict[docNo_list[x]] = tokens

    # SO AT THIS POINT docDict contains all doc IDs and their corresponding text fields that are tokenized
            # postingList_Dict = {}  # term as key value, [docID, docfreq] as value

    print 'number of docIDs: ', len(docDict)
    # for each docID
    for docID, text_field in docDict.iteritems():

        alreadyProcessed = {}
        # for each text_token corresponding to the current docID's text field:
        for text_token in text_field:
            if text_token not in alreadyProcessed:
                alreadyProcessed[text_token] = True
                # iterate through text field, increment tf for that term
                term_freq_currDoc = 0
                for text_token_iter in text_field:
                    if text_token_iter == text_token:
                        term_freq_currDoc += 1


                # if term not already in postingList_Dict
                if text_token not in postingList_Dict:
                    # create new element in postingList_Dict, using docID and tf
                    postingList = []
                    postingList_term = [docID, term_freq_currDoc]
                    postingList.append(postingList_term)
                    postingList_Dict[text_token] = postingList

                else:
                    # append docID and tf to posting list of current term

                    postingList_term = [docID, term_freq_currDoc]
                    postingList_Dict[text_token].append(postingList_term)

    # PRINT TO FILE

    pl_fName = 'tripleList_Disk_FINAL_STEM'
    output_dir += pl_fName

    with open(output_dir, 'w') as f:
        for key in sorted(postingList_Dict):
            postingList = postingList_Dict[key]

            # print 'here'
            # for each posting list term, write a line to file: term, docID, tf
            if ((len(postingList) >  1) or (  postingList_Dict[key][0][1] > 1  )  ):  # ignore terms that occur in only one document or only occur once

                for term in postingList:
                    pl_string = str(key) + ', ' + term[0] + ', ' + str(term[1]) + '\n'
                    f.write(pl_string)
                    # print 'HERE'
                    # pdb.set_trace()


def makeFinal_PHRASE(path, remove_stopWords, phraseSize, output_dir):

    stemmer = PorterStemmer()
       # global posting list
    Lexicon = {}

    postingList_Dict = {}  # term as key value, [docID, docfreq] as value
    # global doc map
    docDict = {}

    for filename in glob.glob(path):
        print 'opening: ', filename
        file_stream = codecs.open(filename, 'r', 'utf-8-sig')
        soup = BeautifulSoup(file_stream, 'html.parser')

        docNo_list = []
        text_list = []

        docNo_expression  = re.compile('^<docno> (.*) </docno>$' )
        for docNo in soup.find_all('docno', string=True):
            # APPEND TO LIST
            result = re.findall(docNo_expression, str(docNo)) # remove <doc> tags
            docNo_list.append(result[0])

        text_expression  = re.compile('^<text>\s+(.*)\s+</text>$' )
        for text in soup.find_all('text'):
            # REMOVE COMMENTS
            comments = soup.findAll(text=lambda text:isinstance(text, Comment))
            [comment.extract() for comment in comments]
            # print text
            # result = re.findall(text_expression, str(text)) # remove <doc> tags

            # REMOVE HTML delimiters

            text = re.sub('<[\w][^>]*>', '', str(text))
            text = re.sub('<\/?[\w][^>]*>', '', str(text))

            text = textNorm(text)

            # print text
            # text = unicode(text, errors='ignore')
            text_list.append(text)

        for x in range(len(docNo_list)):
            # - TOKENIZATION -
            stop = set(stopwords.words('english'))
            # docDict[docNo_list[x]] = text_list[x] #\

            tokens = text_list[x].split()

            if remove_stopWords:
                tokens = [i for i in tokens if i not in stop]  # REMOVE STOP WORDS


            if phraseSize == 2:
                temp = []
                for i in range(len(tokens)):
                    if i < len(tokens)-1:
                        phrase = tokens[i] + ' ' + tokens[i+1]
                        temp.append(phrase)
                tokens = temp

            if phraseSize == 3:
                temp = []
                for i in range(len(tokens)):
                    if i < len(tokens)-2:
                        phrase = tokens[i] + ' ' + tokens[i+1] + ' ' + tokens[i+2]
                        temp.append(phrase)
                tokens = temp



            docDict[docNo_list[x]] = tokens

    # SO AT THIS POINT docDict contains all doc IDs and their corresponding text fields that are tokenized
            # postingList_Dict = {}  # term as key value, [docID, docfreq] as value

    print 'number of docIDs: ', len(docDict)
    # for each docID
    for docID, text_field in docDict.iteritems():

        alreadyProcessed = {}
        # for each text_token corresponding to the current docID's text field:
        for text_token in text_field:
            if text_token not in alreadyProcessed:
                alreadyProcessed[text_token] = True
                # iterate through text field, increment tf for that term
                term_freq_currDoc = 0
                for text_token_iter in text_field:
                    if text_token_iter == text_token:
                        term_freq_currDoc += 1


                # if term not already in postingList_Dict
                if text_token not in postingList_Dict:
                    # create new element in postingList_Dict, using docID and tf
                    postingList = []
                    postingList_term = [docID, term_freq_currDoc]
                    postingList.append(postingList_term)
                    postingList_Dict[text_token] = postingList

                else:
                    # append docID and tf to posting list of current term

                    postingList_term = [docID, term_freq_currDoc]
                    postingList_Dict[text_token].append(postingList_term)

    # PRINT TO FILE
    tripleList_fName =  'tripleList_Disk_FINAL_PHRASE' + str(phraseSize)

    output_dir += tripleList_fName

    with open(output_dir, 'w') as f:
        for key in sorted(postingList_Dict):

            # if positing list for curr key has size greater than 1 OR postingList_Dict[key][1] > 1

            postingList = postingList_Dict[key]

            if ((len(postingList) >  1) or (  postingList_Dict[key][0][1] > 1  )  ):  # ignore terms that occur in only one document or only occur once

            # print 'here'
            # for each posting list term, write a line to file: term, docID, tf
                for term in postingList:
                    pl_string = str(key) + ', ' + term[0] + ', ' + str(term[1]) + '\n'


                    f.write(pl_string)

def writeDocMap(trec_files_directory_path, stopWords, output_dir):  # THIS SHOULD BE PART OF INDEX CONSTRUCTION

    docMapName = output_dir + 'docMap'
    docNo_list = []
    token_list = []

    fName_docID_text = output_dir + 'docID_text'

    for filename in glob.glob(trec_files_directory_path):

        file_stream = codecs.open(filename, 'r', 'utf-8-sig')
        soup = BeautifulSoup(file_stream, 'html.parser')

        docDict = {}
        docID_text = {}

        stop = set(stopwords.words('english'))

        docNo_expression  = re.compile('^<docno> (.*) </docno>$' )
        for docNo in soup.find_all('docno', string=True):
            # APPEND TO LIST
            result = re.findall(docNo_expression, str(docNo)) # remove <doc> tags
            docNo_list.append(result[0])

        text_expression  = re.compile('^<text>\s+(.*)\s+</text>$' )
        for text in soup.find_all('text'):
            # REMOVE COMMENTS
            comments = soup.findAll(text=lambda text:isinstance(text, Comment))
            [comment.extract() for comment in comments]
            # print text
            # result = re.findall(text_expression, str(text)) # remove <doc> tags

            # REMOVE HTML delimiters

            text = re.sub('<[\w][^>]*>', '', str(text))
            text = re.sub('<\/?[\w][^>]*>', '', str(text))

            text = textNorm(text)

            tokens = text.split()

            if stopWords:
                tokens = [i for i in tokens if i not in stop]  # REMOVE STOP WORDS

            # text = unicode(text, errors='ignore')
            token_list.append(tokens)

        for x in range(len(docNo_list)):
            docDict[docNo_list[x]] = len(token_list[x])
            docID_text[docNo_list[x]] = token_list[x]

        with open(docMapName, 'w') as f:
                for key, value in docDict.iteritems():
                    line = str(key) + ', ' + str(value) + '\n'
                    # print line
                    f.write(line)

        with open(fName_docID_text, 'w') as f:
            for key, value in docID_text.iteritems():
                line = str(key)
                for item in value:
                    line +=  ', ' + str(item)
                line += '\n'
                f.write(line)

        # - END TOKENIZATION -

# ------------------------------------------------------------------------------------------------------------

# UPDATE writeDocMap

if (len(sys.argv) != 4):
    print "ERROR: invalid number of arguments: ", len(sys.argv) - 1
else:

    trec_files_directory_path = sys.argv[1]
    trec_files_directory_path += "fr*"  # for glob
    index_type = sys.argv[2]
    output_dir = sys.argv[3]

    print 'begin index construction, type: ', index_type

    if not os.path.exists(output_dir): # check if dirctory exists, create if it doesn't
        os.makedirs(output_dir)

    remove_stopWords = True

    docMapPath = output_dir + 'docMap'
    if not os.path.exists(docMapPath): # if docMap does not exist, write it
        print 'Creating doc maps - This only needs to be done the first time an index is built'
        writeDocMap(trec_files_directory_path, remove_stopWords, output_dir)
        print 'Created doc map.'

    # based on index type, create index
    if (index_type == "single"):

        makeFinal_SINGLE(trec_files_directory_path, remove_stopWords, output_dir)
        constructLexicon('tripleList_Disk_FINAL_SINGLE', 'tripleList_Disk_LEXICON_SINGLE', output_dir)

    elif (index_type == "stem"):

        makeFinal_STEM(trec_files_directory_path, remove_stopWords, output_dir)
        constructLexicon('tripleList_Disk_FINAL_STEM', 'tripleList_Disk_LEXICON_STEM', output_dir)

    elif (index_type == "phrase"):
        phraseSize = 2
        makeFinal_PHRASE(trec_files_directory_path, remove_stopWords, phraseSize, output_dir)  # phraseSize can be either 2 or 3
        constructLexicon('tripleList_Disk_FINAL_PHRASE2', 'tripleList_Disk_LEXICON_PHRASE2', output_dir)

    elif (index_type == "positional"):
        makeFinal_POSITIONAL(trec_files_directory_path, remove_stopWords, output_dir)
        constructLexicon('tripleList_Disk_FINAL_POSITIONAL', 'tripleList_Disk_LEXICON_POSITIONAL', output_dir)

    else:
        print 'ERROR: invalid index type specified'

# trec-files-directory-path
