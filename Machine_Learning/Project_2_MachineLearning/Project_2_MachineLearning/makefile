# Instructions:
#
# 1. Add source files to the line beginning with SRCS = main.cc.
# 2. To build the dependency list, on the command line, type 'make depend'.
#    You'll need to do this whenever you add a source file.
# 3. To build the project, on the command line, type 'make'.
# 4. To start over, type 'make clean', which does not clean the
#    dependency list.
#
 
CC = g++
#CFLAGS = -Wall -g
CFLAGS = -std=c++11x -Wall -g
COMMON_SOURCES = Attribute.cpp NumericAttribute.cpp AttributeFactory.cpp Attributes.cpp NominalAttribute.cpp DataSet.cpp TrainTestSets.cpp Example.cpp Examples.cpp Performance.cpp Classifier.cpp Evaluator.cpp
KNN_SOURCES = knn-main.cpp
NB_SOURCES = nb-main.cpp

COMMON_OBJECTS = ${COMMON_SOURCES:.cpp=.o}

KNN_OBJECTS = ${KNN_SOURCES:.cpp=.o}

NB_OBJECTS = ${NB_SOURCES:.cpp=.o}

NaiveBayes: $(NB_OBJECTS) $(COMMON_OBJECTS)
	$(CC) $(CFLAGS) $(NB_OBJECTS) $(COMMON_OBJECTS) -o NaiveBayes

kNN: $(KNN_OBJECTS) $(COMMON_OBJECTS)
	$(CC) $(CFLAGS) $(KNN_OBJECTS) $(COMMON_OBJECTS) -o kNN


#$(COMMON_OBJECTS):
#	$(CC) $(CFLAGS) -c $*.cpp

#$(KNN_OBJECTS):
#	$(CC) $(CFLAGS) -c $*.cpp

$(NB_OBJECTS):
	$(CC) $(CFLAGS) -c $*.cpp

depend:
	$(CC) -MM $(COMMON_OBJECTS) > Makefile.dep

docs:
	rm -fr html latex
	/usr/bin/doxygen doxygen.config *.h *.cc

Makefile.dep:
	touch Makefile.dep

clean_NaiveBayes:
	rm -f $(COMMONOBJS) NaiveBayes core

clean_kNN:
	rm -f $(COMMONOBJS) kNN core

include Makefile.dep

