#Environment
CXX = g++
DBFLAGS = -g -Wall -pthread -std=c++20
BDFLAGS = -Wall -o2 -pthread -std=c++20
LPQFLAGS = -lpqxx -lpq
SRCS := $(shell find . -name "*.cpp")
OBJS = $(SRCS:.cpp=.o)

#The build target
TARGETS=rise-test
all: $(TARGETS)

depend: .depend

.depend: $(SRCS)
	rm -f "$@"
	$(CC) $(CFLAGS) -MM $^ > "$@"

include .depend

rise-test: $(OBJS)
	$(CXX) $(DBFLAGS) -o $@ $^ $(LPQFLAGS)
rise-server: $(OBJS)
	$(CXX) $(BDFLAGS) -o $@ $^ $(LPQFLAGS)

.cpp.o:
	$(CXX) $(DBFLAGS) -c $< -o $@

# utility
clean:
	rm -f $(OBJS) $(TARGETS) .depend
