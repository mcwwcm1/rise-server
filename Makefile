#Environment
CXX = g++-11
DBFLAGS = -g -Wall -pthread -std=c++20 -DBT_USE_DOUBLE_PRECISION
BDFLAGS = -Wall -o2 -pthread -std=c++20
ROOT_DIR:=$(shell dirname "$(realpath $(firstword $(MAKEFILE_LIST)))")
INCLUDES = -I"$(ROOT_DIR)" -I/usr/include/bullet
LPQFLAGS = -lpqxx -lpq -lBullet3Collision-float64 -lBullet3Common-float64 -lBullet3Dynamics-float64 -lBullet3Geometry-float64 -lBullet3OpenCL_clew-float64 -lBulletCollision-float64 -lBulletDynamics-float64 -lBulletSoftBody-float64 -lLinearMath-float64
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
