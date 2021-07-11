#Environment
CXX = g++
CXXFLAGS = -g -Wall -pthread -std=c++20

#The build target
TARGETS=buffertest
all: $(TARGETS)
buffertest: main.o
	$(CXX) $(CXXFLAGS) -o buffertest main.o

#The Ingredients
main.o: main.cpp circularbuffer/circularbuffer.h primary/indexes.h
	$(CXX) $(CXXFLAGS) -c main.cpp

# utility
clean:
	rm -f *.o $(TARGETS)
