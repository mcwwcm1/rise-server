#Environment
CXX = g++
CXXFLAGS = -g -Wall -pthread -std=c++20 -lpqxx -lpq

#The build target
TARGETS=buffertest
all: $(TARGETS)
buffertest: main.o
	$(CXX) -o buffertest main.o ${CXXFLAGS}

#The Ingredients
main.o: main.cpp circularbuffer/circularbuffer.h primary/indexes.h
	$(CXX) -c main.cpp $(CXXFLAGS)

# utility
clean:
	rm -f *.o $(TARGETS)
