#Environment
CXX = g++
CXXFLAGS = -g -Wall -pthread -std=c++20 -lpqxx -lpq

#The build target
TARGETS=buffertest
all: $(TARGETS)
buffertest: main.o mysticmath/double3.o mysticmath/quaternion.o mysticmath/matrix4x4.o physics/physicsspace.o physics/rigidbody.o physics/airship.o
	$(CXX) $(CXXFLAGS) -o $@ $^

#The Ingredients
main.o: main.cpp core/circularbuffer.h core/session.h core/listener.h core/globals.h primary/echotest.h primary/physicstick.h

double3.o: mysticmath/double3.cpp mysticmath/double3.h
quaternion.o: mysticmath/quaternion.cpp mysticmath/quaternion.h
matrix4x4.o: mysticmath/matrix4x4.cpp mysticmath/matrix4x4.h
physicsspace.o: physics/physicsspace.cpp physics/physicsspace.h
rigidbody.o: physics/rigidbody.cpp physics/rigidbody.h 
airship.o: physics/airship.cpp physics/airship.h

# utility
clean:
	rm -f *.o $(TARGETS)
