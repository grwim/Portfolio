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
COMMON_SOURCES = BP.cpp Attribute.cpp NumericAttribute.cpp AttributeFactory.cpp Attributes.cpp NominalAttribute.cpp DataSet.cpp TrainTestSets.cpp Example.cpp Examples.cpp Performance.cpp Classifier.cpp Evaluator.cpp 
BP_SOURCES = bp-main.cpp

COMMON_OBJECTS = ${COMMON_SOURCES:.cpp=.o}

BP_OBJECTS = ${BP_SOURCES:.cpp=.o}

BP: $(BP_OBJECTS) $(COMMON_OBJECTS)
	$(CC) $(CFLAGS) $(BP_OBJECTS) $(COMMON_OBJECTS) -o BP

$(COMMON_OBJECTS):
	$(CC) $(CFLAGS) -c $*.cpp

depend:
	$(CC) -MM $(COMMON_OBJECTS) > Makefile.dep

docs:
	rm -fr html latex
	/usr/bin/doxygen doxygen.config *.h *.cc

Makefile.dep:
	touch Makefile.dep

clean_BP:
	rm -f $(COMMONOBJS) BP core

include Makefile.dep

