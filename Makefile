#Environment
CXX = g++
CXXFLAGS = -g -Wall -pthread -std=c++20 -lpqxx -lpq

#The build target
TARGETS=buffertest
all: $(TARGETS)
buffertest: main.o core/session.o
	$(CXX) $(CXXFLAGS) -o $@ $^

#The Ingredients
main.o: main.cpp core/circularbuffer.h core/fail.h core/session.h primary/indexes.h

session.o: core/session.cpp core/session.h core/fail.h

# utility
clean:
	rm -f *.o $(TARGETS)
