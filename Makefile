#Environment
CXX = g++
DBFLAGS = -g -Wall -pthread -std=c++20
BDFLAGS = -Wall -o2 -pthread -std=c++20
LPQFLAGS = -lpqxx -lpq

#The build target
TARGETS=rise-test rise-server
all: $(TARGETS)
rise-test: main.o
	$(CXX) $(DBFLAGS) -o $@ $^ $(LPQFLAGS)
rise-server: main.o
	$(CXX) $(BDFLAGS) -o $@ $^ $(LPQFLAGS)

#The Ingredients
main.o: main.cpp core/circularbuffer.h core/session.h core/listener.h core/globals.h core/send.h core/register.h primary/echotest.h primary/echo.h

# utility
clean:
	rm -f *.o $(TARGETS)
