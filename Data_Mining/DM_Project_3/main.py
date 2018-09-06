from __future__ import print_function
from sklearn import cross_validation
import numpy as np
import pandas as pd
from collections import Counter
from sklearn import preprocessing
from sklearn import metrics
from sklearn.naive_bayes import GaussianNB
import matplotlib.pyplot as plt
from scipy import interp
from sklearn.metrics import roc_curve, auc
from sklearn.cross_validation import StratifiedKFold
from sklearn.ensemble import BaggingClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.ensemble import VotingClassifier
from sklearn.dummy import DummyClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn import svm

def transormY(y):
    newY = list()
    for index in range(len(y)):
        if y[index] != 1:
            newY.append(0)
        else:
            newY.append(1)
    return newY

# ISSUES:

    #  TRY WEIGHTING OF VotingClassifier

# TO DO:

# what is it about the data set that is preventing better performance?



# ------------ LOAD CSV FILES --------------

df_train = pd.read_csv('/Users/Konrad/Downloads/train.csv')
# df_test = pd.read_csv('/Users/Konrad/Downloads/test.csv')
# df_smallTrain = pd.read_csv('/Users/Konrad/Downloads/train_small.csv')
# df_smallTest = pd.read_csv('/Users/Konrad/Downloads/test_small.csv')

#   ----------- PREPROCESSING ------------

#  1) REPLACE MISSING VALUES

# TRAIN
#Replace missing categorical values
indices = [i for i, tp in enumerate(df_train.dtypes) if tp == np.object]
names = [column_name for j, column_name in enumerate(df_train.dtypes.index) if j in indices]
for col in names:
    df_train[col].fillna(df_train[col].value_counts().index[0], inplace=True)

# Replace missing values in numeric columns with mode of that columne
df_train.fillna(df_train.mean(),inplace=True)
print ("Replaced missing values for train.csv")

# # TEST
# #Replace missing categorical values
# indices = [i for i, tp in enumerate(df_test.dtypes) if tp == np.object]
# names = [column_name for j, column_name in enumerate(df_test.dtypes.index) if j in indices]
# for col in names:
#     df_test[col].fillna(df_test[col].value_counts().index[0], inplace=True)

# # Replace missing values in numeric columns with mode of that columne
# df_test.fillna(df_test.mean(),inplace=True)
# print ("Replaced missing values for test.csv")


# # TRAIN_SMALL
# #Replace missing categorical values
# indices = [i for i, tp in enumerate(df_smallTrain.dtypes) if tp == np.object]
# names = [column_name for j, column_name in enumerate(df_smallTrain.dtypes.index) if j in indices]
# for col in names:
#     df_smallTrain[col].fillna(df_smallTrain[col].value_counts().index[0], inplace=True)

# # Replace missing values in numeric columns with mode of that columne
# df_smallTrain.fillna(df_smallTrain.mean(),inplace=True)
# print ("Replaced missing values for df_smallTrain.csv")

# # TEST_SMALL
# #Replace missing categorical values
# indices = [i for i, tp in enumerate(df_smallTest.dtypes) if tp == np.object]
# names = [column_name for j, column_name in enumerate(df_smallTest.dtypes.index) if j in indices]
# for col in names:
#     df_smallTest[col].fillna(df_smallTest[col].value_counts().index[0], inplace=True)

# # Replace missing values in numeric columns with mode of that columne
# df_smallTest.fillna(df_smallTest.mean(),inplace=True)
# print ("Replaced missing values for df_smallTest.csv")

# #Replace missing categorical values
# indices = [i for i, tp in enumerate(df_smallTrain.dtypes) if tp == np.object]
# names = [column_name for j, column_name in enumerate(df_smallTrain.dtypes.index) if j in indices]
# for col in names:
#     df_smallTrain[col].fillna(df_smallTrain[col].value_counts().index[0], inplace=True)

# # Replace missing values in numeric columns with mode of that columne
# df_smallTrain.fillna(df_smallTrain.mean(),inplace=True)
# print ("Replaced missing values for train_small.csv")

# #Replace missing categorical values
# indices = [i for i, tp in enumerate(df_smallTest.dtypes) if tp == np.object]
# names = [column_name for j, column_name in enumerate(df_smallTest.dtypes.index) if j in indices]
# for col in names:
#     df_smallTest[col].fillna(df_smallTest[col].value_counts().index[0], inplace=True)

# # Replace missing values in numeric columns with mode of that columne
# df_smallTest.fillna(df_smallTest.mean(),inplace=True)
# print ("Replaced missing values for test_small.csv")


#  2) CONVERT CATEGORICAL VALUES TO NUMERIC

# get list of column names for all categorical values
catIndices = [i for i, tp in enumerate(df_train.dtypes) if tp == np.object]

names = [column_name for j, column_name in enumerate(df_train.dtypes.index) if j in catIndices]
# for i, tp in enumerate(df_train.dtypes):
#     if tp == np.object:
#         indices.append(i)
# print(indices)
# print(names)


from sklearn.preprocessing import LabelEncoder


for i in range(len(names)): # for each categorical attribute
    le = LabelEncoder()
    df_train[ names[i] ] = le.fit_transform( df_train[ names[i] ]  )
    continue
#     currDict =  dict([(j,name) for j,name in enumerate(Counter(df_train[names[i]]))]) # obtain dict of all possiblle values
#     # print(currDict)

# # numpy matrices
# #  can rotate for free --> .t

#     for j in range(len(df_smallTrain[names[i]])): # iterate through column
#         index = 0
#         for key in currDict.values():  # and replace each categorical value with its corresponding index in the dict
#             if df_smallTrain[names[i] ] [j] == key:
#                 df_smallTrain[names[i] ] [j] = index
#             index += 1
print("Converted categorical values in train.csv to numeric encoding ")

# for i in range(len(names)): # for each categorical value
#     le = LabelEncoder()
#     df_test[ names[i] ] = le.fit_transform( df_test[ names[i] ]  )
#     continue
# #     currDict =  dict([(j,name) for j,name in enumerate(Counter(df_smallTest[names[i]]))]) # obtain dict of all possiblle values

# #     for j in range(len(df_smallTest[names[i]])): # iterate through column
# #         index = 0
# #         for key in currDict.values():  # and replace each categorical value with its corresponding index in the dict
# #             if df_smallTest[names[i] ] [j] == key:
# #                 df_smallTest[names[i] ] [j] = index
# #             index += 1
# print("Converted categorical values in test.csv to numeric encoding ")

# 4) FEATURE SELECTION

def selectValuableFeatures_train( X, y, ):

    model = ExtraTreesClassifier()
    model.fit(X, y)
    # display the relative importance of each attribute
    # a
    # # print(model.feature_importances_)
    importance = model.feature_importances_
    # # model
    # # print(importance)
    # a

    count = 0
    for i in range(len(importance)):
        if (importance[i]  != 0):
            count = count + 1

    model = LogisticRegression()
    # create the RFE model and select 3 attributes
    rfe = RFE(model, count)
    rfe = rfe.fit(X , y)

    # a
    # # summarize the selection of the attributes
    # # print(rfe.support_)
    # # print(rfe.ranking_)

    keep = list()
    remove = list()

    for i in range(len(rfe.ranking_)): # build list of columns to keep
        if rfe.ranking_[i] == 1:
            keep.append(i)
        else:
            remove.append(i)

    def extract_from_matrix(matrix, columns):
            a = np.array(matrix)
            extractedData = a[:,columns]
            extractedData = extractedData.astype(float)
            return extractedData.tolist()

    print("Number of features removed: " , len(remove))
    print("Number of features remaining: " , len(keep))

    return extract_from_matrix(X, keep), keep


def selectValuableFeatures_test( X, y, keep):

    model = ExtraTreesClassifier()
    model.fit(X, y)
    # display the relative importance of each attribute
    # print(model.feature_importances_)
    importance = model.feature_importances_
    model

    def extract_from_matrix(matrix, columns):
            a = np.array(matrix)
            extractedData = a[:,columns]
            extractedData = extractedData.astype(float)
            return extractedData.tolist()

    # print("Number of features removed: " , len(remove))
    # print("Number of features remaining: " , len(keep))

    # if keepList:
    #     keep = keepList

    return extract_from_matrix(X, keep)

# ----------- SET UP DATA --------

from sklearn.preprocessing import OneHotEncoder


# TRAIN
train = df_train.values
# print("Size of train: ", len(train))
X = train[:,2:]  # feature-object matrix
y = train[0:,1]   # values of the y target variable.
y = y.astype(int)

print("X dimenstionality: ", len(X[0]))
#  feature selection

# for item in X[0]:
#     print(item, end=" ")
# print(" ")

# X, keepList = selectValuableFeatures_train(X, y)

enc = OneHotEncoder(n_values='auto', categorical_features= catIndices ) # ONE HOT ENCODER --> encode converted categorical values
X = enc.fit_transform(X) # ONE HOT ENCODER
X = X.toarray()
print("X dimensionality after OneHotEncoder: ", len(X[0]))

# from sklearn.feature_selection import SelectFdr
from sklearn.feature_selection import SelectPercentile
sp = SelectPercentile(percentile=20)
sp.fit(X, y)
X = sp.transform(X)
print("X dimensionality after SelectPercentile: ", len(X[0]))

# print("after")
# for item in X[0]:
#     print(item, end=" ")
# print(" ")


# from sklearn.decomposition import PCA
# pca = PCA()
# pca.fit(X, y)
# X = pca.transform(X)
# print("X dimensionality after PCA: ", len(X[0]))

# fdr = SelectFdr()
# fdr.fit(X, y)
# X = fdr.transform(X)
# print("X dimensionality after SelectFDR: ", len(X[0]))


X = preprocessing.normalize(X)
# print(X[0])

# # TEST
# test = df_test.values
# # print("Size of test: ", len(test))
# X_test_real = test[:,1:]
# y_test_real = test[0:,1]
# y_test_real = y_test_real.astype(int)
# y_test_real = transormY(y_test_real)

# # print("X_test dimenstionality: ", len(X_test[0]))

# # for item in X[0]:
# #     print(item, end=" ")
# # print(" ")


# # X_test = selectValuableFeatures_test(X_test, y_test, keepList)

# X_test_real = enc.transform(X_test_real) # ONE HOT ENCODER
# X_test_real = X_test_real.toarray()
# print("X_test dimensionality after OneHotEncoder: ", len(X_test_real[0]))


# print("after")
# for item in X_test[0]:
#     print(item, end=" ")
# print(" ")

# X_test = pca.transform(X_test)
# print("X_test dimensionality after PCA: ", len(X_test[0]))


# X_test = fdr.transform(X_test)
# print("X_test dimensionality after SelectFDR: ", len(X_test[0]))

# X_test_real = sp.transform(X_test_real)
# print("X_test dimensionality after SelectPercentile: ", len(X_test_real[0]))


# X_test_real = preprocessing.normalize(X_test_real)


# print("X: ", X)
# print("y: ", y)

# print("X_test: ", X_test)
# print("y_test: ", y_test)

# X_test = preprocessing.normalize(X_test)

# from sklearn.cross_validation import KFold

# FOLDS = 3
# count = 0
# folds = KFold(len(X), FOLDS)
# for train_index, test_index in folds:
#     X_train, X_test = X[train_index], X[test_index]
#     y_train, y_test = y[train_index], y[test_index],

#     count = count + 1

#     print(" ")
#     print(" ")
#     print("--------- FOLD: ", count, " ---------")
#     print(" ")



#     #   ----------- CLASSIFIERS -----------------

#     # 0) DUMMY CLASSIFIER

#     from sklearn.dummy import DummyClassifier


#     print("Dummy Clasifier")
#     model = DummyClassifier()
#     model.fit(X_train, y_train)
#     # print(model)
#     # make predictions
#     expected = y_test
#     predicted = model.predict(X_test)
#     # summarize the fit of the model
#     print(metrics.classification_report(expected, predicted))
#     print(metrics.confusion_matrix(expected, predicted))
#     print("Area under ROC: ", metrics.roc_auc_score(expected, predicted) )
#     print("Log Loss: ", metrics.log_loss(expected,predicted))
#     print(" ")

#      # 1) NAIVE BAYES


#     print("Naive Bayes")
#     model = GaussianNB()
#     model.fit(X_train, y_train)
#     # print(model)
#     # make predictions
#     expected = y_test
#     predicted = model.predict(X_test)
#     # summarize the fit of the model
#     print(metrics.classification_report(expected, predicted))
#     print(metrics.confusion_matrix(expected, predicted))
#     print("Area under ROC: ", metrics.roc_auc_score(expected, predicted) )
#     print("Log Loss: ", metrics.log_loss(expected,predicted))
#     print(" ")


#     # 2) k-Nearest Neighbors


#     # fit a k-nearest neighbor model to the data
#     model =  KNeighborsClassifier(weights='distance')
#     model.fit(X_train, y_train)
#     print(model)
#     # make predictions
#     expected = y_test
#     predicted = model.predict(X_test)
#     # summarize the fit of the model
#     print( "KNeighborsClassifier" )
#     print(metrics.classification_report(expected, predicted))
#     print(metrics.confusion_matrix(expected, predicted))
#     print("Area under ROC: ", metrics.roc_auc_score(expected, predicted) )
#     print("Log Loss: ", metrics.log_loss(expected,predicted))
#     print(" ")
#     # print(model.decision_function(X_test))
#     # probabilities = model.predict_proba(X_test)
#     # for item in probabilities:
#     #     print (item)




#     # 4) EXPERIMENTING WITH CLASS WEIGHTING, SVM

#     # fit the model and get the separating hyperplane

#     # train = df_smallTrain.values
#     # X = train[:,2:]  # feature-object matrix
#     # y = train[0:,1]   # values of the y target variable.
#     # y = y.astype(int)

#     # X = preprocessing.normalize(X)

#     # # print ("X: ", X)

#     # #  feature selection
#     # X, keepList = selectValuableFeatures_train(X, y)
#     # print ("Attribute SIZE X w/ feature selection: ", len(X[0]))



#     # clf = svm.SVC(kernel='linear', C=1.0)
#     # clf.fit(X, y)

#     # w = clf.coef_[0]
#     # a = -w[0] / w[1]
#     # xx = np.linspace(-5, 5)
#     # yy = a * xx - clf.intercept_[0] / w[1]

#     # # get the separating hyperplane using weighted classes
#     # wclf = svm.SVC(kernel='linear', class_weight={1: 10})
#     # wclf.fit(X, y)

#     # ww = wclf.coef_[0]
#     # wa = -ww[0] / ww[1]
#     # wyy = wa * xx - wclf.intercept_[0] / ww[1]

#     # # plot separating hyperplanes and samples
#     # h0 = plt.plot(xx, yy, 'k-', label='no weights')
#     # h1 = plt.plot(xx, wyy, 'k--', label='with weights')
#     # plt.scatter(X[:, 0], X[:, 1], c=y, cmap=plt.cm.Paired)
#     # plt.legend()

#     # plt.axis('tight')
#     # plt.show()


#     #  WIERD THING: TEST DOESNT HAVE A y .... How to deal with this?
#     # .... probably just cant use classification_report and confusion_matrix for test data,
#     # have to use bootstrapping (or holdout would probs be better) for generation of test sets that have class values


#     # 7) ENSAMBLE CLASSIFIERS
#     from sklearn.ensemble import BaggingClassifier   # THIS WITH UN-NORMALIZED DATA GETS best 1 precision (.77) (but only .29 recall)
#     print("BaggingClassifier()")
#     bagging = BaggingClassifier( max_features=.5)
#     bagging.fit(X_train, y_train)
#     expected = y_test
#     predicted = bagging.predict(X_test)
#     # summarize the fit of the model
#     print(metrics.classification_report(expected, predicted))
#     print(metrics.confusion_matrix(expected, predicted))
#     print("Area under ROC: ", metrics.roc_auc_score(expected, predicted) )
#     print("Log Loss: ", metrics.log_loss(expected,predicted))
#     print(" ")
#     probabilities = bagging.predict_proba(X_test)

#     from sklearn.ensemble import RandomForestClassifier
#     print(" RandomForestClassifier(n_estimators=10)")
#     clf = RandomForestClassifier(n_estimators=10)
#     clf = clf.fit(X_train, y_train)
#     expected = y_test
#     predicted = clf.predict(X_test)
#     # summarize the fit of the model
#     print(metrics.classification_report(expected, predicted))
#     print(metrics.confusion_matrix(expected, predicted))
#     print("Area under ROC: ", metrics.roc_auc_score(expected, predicted) )
#     print("Log Loss: ", metrics.log_loss(expected,predicted))
#     print(" ")
#     # probabilities = clf.predict_proba(X_test)
#     # for item in probabilities:
#     #     print (item)

#     # probabilities = clf.predict_proba(X_test)

#     from sklearn.ensemble import AdaBoostClassifier
#     print(" AdaBoostClassifier(n_estimators=100)")
#     clf = AdaBoostClassifier(n_estimators=100)
#     clf = clf.fit(X_train, y_train)
#     expected = y_test
#     predicted = clf.predict(X_test)
#     # summarize the fit of the model
#     print(metrics.classification_report(expected, predicted))
#     print(metrics.confusion_matrix(expected, predicted))
#     print("Area under ROC: ", metrics.roc_auc_score(expected, predicted) )
#     print("Log Loss: ", metrics.log_loss(expected,predicted))
#     print(" ")
#     # probabilities = clf.predict_proba(X_test)

#     from sklearn.ensemble import VotingClassifier
#     from sklearn.linear_model import LogisticRegression
#     from sklearn.naive_bayes import GaussianNB
#     from sklearn.ensemble import RandomForestClassifier
#     print(" VotingClassifier(estimators= ('lr', clf1), ('rf', clf2), ('gnb', clf3)], voting='hard')")
#     clf1 = GaussianNB()
#     clf2 = BaggingClassifier()
#     clf3 = RandomForestClassifier(n_estimators=100)
#     eclf1 = VotingClassifier(estimators=[
#         ('b', clf2), ('rf', clf3)], voting='hard')
#     eclf1 = eclf1.fit(X_train, y_train)
#     expected = y_test
#     predicted = eclf1.predict(X_test)
#     # summarize the fit of the model
#     print(metrics.classification_report(expected, predicted))
#     print(metrics.confusion_matrix(expected, predicted))
#     print("Area under ROC: ", metrics.roc_auc_score(expected, predicted) )
#     print("Log Loss: ", metrics.log_loss(expected,predicted))
#     print(" ")

#    # 3) SVM

#     # print ("Attribute SIZE X_test w/ feature selection: ", len(X_test[0]))

#     print("SVC: probability = True , kernel='rbf')")
#     model = svm.SVC( kernel='rbf', max_iter = 20, class_weight={0: 2}) #"""class_weight={0: 5}"""
#     model.fit(X_train, y_train)
#     # print(model)
#     # make predictions
#     expected = y_test
#     predicted = model.predict(X_test)
#     # summarize the fit of the model
#     print(metrics.classification_report(expected, predicted))
#     print(metrics.confusion_matrix(expected, predicted))
#     print("Area under ROC: ", metrics.roc_auc_score(expected, predicted) )
#     print("Log Loss: ", metrics.log_loss(expected,predicted))
#     print(" ")

#     print("SVC: probability = True , kernel= 'poly')")
#     model = svm.SVC( kernel = 'poly', max_iter = 20, class_weight={0: 2}) #"""class_weight={0: 5}"""
#     model.fit(X_train, y_train)
#     # print(model)
#     # make predictions
#     expected = y_test
#     predicted = model.predict(X_test)
#     # summarize the fit of the model
#     print(metrics.classification_report(expected, predicted))
#     print(metrics.confusion_matrix(expected, predicted))
#     print("Area under ROC: ", metrics.roc_auc_score(expected, predicted) )
#     print("Log Loss: ", metrics.log_loss(expected,predicted))
#     print(" ")

#     print("SVC: probability = True , kernel= 'sigmoid')")
#     model = svm.SVC( kernel = 'sigmoid', max_iter = 50, class_weight={0: 2}) #"""class_weight={0: 5}"""
#     model.fit(X_train, y_train)
#     # print(model)
#     # make predictions
#     expected = y_test
#     predicted = model.predict(X_test)
#     # summarize the fit of the model
#     print(metrics.classification_report(expected, predicted))
#     print(metrics.confusion_matrix(expected, predicted))
#     print("Area under ROC: ", metrics.roc_auc_score(expected, predicted) )
#     print("Log Loss: ", metrics.log_loss(expected,predicted))
#     print(" ")




    # 8) PRINTING ROC CURVE WITH CROSS VALIDATION:
print("Processing for printing of ROC --- Best Classifiers Only")


# Run classifier with cross-validation and plot ROC curves
random_state = np.random.RandomState(0)

cv = StratifiedKFold(y, n_folds=2)
# classifier = svm.SVC( kernel = 'poly', max_iter = 100, probability = True, random_state = True)

clf1 = GaussianNB()
clf2 = BaggingClassifier()
clf3 = RandomForestClassifier(n_estimators=10)
classifier = VotingClassifier(estimators=[
    ('bagging', clf2), ('randomForest', clf3)], voting='soft')
dummy = DummyClassifier()

mean_tpr = 0.0
mean_fpr = np.linspace(0, 1, 100)
all_tpr = []

for i, (train, test) in enumerate(cv):
    print("FOLD: ", i)
    probas_ = classifier.fit(X[train], y[train]).predict_proba(X[test])
    # print(probas_, "END")
    # Compute ROC curve and area the curve
    fpr, tpr, thresholds = roc_curve(y[test], probas_[:, 1])
    mean_tpr += interp(mean_fpr, fpr, tpr)
    mean_tpr[0] = 0.0
    roc_auc = auc(fpr, tpr)
    plt.plot(fpr, tpr, lw=1, label='Voting fold %d (area = %0.2f)' % (i, roc_auc))  # change names here

    print("Perforamnce for Voting:")
    expected = y[test]
    predicted = classifier.predict(X[test])
    # summarize the fit of the model
    print(metrics.classification_report(expected, predicted ))
    print(metrics.confusion_matrix(expected, predicted))
    # print("Area under ROC: ", metrics.roc_auc_score(expected, predicted) )
    print("Log Loss: ", metrics.log_loss(expected,probas_))
    print(" ")

#
    probas_ = clf1.fit(X[train], y[train]).predict_proba(X[test])
    # print(probas_, "END")
    # Compute ROC curve and area the curve
    fpr, tpr, thresholds = roc_curve(y[test], probas_[:, 1])
    mean_tpr += interp(mean_fpr, fpr, tpr)
    mean_tpr[0] = 0.0
    roc_auc = auc(fpr, tpr)
    plt.plot(fpr, tpr, lw=1, label='NaiveBayes fold %d (area = %0.2f)' % (i, roc_auc))  # change names here

    print("Perforamnce for NaiveBayes:")
    expected = y[test]
    predicted = clf1.predict(X[test])
    # summarize the fit of the model
    print(metrics.classification_report(expected, predicted))
    print(metrics.confusion_matrix(expected, predicted))
    # print("Area under ROC: ", metrics.roc_auc_score(expected, predicted) )
    print("Log Loss: ", metrics.log_loss(expected,probas_))
    print(" ")

    probas_ = clf2.fit(X[train], y[train]).predict_proba(X[test])
    # print(probas_, "END")
    # Compute ROC curve and area the curve
    fpr, tpr, thresholds = roc_curve(y[test], probas_[:, 1])
    mean_tpr += interp(mean_fpr, fpr, tpr)
    mean_tpr[0] = 0.0
    roc_auc = auc(fpr, tpr)
    plt.plot(fpr, tpr, lw=1, label='Bagging fold %d (area = %0.2f)' % (i, roc_auc))  # change names here

    print("Perforamnce for Bagging:")
    expected = y[test]
    predicted = clf2.predict(X[test])
    # summarize the fit of the model
    print(metrics.classification_report(expected, predicted))
    print(metrics.confusion_matrix(expected, predicted))
    # print("Area under ROC: ", metrics.roc_auc_score(expected, predicted) )
    print("Log Loss: ", metrics.log_loss(expected,probas_))
    print(" ")


    probas_ = clf3.fit(X[train], y[train]).predict_proba(X[test])
    # print(probas_, "END")
    # Compute ROC curve and area the curve
    fpr, tpr, thresholds = roc_curve(y[test], probas_[:, 1])
    mean_tpr += interp(mean_fpr, fpr, tpr)
    mean_tpr[0] = 0.0
    roc_auc = auc(fpr, tpr)
    plt.plot(fpr, tpr, lw=1, label='RandomForest fold %d (area = %0.2f)' % (i, roc_auc))  # change names here

    print("Perforamnce for RandomForest:")
    expected = y[test]
    predicted = clf3.predict(X[test])
    # summarize the fit of the model
    print(metrics.classification_report(expected, predicted))
    print(metrics.confusion_matrix(expected, predicted))
    # print("Area under ROC: ", metrics.roc_auc_score(expected, predicted) )
    print("Log Loss: ", metrics.log_loss(expected,probas_))
    print(" ")

    probas_ = dummy.fit(X[train], y[train]).predict_proba(X[test])
    # print(probas_, "END")
    # # Compute ROC curve and area the curve
    # fpr, tpr, thresholds = roc_curve(y[test], probas_[:, 1])
    # mean_tpr += interp(mean_fpr, fpr, tpr)
    # mean_tpr[0] = 0.0
    # roc_auc = auc(fpr, tpr)
    # plt.plot(fpr, tpr, lw=1, label='Dummy fold %d (area = %0.2f)' % (i, roc_auc))  # change names here

    print("Perforamnce for Dummy:")
    expected = y[test]
    predicted = dummy.predict(X[test])
    # summarize the fit of the model
    print(metrics.classification_report(expected, predicted))
    print(metrics.confusion_matrix(expected, predicted))
    # print("Area under ROC: ", metrics.roc_auc_score(expected, predicted) )
    print("Log Loss: ", metrics.log_loss(expected,probas_))
    print(" ")

plt.plot([0, 1], [0, 1], '--', color=(0.6, 0.6, 0.6), label='Luck')

mean_tpr /= len(cv)
mean_tpr[-1] = 1.0
mean_auc = auc(mean_fpr, mean_tpr)
# # plt.plot(mean_fpr, mean_tpr, 'k--',
#          label='Mean ROC (area = %0.2f)' % mean_auc, lw=2)

plt.xlim([-0.05, 1.05])
plt.ylim([-0.05, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver operating characteristic example')
plt.legend(loc="lower right")
# plt.frame.Maximize(True)
plt.show()
print("AUC printed")

