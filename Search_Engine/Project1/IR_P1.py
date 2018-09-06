
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

# for , only store terms that occur more than x times

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

# --- FUNC DEF ---
# does: creates triples for each dictionary element, writes them to temp files, the size of which are restricted by a memory constraint
# returns: count of temp disk Files created
def convertToTriples_SINGLE(docDict, count_diskFiles):
    # # --- populate termDict ---
    termList_total = [] # Lexicon?
    triplesList_total = []
    tripleList_buffer = []

    countTerms = 0

    for docNo, docText in docDict.iteritems():

        if docNo not in docIDs_dict:
            docIDs_dict[docNo] = True
        # --- Build triple list for document ---
        tripleList_doc = []
        # for term in termDict_doc:
        for term_Text in docText:
            if term_Text not in terms_dict:
                terms_dict[term_Text] = True


            incremented_tF = False
            for triple in tripleList_doc:
                if term_Text == triple[0]:
                    triple[2] += 1
                    incremented_tF = True
                    # print triple[2]

            if not incremented_tF: # then need to make triple, then append to list
                triple = []
                triple.append(term_Text)
                triple.append(docNo)
                triple.append(1)
                tripleList_doc.append(triple)

            # ---- add to tripleList_buffer that is constrained at a given parameter

        if (len(tripleList_buffer) < memory_constraint):

            # BEGIN merge tripleList_doc with tripleList_buffer
            match = False
            for triple_doc in tripleList_doc:
                for triple_buffer in tripleList_buffer:
                    if triple_doc[0] == triple_buffer[0]:
                        # need to incrment pre-existing item
                        triple_buffer[2] += 1
                        match = True

                if not match: # append to tripleList_buffer
                    tripleList_buffer.append(triple_doc)
                # END merge tripleList_doc with tripleList_buffer

        else:  # flush tripleList_buffer to hard drive (where tripleList_Disk_# will be )
            tripleList_buffer = sorted(tripleList_buffer, key = lambda x: (x[0], x[1])) # sort on term and docID
            flushFile_name = 'tripleList_Disk_SINGLE_' + str(count_diskFiles)
            # print 'flushed buffer to ', flushFile_name
            with open(flushFile_name, 'w') as f:
                for triple_buffer in tripleList_buffer:
                    triple_buffer_string = str(triple_buffer[0]) + ', ' + str(triple_buffer[1]) + ', ' + str(triple_buffer[2]) + '\n'
                    f.write(triple_buffer_string)

            count_diskFiles += 1

            tripleList_buffer = [] # clear buffer
            # BEGIN merge tripleList_doc with tripleList_buffer
            match = False
            for triple_doc in tripleList_doc:
                for triple_buffer in tripleList_buffer:
                    if triple_doc[0] == triple_buffer[0]: # then need to incrment pre-existing item

                        triple_buffer[2] += 1
                        match = True
                if not match: # else, append to tripleList_buffer
                    tripleList_buffer.append(triple_doc)


    # flush remaining triples in tripleList_buffer to disk
    if tripleList_buffer:
        tripleList_buffer = sorted(tripleList_buffer, key = lambda x: (x[0], x[1])) # sort on term and docID
        flushFile_name = 'tripleList_Disk_SINGLE_' + str(count_diskFiles)
        with open(flushFile_name, 'w') as f:
            for triple_buffer in tripleList_buffer:
                    triple_buffer_string = str(triple_buffer[0]) + ', ' + str(triple_buffer[1]) + ', ' + str(triple_buffer[2]) + '\n'
                    f.write(triple_buffer_string)
        count_diskFiles += 1
        tripleList_buffer = [] # clear buffer
    # END PHASE 1 & 2
    return count_diskFiles

def merge_SINGLE(count_diskFiles):
    list_minTriples = []
    file_num = 0
    fileNum_currMin = 0
    final_file = open('tripleList_Disk_FINAL_SINGLE', 'w+')

    while file_num < count_diskFiles: # load list_minTriples for first time
        currFile_name = 'tripleList_Disk_SINGLE_' + str(file_num)
        currFile = open(currFile_name, 'r')
        currTriple_string = currFile.readline()
        currTriple = [x.strip() for x in currTriple_string.split(',')]   # need to convert from string to list
        currTriple.append(file_num) # temporarly append a way to keep count of which disk file the triple is from
        if len(list_minTriples) < memory_constraint:
            list_minTriples.append(currTriple) # add first element of each list to min heap (perhaps needs to be list that I sort twice each time?)
        else:
            print "WARNING: list_minTriples in Merge process has surpassed memory constraint"

        file_num += 1
        currFile.close()

    # create list of line indexes keep track of current file lines for each file
    fileLineIndices = [1] * file_num

    while list_minTriples:     # repeat process until list_minTriples is empty

        # pdb.set_trace()
        list_minTriples = sorted(list_minTriples, key = lambda x: (x[0], x[1])) # sort on term and docID
        fileNum_currMin = list_minTriples[0][3]
        del list_minTriples[0][3]

    # take min triple, which is the first one in list, and add to final output file
        triple_string = str(list_minTriples[0][0]) + ', ' + str(list_minTriples[0][1]) + ', ' + str(list_minTriples[0][2]) + '\n'

        final_file.write(triple_string)

        # then pop off of heap
        del list_minTriples[0]

        # load new triple into min heap, from the file that the triple just popped off the heap was from
        new_triple = []
        currFile_name = 'tripleList_Disk_SINGLE_' + str(fileNum_currMin)
        currFile = open(currFile_name, 'r')   # POINTER IS PLACED AT BEGINING OF FILE   < ----- THIS WONT WORK

        num_lines = sum(1 for line in currFile)
        if fileLineIndices[fileNum_currMin] < num_lines:
        # if file from which the triple just added to the final list came still has un-incorperated triples, load next line from file just writen from into list_minTriples, and increment the line index for that file
            currFile.seek(0,0) # set file pointer at begining of file

            currTriple_string = currFile.readlines()[fileLineIndices[fileNum_currMin]]   # take triple string from proper line in proper file
            fileLineIndices[fileNum_currMin] += 1         # increment line index for corresponding file
            currTriple = [x.strip() for x in currTriple_string.split(',')]   # need to convert from string to list
            currTriple.append(fileNum_currMin) # temporarly append a way to keep count of which disk file the triple is from
            list_minTriples.append(currTriple)

        currFile.close()
    final_file.close()

def convertToTriples_POSITIONAL(docDict, count_diskFiles): # --- populate termDict ---
    termList_total = [] # Lexicon?
    triplesList_total = []
    tripleList_buffer = []

    for docNo, docText in docDict.iteritems():

        # --- Build triple list for document ---
        tripleList_doc = []
        # for term in termDict_doc:
        term_position = 0
        for term_Text in docText:
            incremented_tF = False
            for triple in tripleList_doc: # check if already added
                if term_Text == triple[0]:
                    triple[2] += 1
                    triple[3].append(term_position) # append location to position list for term
                    incremented_tF = True

            if not incremented_tF: # then need to make triple, then append to list
                triple = []
                triple.append(term_Text.encode('ascii', 'ignore'))
                triple.append(docNo)
                triple.append(1)
                positions = []
                positions.append(term_position)
                triple.append(positions)
                tripleList_doc.append(triple)
            term_position += 1

            # ---- add to tripleList_buffer that is constrained at a given parameter
        if (len(tripleList_buffer) < memory_constraint):
            # add tripleList_doc to buffer

            # BEGIN merge tripleList_doc with tripleList_buffer
            match = False
            for triple_doc in tripleList_doc:
                for triple_buffer in tripleList_buffer:
                    if triple_doc[0] == triple_buffer[0]:
                        # need to incrment pre-existing item
                        triple_buffer[2] += 1
                        for position in triple_doc[3]:
                            triple_buffer[3].append(position) # append location to position list for term
                        match = True
                if not match: # append to tripleList_buffer
                    tripleList_buffer.append(triple_doc)
                # END merge tripleList_doc with tripleList_buffer

        else:  # flush tripleList_buffer to hard drive (where tripleList_Disk_# will be )
            tripleList_buffer = sorted(tripleList_buffer, key = lambda x: (x[0], x[1])) # sort on term and docID
            flushFile_name = 'tripleList_Disk_POSITIONAL_' + str(count_diskFiles)
            with open(flushFile_name, 'w') as f:
                for triple_buffer in tripleList_buffer:
                    triple_buffer_string = str(triple_buffer[0].encode('ascii', 'ignore')) + ', ' + str(triple_buffer[1]) + ', ' + str(triple_buffer[2])
                    iterPositions = iter(triple_buffer[3])
                    positions_string = triple_buffer[3][0]
                    next(iterPositions)
                    for position in iterPositions:
                            positions_string += ', ' + str(position)
                    triple_buffer_string +=  ', ' + positions_string + '\n'
                    f.write(triple_buffer_string)
            count_diskFiles += 1

            tripleList_buffer = [] # clear buffer
            # now that buffer flushed, BEGIN merge tripleList_doc with tripleList_buffer
            match = False
            for triple_doc in tripleList_doc:
                for triple_buffer in tripleList_buffer:
                    if triple_doc[0] == triple_buffer[0]: # then need to incrment pre-existing item

                        triple_buffer[2] += 1
                        for position in triple_doc[3]:
                            triple_buffer[3].append(position) # append location to position list for term
                        match = True
                if not match: # else, append to tripleList_buffer
                    tripleList_buffer.append(triple_doc)

    # flush remaining triples in tripleList_buffer to disk
    if tripleList_buffer:
        tripleList_buffer = sorted(tripleList_buffer, key = lambda x: (x[0], x[1])) # sort on term and docID
        flushFile_name = 'tripleList_Disk_POSITIONAL_' + str(count_diskFiles)
        with open(flushFile_name, 'w') as f:
            for triple_buffer in tripleList_buffer:
                    triple_buffer_string = triple_buffer[0] + ', ' + triple_buffer[1] + ', ' + str(triple_buffer[2])
                    iterPositions = iter(triple_buffer[3])
                    positions_string = triple_buffer[3][0]
                    next(iterPositions)
                    for position in iterPositions:
                            positions_string = str(positions_string) + ', ' + str(position)
                    triple_buffer_string += ', ' + str(positions_string) + '\n'

                    f.write(triple_buffer_string)
        count_diskFiles += 1
        tripleList_buffer = [] # clear buffer
    # END PHASE 1 & 2
    return count_diskFiles

def merge_POSITIONAL(count_diskFiles):
    list_minTriples = []
    file_num = 0
    fileNum_currMin = 0
    final_file = open('tripleList_Disk_FINAL_POSITIONAL', 'w+')

    while file_num < count_diskFiles: # load list_minTriples for first time
        currFile_name = 'tripleList_Disk_POSITIONAL_' + str(file_num)
        currFile = open(currFile_name, 'r')
        currTriple_string = currFile.readline()
        currTriple_temp = [x.strip() for x in currTriple_string.split(',')]   # need to convert from string to list
        currTriple = []
        currTriple.append(currTriple_temp[0])
        currTriple.append(currTriple_temp[1])
        currTriple.append(currTriple_temp[2])
        # need to convert positions into list, and put list on
        position_list = []
        index = 3 # start iindex at 3, so as to start at positions
        while index < (len(currTriple_temp)):
            position_list.append(currTriple_temp[index])
            index += 1
        currTriple.append(position_list)
        currTriple.append(file_num) # temporarly append a way to keep count of which disk file the triple is from


        if len(list_minTriples) < memory_constraint:
            list_minTriples.append(currTriple) # add first element of each list to min heap (perhaps needs to be list that I sort twice each time?)
        else:
            print "WARNING: list_minTriples in Merge process has surpassed memory constraint"
        file_num += 1

        currFile.close()
    # create list of line indexes keep track of current file lines for each file
    fileLineIndices = [1] * file_num

    while list_minTriples:     # repeat process until list_minTriples is empty

        list_minTriples = sorted(list_minTriples, key = lambda x: (x[0], x[1])) # sort on term and docID
        fileNum_currMin = list_minTriples[0][-1]
        del list_minTriples[0][-1]

    # take min triple, which is the first one in list, and add to final output file
        triple_string = str(list_minTriples[0][0]) + ', ' + str(list_minTriples[0][1]) + ', ' + str(list_minTriples[0][2])
        iterPositions = iter(list_minTriples[0])   # cant do this, because I;m merely iterating over an index of the position list, not the list itself, because the positions aren't actually in a list at this point
        positions_string = str(list_minTriples[0][3]) # get first position  THIS ISNT RIGHT
        next(iterPositions) # increment pointer to 1
        next(iterPositions) # increment pointer to 2
        next(iterPositions) # increment pointer to 3
        next(iterPositions) # increment pointer to 4
        for position in iterPositions:
                positions_string += ', ' + str(position)
        triple_string += ', ' + positions_string + '\n'
        final_file.write(triple_string)

        # then pop off of heap
        del list_minTriples[0]

        # load new triple into min heap, from the file that the triple just popped off the heap was from
        new_triple = []
        currFile_name = 'tripleList_Disk_POSITIONAL_' + str(fileNum_currMin)
        currFile = open(currFile_name, 'r')   # POINTER IS PLACED AT BEGINING OF FILE   < ----- THIS WONT WORK

        num_lines = sum(1 for line in currFile)
        if fileLineIndices[fileNum_currMin] < num_lines:
        # if file from which the triple just added to the final list came still has un-incorperated triples, load next line from file just writen from into list_minTriples, and increment the line index for that file
            currFile.seek(0,0) # set file pointer at begining of file

            currTriple_string = currFile.readlines()[fileLineIndices[fileNum_currMin]]   # take triple string from proper line in proper file
            fileLineIndices[fileNum_currMin] += 1         # increment line index for corresponding file
            currTriple = [x.strip() for x in currTriple_string.split(',')]   # need to convert from string to list
            currTriple.append(fileNum_currMin) # temporarly append a way to keep count of which disk file the triple is from
            list_minTriples.append(currTriple)

        currFile.close()
    final_file.close()

from nltk.stem import *

def convertToTriples_STEM(docDict, count_diskFiles):
    stemmer = PorterStemmer()
    # # --- populate termDict ---
    termList_total = [] # Lexicon?
    triplesList_total = []
    tripleList_buffer = []

    for docNo, docText in docDict.iteritems():
        docText = [stemmer.stem(docItem).encode('ascii', 'ignore') for docItem in docText]
        # run through the whole document, compile list of all terms in that document
        termDict_doc = {}
        for term in docText:
            if term not in termDict_doc:
                termDict_doc[term] = True

        # --- Build triple list for document ---
        tripleList_doc = []
        # for term in termDict_doc:
        for term_Text in docText:
            incremented_tF = False
            for triple in tripleList_doc:
                if term_Text == triple[0]:
                    triple[2] += 1
                    incremented_tF = True

            if not incremented_tF: # then need to make triple, then append to list
                triple = []
                triple.append(term_Text)
                triple.append(docNo)
                triple.append(1)
                tripleList_doc.append(triple)

            # ---- add to tripleList_buffer that is constrained at a given parameter
        # difference = memory_constraint - len(tripleList_doc)    """ and (difference >= 0) """
        if (len(tripleList_buffer) < memory_constraint):
            # add tripleList_doc to buffer

            # BEGIN merge tripleList_doc with tripleList_buffer
            match = False
            for triple_doc in tripleList_doc:
                for triple_buffer in tripleList_buffer:
                    if triple_doc[0] == triple_buffer[0]:
                        # need to incrment pre-existing item
                        triple_buffer[2] += 1
                        match = True

                if not match: # append to tripleList_buffer
                    tripleList_buffer.append(triple_doc)
                # END merge tripleList_doc with tripleList_buffer

        else:  # flush tripleList_buffer to hard drive (where tripleList_Disk_# will be )
            tripleList_buffer = sorted(tripleList_buffer, key = lambda x: (x[0], x[1])) # sort on term and docID
            flushFile_name = 'tripleList_Disk_STEM_' + str(count_diskFiles)
            with open(flushFile_name, 'w') as f:
                for triple_buffer in tripleList_buffer:
                    triple_buffer_string = str(triple_buffer[0]) + ', ' + str(triple_buffer[1]) + ', ' + str(triple_buffer[2]) + '\n'
                    f.write(triple_buffer_string)
            count_diskFiles += 1

            tripleList_buffer = [] # clear buffer
            # BEGIN merge tripleList_doc with tripleList_buffer
            match = False
            for triple_doc in tripleList_doc:
                for triple_buffer in tripleList_buffer:
                    if triple_doc[0] == triple_buffer[0]: # then need to incrment pre-existing item
                        triple_buffer[2] += 1
                        match = True
                if not match: # else, append to tripleList_buffer
                    tripleList_buffer.append(triple_doc)


    # flush remaining triples in tripleList_buffer to disk
    if tripleList_buffer:
        tripleList_buffer = sorted(tripleList_buffer, key = lambda x: (x[0], x[1])) # sort on term and docID
        flushFile_name = 'tripleList_Disk_STEM_' + str(count_diskFiles)
        with open(flushFile_name, 'w') as f:
            for triple_buffer in tripleList_buffer:
                    triple_buffer_string = str(triple_buffer[0]) + ', ' + str(triple_buffer[1]) + ', ' + str(triple_buffer[2]) + '\n'
                    f.write(triple_buffer_string)
        count_diskFiles += 1
        tripleList_buffer = [] # clear buffer
    # END PHASE 1 & 2
    return count_diskFiles


def merge_STEM(count_diskFiles):

    list_minTriples = []
    file_num = 0
    fileNum_currMin = 0
    final_file = open('tripleList_Disk_FINAL_STEM', 'w+')

    while file_num < count_diskFiles: # load list_minTriples for first time
        currFile_name = 'tripleList_Disk_STEM_' + str(file_num)
        currFile = open(currFile_name, 'r')
        currTriple_string = currFile.readline()
        currTriple = [x.strip() for x in currTriple_string.split(',')]   # need to convert from string to list
        currTriple.append(file_num) # temporarly append a way to keep count of which disk file the triple is from
        if len(list_minTriples) < memory_constraint:
            list_minTriples.append(currTriple) # add first element of each list to min heap (perhaps needs to be list that I sort twice each time?)
        else:
            print "WARNING: list_minTriples in Merge process has surpassed memory constraint"
        file_num += 1

        currFile.close()

    # create list of line indexes keep track of current file lines for each file
    fileLineIndices = [1] * file_num

    while list_minTriples:     # repeat process until list_minTriples is empty

        list_minTriples = sorted(list_minTriples, key = lambda x: (x[0], x[1])) # sort on term and docID
        fileNum_currMin = list_minTriples[0][3]
        del list_minTriples[0][3]

    # take min triple, which is the first one in list, and add to final output file
        triple_string = str(list_minTriples[0][0]) + ', ' + str(list_minTriples[0][1]) + ', ' + str(list_minTriples[0][2]) + '\n'
        final_file.write(triple_string)

        # then pop off of heap
        del list_minTriples[0]

        # load new triple into min heap, from the file that the triple just popped off the heap was from
        new_triple = []
        currFile_name = 'tripleList_Disk_STEM_' + str(fileNum_currMin)
        currFile = open(currFile_name, 'r')   # POINTER IS PLACED AT BEGINING OF FILE   < ----- THIS WONT WORK

        num_lines = sum(1 for line in currFile)
        if fileLineIndices[fileNum_currMin] < num_lines:
        # if file from which the triple just added to the final list came still has un-incorperated triples, load next line from file just writen from into list_minTriples, and increment the line index for that file
            currFile.seek(0,0) # set file pointer at begining of file

            currTriple_string = currFile.readlines()[fileLineIndices[fileNum_currMin]]   # take triple string from proper line in proper file
            fileLineIndices[fileNum_currMin] += 1         # increment line index for corresponding file
            currTriple = [x.strip() for x in currTriple_string.split(',')]   # need to convert from string to list
            currTriple.append(fileNum_currMin) # temporarly append a way to keep count of which disk file the triple is from
            list_minTriples.append(currTriple)

        currFile.close()
    final_file.close()

def convertToTriples_PHRASE(docDict, count_diskFiles, phraseSize):
    # # --- populate termDict ---
    termList_total = [] # Lexicon?
    triplesList_total = []
    tripleList_buffer = []

    for docNo, docText in docDict.iteritems():


        if phraseSize == 2:
            temp = []
            for i in range(len(docText)):
                if i < len(docText)-1:
                    phrase = docText[i] + ' ' + docText[i+1]
                    temp.append(phrase)

            docText = temp

        if phraseSize == 3:
            temp = []
            for i in range(len(docText)):
                if i < len(docText)-2:
                    phrase = docText[i] + ' ' + docText[i+1] + ' ' + docText[i+2]
                    temp.append(phrase)

            docText = temp

        # --- Build triple list for document ---
        tripleList_doc = []
        # for term in termDict_doc:
        for term_Text in docText:
            incremented_tF = False
            for triple in tripleList_doc:
                if term_Text == triple[0]:
                    triple[2] += 1
                    incremented_tF = True

            if not incremented_tF: # then need to make triple, then append to list
                triple = []
                triple.append(term_Text)
                triple.append(docNo)
                triple.append(1)
                tripleList_doc.append(triple)

            # ---- add to tripleList_buffer that is constrained at a given parameter
        if (len(tripleList_buffer) < memory_constraint):
            # add tripleList_doc to buffer

            # BEGIN merge tripleList_doc with tripleList_buffer
            match = False
            for triple_doc in tripleList_doc:
                for triple_buffer in tripleList_buffer:
                    if triple_doc[0] == triple_buffer[0]:
                        # need to incrment pre-existing item
                        triple_buffer[2] += 1
                        match = True

                if not match: # append to tripleList_buffer
                    tripleList_buffer.append(triple_doc)
                # END merge tripleList_doc with tripleList_buffer

        else:  # flush tripleList_buffer to hard drive (where tripleList_Disk_# will be )
            tripleList_buffer = sorted(tripleList_buffer, key = lambda x: (x[0], x[1])) # sort on term and docID
            flushFile_name = 'tripleList_Disk_PHRASE_' + str(count_diskFiles)
            with open(flushFile_name, 'w') as f:
                for triple_buffer in tripleList_buffer:
                    triple_buffer_string = str(triple_buffer[0]) + ', ' + str(triple_buffer[1]) + ', ' + str(triple_buffer[2]) + '\n'
                    f.write(triple_buffer_string)
            count_diskFiles += 1

            tripleList_buffer = [] # clear buffer
            # BEGIN merge tripleList_doc with tripleList_buffer
            match = False
            for triple_doc in tripleList_doc:
                for triple_buffer in tripleList_buffer:
                    if triple_doc[0] == triple_buffer[0]: # then need to incrment pre-existing item

                        triple_buffer[2] += 1
                        match = True
                if not match: # else, append to tripleList_buffer
                    tripleList_buffer.append(triple_doc)

    # flush remaining triples in tripleList_buffer to disk
    if tripleList_buffer:
        tripleList_buffer = sorted(tripleList_buffer, key = lambda x: (x[0], x[1])) # sort on term and docID
        flushFile_name = 'tripleList_Disk_PHRASE_' + str(count_diskFiles)
        with open(flushFile_name, 'w') as f:
            for triple_buffer in tripleList_buffer:
                    triple_buffer_string = str(triple_buffer[0]) + ', ' + str(triple_buffer[1]) + ', ' + str(triple_buffer[2]) + '\n'
                    f.write(triple_buffer_string)
        count_diskFiles += 1
        tripleList_buffer = [] # clear buffer
    # END PHASE 1 & 2
    return count_diskFiles

def merge_PHRASE(count_diskFiles):

    list_minTriples = []
    file_num = 0
    fileNum_currMin = 0
    final_file = open('tripleList_Disk_FINAL_PHRASE', 'w+')

    while file_num < count_diskFiles: # load list_minTriples for first time
        currFile_name = 'tripleList_Disk_PHRASE_' + str(file_num)
        currFile = open(currFile_name, 'r')
        currTriple_string = currFile.readline()
        currTriple = [x.strip() for x in currTriple_string.split(',')]   # need to convert from string to list
        currTriple.append(file_num) # temporarly append a way to keep count of which disk file the triple is from
        if len(list_minTriples) < memory_constraint:
            list_minTriples.append(currTriple) # add first element of each list to min heap (perhaps needs to be list that I sort twice each time?)
        else:
            print "WARNING: list_minTriples in Merge process has surpassed memory constraint"
        file_num += 1

        currFile.close()

    # create list of line indexes keep track of current file lines for each file
    fileLineIndices = [1] * file_num

    while list_minTriples:     # repeat process until list_minTriples is empty

        list_minTriples = sorted(list_minTriples, key = lambda x: (x[0], x[1])) # sort on term and docID
        fileNum_currMin = list_minTriples[0][3]
        del list_minTriples[0][3]

    # take min triple, which is the first one in list, and add to final output file
        triple_string = str(list_minTriples[0][0]) + ', ' + str(list_minTriples[0][1]) + ', ' + str(list_minTriples[0][2]) + '\n'
        final_file.write(triple_string)

        # then pop off of heap
        del list_minTriples[0]

        # load new triple into min heap, from the file that the triple just popped off the heap was from
        new_triple = []
        currFile_name = 'tripleList_Disk_PHRASE_' + str(fileNum_currMin)
        currFile = open(currFile_name, 'r')   # POINTER IS PLACED AT BEGINING OF FILE   < ----- THIS WONT WORK

        num_lines = sum(1 for line in currFile)
        if fileLineIndices[fileNum_currMin] < num_lines:
        # if file from which the triple just added to the final list came still has un-incorperated triples, load next line from file just writen from into list_minTriples, and increment the line index for that file
            currFile.seek(0,0) # set file pointer at begining of file

            currTriple_string = currFile.readlines()[fileLineIndices[fileNum_currMin]]   # take triple string from proper line in proper file
            fileLineIndices[fileNum_currMin] += 1         # increment line index for corresponding file
            currTriple = [x.strip() for x in currTriple_string.split(',')]   # need to convert from string to list
            currTriple.append(fileNum_currMin) # temporarly append a way to keep count of which disk file the triple is from
            list_minTriples.append(currTriple)

        currFile.close()

def constructLexicon(indexName, lexName, output_dir):

    Lexicon = {}
    Documents = {} # to keep track of # documents

    lexName = output_dir + lexName
    indexName = output_dir + lexName

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

    # print 'number of terms: ', len(postingList_Dict)

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

def makeFinal_STEM(path, remove_stopWords, outputPath):
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

            tokens = [stemmer.stem(token) for token in tokens]   # STEM

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


def makeFinal_PHRASE(path, remove_stopWords, phraseSize, outputPath):

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
                # print 'HERE'
                # pdb.set_trace()


#    ---------    MAIN    --------------

memory_constraint = 1000          # SET MEMORY CONSTRAINT HERE

path = "/Users/Konrad/Desktop/Mini-Trec-Data(1)/BigSample/fr*"   # SET FILE PATH HERE (GLOB)

Single = False

Positional = False

Stem = False

Phrase = False

remove_stopWords = True

# pdb.set_trace()
# makeFinal_SINGLE(path, remove_stopWords)
# constructLexicon('tripleList_Disk_FINAL_SINGLE', 'tripleList_Disk_LEXICON_SINGLE')
# print 'finished with single'

# makeFinal_POSITIONAL(path, remove_stopWords)
# constructLexicon('tripleList_Disk_FINAL_POSITIONAL', 'tripleList_Disk_LEXICON_POSITIONAL')
# print 'finished with positional'

# makeFinal_STEM(path, remove_stopWords)
# constructLexicon('tripleList_Disk_FINAL_STEM', 'tripleList_Disk_LEXICON_STEM')
# print 'finished with stem'

# phraseSize = 2 # 2 or 3
# makeFinal_PHRASE(path, remove_stopWords, phraseSize)  # phraseSize can be either 2 or 3
# constructLexicon('tripleList_Disk_FINAL_PHRASE2', 'tripleList_Disk_LEXICON_PHRASE2')
# print 'finished with phrase'

# phraseSize = 3 # 2 or 3
# makeFinal_PHRASE(path, remove_stopWords, phraseSize)  # phraseSize can be either 2 or 3
# constructLexicon('tripleList_Disk_FINAL_PHRASE3', 'tripleList_Disk_LEXICON_PHRASE3')
# print 'finished with phrase'

# Single Term - No Stop Words
if Single:

    print '---- Single ----'
    count_diskFiles = 0
    remove_stopWords = False
    time3 = time.clock()
    time1 = time.clock()
    for filename in glob.glob(path):
        print 'count_diskFiles: ', count_diskFiles
        docDict = preProcessing(filename, memory_constraint, remove_stopWords)
        count_diskFiles = convertToTriples_SINGLE(docDict, count_diskFiles)

    time2 = time.clock()
    time1End = time.clock()
    merge_SINGLE(count_diskFiles)
    time2End = time.clock()
    time3End = time.clock()
    print 'time1: ', (time1End - time1)
    print 'time2: ', (time2End - time2)
    constructLexicon('tripleList_Disk_FINAL_SINGLE', 'tripleList_Disk_LEXICON_SINGLE')

    print 'time3: ', (time3End - time3)
    print 'size docIDs_dict: ', len(docIDs_dict), ' (incremented in convertToTriples_SINGLE)' # ok, so the problem with # docs has to be with lexicon
    print 'terms_dict: ', len(terms_dict), ' (incremented in convertToTriples_SINGLE)'

# Positional (single term) - with stop words
if Positional:
    print '---- Positional ----'
    time1 = time.clock()
    time3 = time.clock()
    count_diskFiles = 0
    remove_stopWords = False
    for filename in glob.glob(path):
        # time2 start = time.clock()
        docDict = preProcessing(filename, memory_constraint, remove_stopWords)
        count_diskFiles = convertToTriples_POSITIONAL(docDict, count_diskFiles)
        # end time2 end = time.clock
    # time3
    time2 = time.clock()
    time1End = time.clock()
    merge_POSITIONAL(count_diskFiles)
    time2End = time.clock()
    time3End = time.clock()
    print 'time1: ', (time1End - time1)
    print 'time2: ', (time2End - time2)
    constructLexicon('tripleList_Disk_FINAL_POSITIONAL', 'tripleList_Disk_LEXICON_POSITIONAL')
    print 'time3: ', (time3End - time3)

if Stem:
    print '---- Stem ----'
    time1 = time.clock()
    time3 = time.clock()
    count_diskFiles = 0
    remove_stopWords = True
    for filename in glob.glob(path):
        docDict = preProcessing(filename, memory_constraint, remove_stopWords)
        count_diskFiles = convertToTriples_STEM(docDict, count_diskFiles)
    time1End = time.clock()
    time2 = time.clock()
    merge_STEM(count_diskFiles)
    time2End = time.clock()
    time3End = time.clock()
    print 'time1: ', (time1End - time1)
    print 'time2: ', (time2End - time2)
    constructLexicon('tripleList_Disk_FINAL_STEM', 'tripleList_Disk_LEXICON_STEM')
    print 'time3: ', (time3End - time3)

if Phrase:
    print '---- Phrase ----'
    time1 = time.clock()
    time3 = time.clock()
    count_diskFiles = 0
    count = 0
    remove_stopWords = True
    for filename in glob.glob(path):
        docDict = preProcessing(filename, memory_constraint, remove_stopWords)
        count_diskFiles = convertToTriples_PHRASE(docDict, count_diskFiles, phraseSize)
    time1End = time.clock()
    time2 = time.clock()
    merge_PHRASE(count_diskFiles)
    time2End = time.clock()
    time3End = time.clock()
    print 'time1: ', (time1End - time1)
    print 'time2: ', (time2End - time2)
    constructLexicon('tripleList_Disk_FINAL_PHRASE', 'tripleList_Disk_LEXICON_PHRASE')
    print 'time3: ', (time3End - time3)





