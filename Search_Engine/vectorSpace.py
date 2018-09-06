def vectorSpace(query, lexicon_fName, postingList_fName): # pass in list of query tokens that have been normlaized / preprocessed
    # load posting lists into memory

    index = {}
    numDoc = 0
    with open(lexicon_fName) as f:
        fLines  = f.readlines()
        for i in range(len(fLines) - 1) :
            term = [x.strip() for x in fLines[i].split(',')]   # need to convert from string to list
            myList = []
            myList.append(term[1])
            index[term[0]] = myList
        numDoc = int(float(fLines[-1]))  # TOTAL NUMBER DOCUMENTS ?

    with open(postingList_fName) as f:
        fLines  = f.readlines()
        for line in fLines:
            term = [x.strip() for x in line.split(',')]   # need to convert from string to list
            postingList_term = []
            postingList_term.append(term[1])
            postingList_term.append(term[2])
            index[term[0]].append(postingList_term)
            # print index[term[0]]

    # obtain tf --> the 1th element in the posting list term that has the proper doc id as the 0th element
    # df --> 0th element of list that is value corresponding to a term being used as a key
    # idf --> idf i = log10(n / df i)  where n is the number of documents

    dict_numerator = {}
    dict_denom_tf_i = {}
    dict_denom_weight = {}
    docList = []
    old_term = ''
    for term in query:
        if term in index:
            print term, index[term]

            # postingList = [ index[x] for x in range(len(index[term]) - 1) ] # skip the first element, which is df
            postingList = []
            for x in range(len(index[term]) - 1):         # obtain posting list l
                # print index[term]
                postingList.append(index[term][x+1])

            print 'posting list: ', postingList
            # obtain df
            df_term_i = int(float(index[term][0]))

            # obtain idf
            idf_term_i = math.log10(numDoc / df_term_i)

            for entry in postingList: # for each entry in the poisting list
                # calculate weight:
                    # ((log tf ij + 1.0) idf j) / sum ((log tf ij + 1.0) idf j) * ^2  --> for all terms in that document

                print entry[0], entry[1]
                if term != old_term:
                    # import pdb; pdb.set_trace()
                    old_term = term
                if entry[0] not in dict_numerator:
                    print('I set it to zero')
                    dict_numerator[entry[0]] = 0
                    dict_denom_tf_i[entry[0]] = 0
                    dict_denom_weight[entry[0]] = 0
                    docList.append(entry[0])

                tf_ij = int(float(entry[1]))

                # calcualte weight
                weight_numer = (math.log10(tf_ij) + 1.0)  * idf_term_i
                weight_denom = 0
                for entry_again in postingList:
                    weight_denom += math.pow((math.log10(tf_ij + 1.0)  * idf_term_i), 2)
                weight_term_ij = weight_numer / weight_denom

                d_ij =  term[1] # replace term[1]

                # if weight term is always 1,

                dict_numerator[entry[0]] += weight_term_ij * tf_ij
                dict_denom_tf_i[entry[0]] += pow(tf_ij, 2)
                dict_denom_weight[entry[0]] += pow(weight_term_ij, 2)


    Score = {}
    for docNo in docList:  # for each document that had one of the query terms
        # print 'sqrt( ', dict_denom_tf_i[docNo], ' * ', dict_denom_weight[docNo]
        # print 'numerator: ', dict_numerator[docNo]
        denominator = math.sqrt( dict_denom_tf_i[docNo] * dict_denom_weight[docNo] )

        print 'dict_numerator[docNo]: ', dict_numerator[docNo], ' ', 'denominator: ', denominator

        Score[docNo] = dict_numerator[docNo] / denominator
        print 'Score = ', dict_numerator[docNo], ' / ', denominator

    return Score
