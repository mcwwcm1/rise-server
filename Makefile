#Environment
CXX = g++
CXXFLAGS = -g -Wall -Werror -pthread

#The build target
TARGETS=buffertest
all: $(TARGETS)
buffertest: main.o
	$(CXX) $(CXXFLAGS) -o buffertest main.o

#The Ingredients
main.o: main.cpp circularbuffer/circularbuffer.h
	$(CXX) $(CXXFLAGS) -c main.cpp

# utility
clean:
	rm -f *.o $(TARGETS)
