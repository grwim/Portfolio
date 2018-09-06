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
CFLAGS = -std=c++11 -Wall -g
COMMON_SOURCES = ForestRI.cpp Attribute.cpp NumericAttribute.cpp AttributeFactory.cpp Attributes.cpp NominalAttribute.cpp DataSet.cpp TrainTestSets.cpp Example.cpp Examples.cpp Performance.cpp Classifier.cpp Evaluator.cpp 
ForestRI_SOURCES = forestri-main.cpp ID3_RI.cpp ForestRI.cpp
ID3_SOURCES = id3-main.cpp ID3.cpp

COMMON_OBJECTS = ${COMMON_SOURCES:.cpp=.o}

ForestRI_OBJECTS = ${ForestRI_SOURCES:.cpp=.o}

ForestRI: $(ForestRI_OBJECTS) $(COMMON_OBJECTS)
	$(CC) $(CFLAGS) $(ForestRI_OBJECTS) $(COMMON_OBJECTS) -o ForestRI

ID3_OBJECTS = ${ID3_SOURCES:.cpp=.o}

ID3: $(ID3_OBJECTS) $(COMMON_OBJECTS)
	$(CC) $(CFLAGS) $(ID3_OBJECTS) $(COMMON_OBJECTS) -o ID3

$(COMMON_OBJECTS):
	$(CC) $(CFLAGS) -c $*.cpp

depend:
	$(CC) -MM $(COMMON_OBJECTS) > Makefile.dep

docs:
	rm -fr html latex
	/usr/bin/doxygen doxygen.config *.h *.cc

Makefile.dep:
	touch Makefile.dep

clean_ForestRI:
	rm -f $(COMMONOBJS) ForestRI core

clean_ID3:
	rm -f $(COMMONOBJS) ID3 core

include Makefile.dep

