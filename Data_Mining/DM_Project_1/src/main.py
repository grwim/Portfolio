from __future__ import print_function, division

from collections import OrderedDict
import os

__author__ = 'Konrad'

import collections
import random

import math


def check(x):
    return {
        '@': 0,
        "relation": 1,
        "attribute": 2,
        "data": 3,
        "numeric": 5,
        '}': 7,
        '{': 8,
    }.get(x, 9)


def load(filename, examples, attributes):
    f = open(filename, 'r')
    # finalList = list()

    for line in f:
        lineList = line.split()
        # print(lineList)
        if not lineList:
            print()
        elif check(lineList[0][0]) == 0:  # then @
            if (check(lineList[0][1::]) == 1):  # relation

                print()
            elif (check(lineList[0][1::]) == 2):  # attribute

                if (check(lineList[2]) == 5):  # numeric attribute

                    attributes.append('num')

                else:
                    attributes.append('nom')

            elif (check(lineList[0][1::]) == 3):  # data
                # print("go to data")
                # print(lineList)
                for line in f:
                    singleExample = line.split()
                    # [x.strip() for x in line.split(',')]
                    singleExample = ' '.join(singleExample).replace(',', '').split()  # remove ','
                    examples.append(singleExample)

    f.close()

def replaceMissingValues(examples, attributes):
    c = collections.Counter()

    # replaceMissingValues( list )
    for m in range(len(attributes)):
        if attributes[m] == 'nom':  # NOMINAL
            coll = collections.Counter([x[m] for x in examples])

            freqList = [x[0] for x in coll.most_common()]

            mostFreq = freqList[0]

            for n in range(len(examples)):
                if examples[n][m] == '?':
                    examples[n][m] = mostFreq


        elif attributes[m] == 'num':  # NUMERIC
            needToFill = False
            count = 0
            sum = 0
            for n in range(len(examples)):

                if examples[n][m] == '?':
                    needToFill = True
                else:
                    sum += int(examples[n][m])
                    count += 1

            if needToFill == True:

                average = sum / count

                for n in range(len(examples)):
                    if examples[n][m] == '?':
                        examples[n][m] = int(average)


# NEED TO REDO USING ENTROPY CALCULATIONS
def discretize_2(index, examples,
                 attributes):  # implements a 2-bucket discretezation based on averages that corresponding to class label values

    # index  12 is hours-per-week
    sumGreater = 0
    sumLesser = 0
    countGreater = 0
    countLesser = 0

    for n in range(len(examples)):

        if examples[n][len(attributes) - 1] == '>50K':  # COMPUTE AVERAGES HERE

            sumGreater += int(examples[n][index])
            countGreater += 1

        if examples[n][len(attributes) - 1] == '<=50K':
            sumLesser += int(examples[n][index])
            countLesser += 1
    avgGreater = sumGreater / countGreater
    avgLesser = sumLesser / countLesser

    if avgGreater - avgLesser < 3:
        print("POOR DISCRETIATION DETECTED")

    determinator = int((avgGreater + avgLesser) / 2)

    for n in range(len(examples)):
        if int(examples[n][index]) > determinator:

            examples[n][index] = 'high'

        elif int(examples[n][index]) < determinator:

            examples[n][index] = 'low'

    attributes[index] = 'nom'


def splitByValue(list, splitValue):
    listLess = []
    listMore = []
    # print(splitValue)
    for n in range(len(list)):
        # print(list[n][0])
        if int(list[n][0]) < splitValue:
            listLess.append(list[n])
        elif int(list[n][0]) > splitValue:
            listMore.append(list[n])

    return listLess, listMore


def entropy_discretize(dataList, index):
    pairList = []
    pair = []

    size = len(dataList)

    for i in range(len(dataList)):  # create list with feature value and corresponding class value
        pair.append(dataList[i][index])
        pair.append(dataList[i][-1])
        pairList.append(pair)
        pair = []

    for i in range(len(pairList)):  # sort list
        for j in range(len(pairList) - 1 - i):
            if pairList[j][0] > pairList[j + 1][0]:
                pairList[j], pairList[j + 1] = pairList[j + 1], pairList[j]

    coll = collections.Counter(pairList[i][1] for i in range(len(pairList)))
    freqList = [x for x in coll.most_common()]

    if len(freqList) < 2:
        baseEntropy = -(freqList[0][1] / size) * math.log(freqList[0][1] / size, 2)
    else:
        baseEntropy = -(
        (freqList[0][1] / size) * math.log(freqList[0][1] / size, 2) + (freqList[1][1] / size) * math.log(
            freqList[1][1] / size, 2))

    maxEntropyGain = 0
    splitValueMaxGain = 0
    entropyLesser = .5  # should this be here???
    entropyGreater = .5  # should this be here???

    oldSplitValue = 0

    for i in (range(len(pairList) - 1)):
        splitValue = (int(pairList[i][0]) + .5)  # TALK ABOUT THIS IN REPORT

        if splitValue != oldSplitValue:
            oldSplitValue = splitValue

            listLess, listMore = splitByValue(pairList, splitValue)
            #

            if len(listLess) > 0:
                coll = collections.Counter(listLess[i][1] for i in range(len(listLess)))
                freqList = [x for x in coll.most_common()]

                if len(freqList) < 2:  # CHECK IF ONLY ONE CLASS VALUE IN SET
                    entropyLesser = -(int(freqList[0][1]) / size) * math.log(int(freqList[0][1]) / size, 2)
                else:
                    entropyLesser = -((int(freqList[0][1]) / size) * math.log(int(freqList[0][1]) / size, 2) + (
                    int(freqList[1][1]) / size) * math.log(int(freqList[1][1]) / size, 2))

            if len(listMore) > 0:  # NEED BETTER CONDITIONAL STATEMENT TO PREVENT OUT OF RANGE ERRORS
                coll = collections.Counter(listMore[i][1] for i in range(len(listMore)))
                freqList = [x for x in coll.most_common()]

                # NEED to: check if both class values are in set
                if len(freqList) < 2:  # CHECK IF ONLY ONE CLASS VALUE IN SET
                    entropyGreater = -(int(freqList[0][1]) / size) * math.log(int(freqList[0][1]) / size, 2)
                else:
                    entropyGreater = -((int(freqList[0][1]) / size) * math.log(int(freqList[0][1]) / size, 2) + (
                    int(freqList[1][1]) / size) * math.log(int(freqList[1][1]) / size, 2))

            netEntropy = (len(listLess) / size) * entropyLesser + (len(listMore) / size) * entropyGreater

            entropyGain = baseEntropy - netEntropy
            # print(entropyGain)
            if maxEntropyGain < entropyGain:
                maxEntropyGain = entropyGain
                splitValueMaxGain = splitValue

    data_sublist1 = []
    data_sublist2 = []
    # print("split val:",splitValueMaxGain)

    for i in (range(len(dataList))):  # create two sub lists, split on best split value
        if (int(dataList[i][index]) <= splitValueMaxGain):
            # print(dataList[i][index],"is less than val")
            data_sublist1.append(dataList[i])
        elif (int(dataList[i][index]) > splitValueMaxGain):
            # print(dataList[i][index],"is greater than than val")
            data_sublist2.append(dataList[i])

    return data_sublist1, data_sublist2, splitValueMaxGain, maxEntropyGain


splitPoints = []


def returnSplitPoints(dataList, index, entropyGain):
    if (entropyGain < .05):
        return None;
    else:
        data_sublist1, data_sublist2, splitPoint, entropyGain = entropy_discretize(dataList, index)
        splitPoint1 = returnSplitPoints(data_sublist1, index, entropyGain)
        if splitPoint1 != None:  # WHY NOT GETTING HERE?
            splitPoints.append(splitPoint1)
        splitPoint2 = returnSplitPoints(data_sublist2, index, entropyGain)
        if splitPoint2 != None:
            splitPoints.append(splitPoint2)
    return splitPoint


def discretize_EntropyBased(examples, index, attributes):
    attributes[index] = 'nom'

    # splitPoints = []

    returnSplitPoints(examples, index, 1)

    # print("IN DESCRETIZE", splitPoints)

    splitValuesList = list(OrderedDict.fromkeys(splitPoints))  # remove duplicates

    if splitValuesList:

        for i in range(len(splitValuesList)):  # sort list
            for j in range(len(splitValuesList) - 1 - i):
                if splitValuesList[j] > splitValuesList[j + 1]:
                    splitValuesList[j], splitValuesList[j + 1] = splitValuesList[j + 1], splitValuesList[j]

        if splitValuesList[0] == 0:
            splitValuesList.remove(0)  # remove o as a split value
        # print(splitValuesList)

        for i in range(len(examples)):  # do all below lowest split
            if (int(examples[i][index]) <= int(splitValuesList[0])):
                lowFill = "<="
                lowFill += str(splitValuesList[0])
                # print("get here 1")
                # print("changed", examples[i][index],"to",lowFill)
                examples[i][index] = lowFill


        # print(splitValuesList[len(splitValuesList)-1])
        for i in range(len(examples)):  # all above highest split
            if (examples[i][index].isdigit()):
                if (int(examples[i][index]) > int(splitValuesList[len(splitValuesList) - 1])):
                    highFill = ">"
                    highFill += str(splitValuesList[len(splitValuesList) - 1])

                    examples[i][index] = highFill
                    # print("get here 2")

        for splitIndex in range(len(splitValuesList)):
            for i in range(len(examples)):
                current = examples[i][index]
                if (current.isdigit()):
                    examples[i][index] = 'between'
                    # print("GOT HERE!!!", current)
                    # if ((int(current) <= int(splitValuesList[splitIndex])) and (int(current) > int(splitValuesList[splitIndex]-1))):
                    #     lowFill = "<="
                    #     lowFill += str(splitValuesList[splitIndex])
                    #     examples[i][index] = lowFill
                    #     print("got here 3")
                    #     print("changed", examples[i][index],"to",lowFill)
                    # elif ((int(examples[i][index]) > int(splitValuesList[splitIndex])) and (int(examples[i][index]) <= int(splitValuesList[splitIndex]+1))):
                    #     highFill = ">"
                    #     highFill += str(splitValuesList[splitIndex])
                    #     examples[i][index] = highFill
                    #     print("g0t here 4")
                    #     print("changed", examples[i][index],"to",highFill)
    else:
        print("ERROR: not enough Entropy Gain, did not discrtize for index:", index)


def splitDataset(dataset, splitRatio):
    trainSize = int(len(dataset) * splitRatio)
    trainSet = []
    copy = list(dataset)
    while len(trainSet) < trainSize:
        index = random.randrange(len(copy))
        trainSet.append(copy.pop(index))
    return [trainSet, copy]


def separateByClass(dataset):
    separated = {}
    for i in range(len(dataset)):
        vector = dataset[i]
        if (vector[-1] not in separated):
            separated[vector[-1]] = []
        separated[vector[-1]].append(vector)
    return separated


def mean(numbers):
    return sum(numbers) / float(len(numbers))


def stdev(numbers):
    avg = mean(numbers)
    variance = sum([pow(x - avg, 2) for x in numbers]) / float(len(numbers) - 1)
    return math.sqrt(variance)


def posteriorProb_1(column):
    coll = collections.Counter(column[i] for i in range(len(column)))
    freqList = [x for x in coll.most_common()]

    count = 0
    attListMember = []
    attValueList = []

    numberOccurences = len(column)

    for i in range(len(freqList)):
        attListMember.append(freqList[i][0])
        for n in range(len(column)):
            if freqList[i][0] == column[n]:
                count += 1
        attListMember.append((count + .5) / (numberOccurences + 1))
        attValueList.append(attListMember)
        attListMember = []
        count = 0

    return attValueList


def splitByClass(dataset):
    separatedList1 = []
    separatedList2 = []

    splitList = []

    classValue1 = dataset[0][-1]
    classValue2 = ''
    for i in range(len(dataset)):
        if dataset[i][-1] != classValue1:
            classValue2 = dataset[i][-1]

    for example in dataset:
        if example[-1] == classValue1:
            separatedList1.append(example)
            # print(example)
        else:
            separatedList2.append(example)

    one = []
    one.append(classValue1)
    one.append(separatedList1)

    splitList.append(one)

    two = []
    two.append(classValue2)
    two.append(separatedList2)

    splitList.append(two)

    return splitList


def summarize_1(dataset, attributes):
    examples = []
    summaries = []
    ints = []

    for attribute in zip(*dataset):
        examples.append(attribute)  # gets all values from row i

    for count in range(len(examples)):

        if attributes[count] == 'num':
            # print("NUM: ", examples[count])
            for n in range(len(examples[count])):
                str = examples[count][n]
                ints.append(int(str))
            summaries.append([mean(ints), stdev(ints)])
            ints = []

        elif attributes[count] == 'nom':
            summaries.append(posteriorProb_1(examples[count]))

    return summaries


def calculateGausProb(x, mean, stdev):  # THIS IS GAUSSIAN, FOR NUMERIC
    e = 2.718
    pi = 3.14159
    exponent = math.exp(-(math.pow(x - mean, 2) / ((2 * math.pow(stdev, 2)) + 1)))  # ADDED 1's here in denomonator !!!
    return (1 / ((((math.sqrt(2 * math.pi) * stdev)) * exponent) + 1))  # AND HERE


def calculateClassProbabilities_1(summaries, attributes, inputVector):
    # print(summaries[0])
    # print(summaries[2])

    probabilities = {}
    probabilities[summaries[0]] = 1
    probabilities[summaries[2]] = 1
    currProb = 0

    for m in range(len(inputVector)):
        currProb = 1
        if attributes[m] == 'num':
            currProb = calculateGausProb(int(inputVector[m]), summaries[1][m][0], summaries[1][m][1])
            # print("num: ", currProb)
        elif attributes[m] == 'nom':
            for value in summaries[1][m]:
                if value[0] == inputVector[m]:
                    currProb = value[1]
                    # print("nom: ", currProb)
        # print(currProb)
        probabilities[summaries[0]] *= currProb

    for m in range(len(inputVector)):
        currProb = 0
        if attributes[m] == 'num':
            currProb = calculateGausProb(int(inputVector[m]), summaries[3][m][0], summaries[3][m][1])
            # print("num: ", currProb)
        elif attributes[m] == 'nom':
            for value in summaries[3][m]:
                if value[0] == inputVector[m]:
                    currProb = value[1]
                    # print("nom: ", currProb)
        # print(currProb)
        probabilities[summaries[2]] *= currProb

    return probabilities


def predict_1(summaries, attributes, inputVector):
    probabilities = calculateClassProbabilities_1(summaries, attributes, inputVector)
    bestLabel, bestProb = None, -1

    keys = probabilities.keys()

    if probabilities[keys[0]] > probabilities[keys[1]]:
        bestLabel = keys[0]
    elif probabilities[keys[0]] < probabilities[keys[1]]:
        bestLabel = keys[1]

    return bestLabel


def getPredictions_1(summaries, attributes, testSet):
    predictions = []
    for i in range(len(testSet)):
        result = predict_1(summaries, attributes, testSet[i])
        predictions.append(result)
    return predictions


def getAccuracy(testSet, predictions):
    correct = 0
    for x in range(len(testSet)):
        if testSet[x][-1] == predictions[x]:
            correct += 1
    return (correct / float(len(testSet))) * 100.0


def createSummariesByClass(trainingSet, attributes):
    list0, list1 = splitByClass(trainingSet)

    listSum0 = summarize_1(list0[1], attributes)
    listSum1 = summarize_1(list1[1], attributes)

    summaries = []
    summaries.append(list0[0])
    summaries.append(listSum0)
    summaries.append(list1[0])
    summaries.append(listSum1)

    return summaries


def partition(dataset, folds):
    partitionsSize = int(len(dataset) / folds)
    partition = []
    partitionList = []
    copy = list(dataset)

    count = 0
    while count < folds:
        count += 1
        while len(partition) < partitionsSize:
            index = random.randrange(len(copy))
            partition.append(copy.pop(index))
        partitionList.append(partition)
        partition = []

    return partitionList


def creatConfusionMatrix(predictions, testSet):
    # create array of two arrays
    # where classValue1 = 1    and classValue0 = 0
    #           predicted class
    #           +   -
    # actual +  tP  fN
    # class  -  fP  tN
    #           classValue0   classValue1

    #
    tP = 0
    fP = 0
    fN = 0
    tN = 0

    row = []
    matrix = []

    classValue1 = testSet[0][-1]
    classValue0 = ''
    for i in range(len(testSet)):
        if testSet[i][-1] != classValue1:
            classValue0 = testSet[i][-1]

    for x in range(len(testSet)):
        if predictions[x] == classValue1:  # P, but true or false?
            if testSet[x][-1] == predictions[x]:  # tP
                tP += 1
            else:  # fP
                fP += 1
        elif predictions[x] == classValue0:  # N, but true or false?
            if testSet[x][-1] == predictions[x]:  # tN
                tN += 1
            else:  # fN
                fN += 1

    row.append(tP)
    row.append(fN)
    matrix.append(row)
    row = []
    row.append(fP)
    row.append(tN)
    matrix.append(row)
    row = []
    row.append(classValue0)
    row.append(classValue1)
    matrix.append(row)

    return matrix


def obtainComplimentMatrix(
        matrix):  # useful for when only two class values .... when only two classes, can simply flip values in table --> TP <--> TN  &   FN <---> FP
    # flip tP & tN
    matrixCopy = list(matrix)

    temp = matrixCopy[0][0]
    matrixCopy[0][0] = matrixCopy[1][1]
    matrixCopy[1][1] = temp

    # flip fP & fN
    temp = matrixCopy[0][1]
    matrixCopy[0][1] = matrixCopy[1][0]
    matrixCopy[1][0] = temp

    # flip classValue0 & classValue1
    temp = matrixCopy[2][0]
    matrixCopy[2][0] = matrixCopy[2][1]
    matrixCopy[2][1] = temp

    return matrixCopy


def calcMicroPrecision(matrix):
    tP_A = matrix[0][0]
    fP_A = matrix[1][0]

    matrixFlipped = obtainComplimentMatrix(matrix)

    tP_B = matrixFlipped[0][0]
    fP_B = matrixFlipped[1][0]

    microPrecision = (tP_A + tP_B) / (tP_A + tP_B + fP_A + fP_B)
    return microPrecision


def calcMicroRecall(matrix):
    tP_A = matrix[0][0]
    fN_A = matrix[0][1]

    matrixFlipped = obtainComplimentMatrix(matrix)

    tP_B = matrixFlipped[0][0]
    fN_B = matrixFlipped[0][1]

    microRecall = (tP_A + tP_B) / (tP_A + tP_B + fN_A + fN_B)
    return microRecall


def calcMicroF1(matrix):
    return (2 * calcMicroPrecision(matrix) * calcMicroRecall(matrix)) / (
    calcMicroPrecision(matrix) + calcMicroRecall(matrix))


def calcPrecision(matrix):
    return matrix[0][0] / (matrix[0][0] + matrix[1][0])


def calcRecall(matrix):
    return matrix[0][0] / (matrix[0][0] + matrix[0][1])


def calcF1(matrix):
    return (2 * calcRecall(matrix) * calcPrecision(matrix)) / (calcRecall(matrix) + calcPrecision(matrix))


def calcMacroPrecision(matrix):
    precision_A = calcPrecision(matrix)
    matrixComp = obtainComplimentMatrix(matrix)
    precision_B = calcPrecision(matrixComp)

    return ((precision_A + precision_B) / 2)


def calcMacroRecall(matrix):
    recall_A = calcRecall(matrix)
    matrixComp = obtainComplimentMatrix(matrix)
    recall_B = calcRecall(matrixComp)

    return ((recall_A + recall_B) / 2)


def calcMacroF1(matrix):
    f1_A = calcF1(matrix)
    matrixComp = obtainComplimentMatrix(matrix)
    f1_B = calcF1(matrixComp)

    return ((f1_A + f1_B) / 2)


def calcAccuracy_CostSensitive(matrix):
    tP_A = matrix[0][0]
    fN_A = matrix[0][1]
    fP_A = matrix[1][0]
    tN_A = matrix[1][1]

    return ((tP_A + tN_A) / (tP_A + tN_A + fP_A + fN_A)) * 100.0


def crossValidation(data, attributes, folds, outputFile):
    with open(outputFile, 'w') as fout:
        fout.write('{:20} {:20} {:20} {:20} {:20} {:20} {:20}\n'.format("Micro P", "Micro R", "Micro F1", "Macro P",
                                                                        "Macro R", "Macro F1", "Accuracy"))

        partitions = partition(data, folds)
        trainData = []
        testData = []
        i = 0

        microP_sum = 0
        microR_sum = 0
        microF1_sum = 0
        macroP_sum = 0
        macroR_sum = 0
        macroF1_sum = 0
        acc_sum = 0

        while i < folds:
            trainData = []
            j = 0
            while j < folds:
                if i != j:
                    for example in range(len(partitions[j])):
                        trainData.append(partitions[j][example])
                        # print(partitions[j][example])
                else:
                    for example in range(len(partitions[j])):
                        testData.append(partitions[j][example])
                j += 1
            i += 1
            # for n in trainData:
            #     print(n)
            summaries = createSummariesByClass(trainData, attributes)
            predictions = getPredictions_1(summaries, attributes, testData)

            matrix = creatConfusionMatrix(predictions, testData)

            microP = calcMicroPrecision(matrix)
            microR = calcMicroRecall(matrix)
            microF1 = calcMicroF1(matrix)
            macroP = calcMacroPrecision(matrix)
            macroR = calcMacroRecall(matrix)
            macroF1 = calcMacroF1(matrix)
            acc = calcAccuracy_CostSensitive(matrix)

            microP_sum += microP
            microR_sum += microR
            microF1_sum += microF1
            macroP_sum += macroP
            macroR_sum += macroR
            macroF1_sum += macroF1
            acc_sum += acc
            print("Fold",i,":")
            print("Micro precision:", calcMicroPrecision(matrix))
            print("Micro recall:", calcMicroRecall(matrix))
            print("Micro F1:", calcMicroF1(matrix))
            print("Macro precision:", calcMacroPrecision(matrix))
            print("Macro recall:", calcMacroRecall(matrix))
            print("Macro F1:", calcMacroF1(matrix))
            print("Accuracy:", calcAccuracy_CostSensitive(matrix))

            print()
            fout.write('{:20} {:20} {:20} {:20} {:20} {:20} {:20}\n'.format(calcMicroPrecision(matrix),
                                                                            calcMicroRecall(matrix),
                                                                            calcMicroF1(matrix),
                                                                            calcMacroPrecision(matrix),
                                                                            calcMacroRecall(matrix),
                                                                            calcMacroF1(matrix),
                                                                            calcAccuracy_CostSensitive(matrix)))

        print()
        microP_avg = microP_sum / folds
        microR_avg = microR_sum / folds
        microF1_avg = microF1_sum / folds
        macroP_avg = macroP_sum / folds
        macroR_avg = macroR_sum / folds
        macroF1_avg = macroF1_sum / folds
        acc_avg = acc_sum / folds

        print("_________Averages over all folds________")
        print("Micro precision average:", microP_avg)
        print("Micro recall average:", microR_avg)
        print("Micro F1 average:", microF1_avg)
        print("Macro precision average:", macroP_avg)
        print("Macro recall average:", macroR_avg)
        print("Macro F1 average:", macroF1_avg)
        print("Accuracy average:", acc_avg)

        fout.write('\n')
        fout.write('{:20} {:20} {:20} {:20} {:20} {:20} {:20}\n'.format(microP_avg, microR_avg, microF1_avg, macroP_avg,
                                                                        macroR_avg, macroF1_avg, acc_avg))

        fout.flush()
        os.fsync(fout.fileno())
        fout.close()


def main():
    # NEED TO: Use WEKA

    inputFile = '/Users/Konrad/Downloads/adult-big.arff'
    outputFile = '/Users/Konrad/Downloads/adult.out'

    attributes = []

    examples = []

    load(inputFile, examples, attributes)

    replaceMissingValues(examples, attributes)

    # trainingSet, testSet = splitDataset(examples, .9)

    # discretize_EntropyBased(examples,0,attributes)
    discretize_EntropyBased(examples,10,attributes)
    discretize_EntropyBased(examples,11,attributes)
    discretize_EntropyBased(examples,12,attributes)

    # print("after discretization")
    # for n in testSet:
    #     print(n)
    crossValidation(examples, attributes, 10, outputFile)

main()
