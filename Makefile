#Environment
CXX = g++
DBFLAGS = -g -Wall -pthread -std=c++20 -lpqxx -lpq
BDFLAGS = -Wall -o2 -pthread -std=c++20 -lpqxx -lpq

#The build target
TARGETS=rise-test rise-server
all: $(TARGETS)
rise-test: main.o
	$(CXX) $(DBFLAGS) -o $@ $^
rise-server: main.o
	$(CXX) $(BDFLAGS) -o $@ $^

#The Ingredients
main.o: main.cpp core/circularbuffer.h core/session.h core/listener.h core/globals.h primary/echotest.h primary/echo.h

# utility
clean:
	rm -f *.o $(TARGETS)
