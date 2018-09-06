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
CFLAGS = -Wall -g
#CFLAGS = -std=c+11x -Wall -g
SRCS = main.cpp Attribute.cpp NumericAttribute.cpp AttributeFactory.cpp Attributes.cpp NominalAttribute.cpp DataSet.cpp TrainTestSets.cpp Example.cpp Examples.cpp

OBJS = ${SRCS:.cpp=.o}

a.out: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS)

$(OBJS):
	$(CC) $(CFLAGS) -c $*.cpp

depend:
	$(CC) -MM $(SRCS) > Makefile.dep

docs:
	rm -fr html latex
	/usr/bin/doxygen doxygen.config *.h *.cc

Makefile.dep:
	touch Makefile.dep

clean:
	rm -f $(OBJS) a.out core

include Makefile.dep






