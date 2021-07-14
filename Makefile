#Environment
CXX = g++
CXXFLAGS = -g -Wall -pthread -std=c++20 -lpqxx -lpq

#The build target
TARGETS=buffertest
all: $(TARGETS)
buffertest: main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

#The Ingredients
main.o: main.cpp core/circularbuffer.h core/session.h core/listener.h core/globals.h primary/echotest.h

# utility
clean:
	rm -f *.o $(TARGETS)
