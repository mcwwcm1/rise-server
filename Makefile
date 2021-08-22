#Environment
CXX = g++
DBFLAGS = -g -Wall -pthread -std=c++20
BDFLAGS = -Wall -o2 -pthread -std=c++20
ROOT_DIR:=$(shell dirname "$(realpath $(firstword $(MAKEFILE_LIST)))")
INCLUDES = -I"$(ROOT_DIR)"
LPQFLAGS = -lpqxx -lpq
SRCS := $(shell find . -name "*.cpp")
OBJS = $(SRCS:.cpp=.o)

#The build target
TARGETS=rise-test
all: $(TARGETS)

depend: .depend

.depend: $(SRCS)
	rm -f "$@"
	$(CC) $(CFLAGS) $(INCLUDES) -MM $^ > "$@"

include .depend

rise-test: $(OBJS)
	$(CXX) $(DBFLAGS) $(INCLUDES) -o $@ $^ $(LPQFLAGS)
rise-server: $(OBJS)
	$(CXX) $(BDFLAGS) $(INCLUDES) -o $@ $^ $(LPQFLAGS)

.cpp.o:
	$(CXX) $(DBFLAGS) $(INCLUDES) -c $< -o $@

# utility
clean:
	rm -f $(OBJS) $(TARGETS) .depend
