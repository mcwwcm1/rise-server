#Environment
CXX = g++
DBFLAGS = -g -Wall -pthread -std=c++20
BDFLAGS = -Wall -o2 -pthread -std=c++20
LPQFLAGS = -lpqxx -lpq

#The build target
TARGETS=rise-test
all: $(TARGETS)

rise-test: main.o mysticmath/double3.o mysticmath/quaternion.o mysticmath/matrix4x4.o world/physics/physicsspace.o world/physics/airship.o world/physics/shapes/shape.o world/physics/shapes/sphereshape.o world/physics/constraints/distanceconstraint.o world/entities/entity.o world/entities/physicsentity.o world/entities/dynamicentity.o
	$(CXX) $(DBFLAGS) -o $@ $^ $(LPQFLAGS)
# rise-server: main.o mysticmath/double3.o mysticmath/quaternion.o mysticmath/matrix4x4.o world/physics/physicsspace.o world/physics/rigidbody.o world/physics/airship.o world/physics/shapes/shape.o world/physics/shapes/sphereshape.o
#	$(CXX) $(BDFLAGS) -o $@ $^ $(LPQFLAGS)

#The Ingredients
main.o: main.cpp core/circularbuffer.h core/session.h core/listener.h core/globals.h core/send.h primary/echotest.h primary/echo.h primary/echoto.h primary/physicstick.h

double3.o: mysticmath/double3.cpp mysticmath/double3.h
quaternion.o: mysticmath/quaternion.cpp mysticmath/quaternion.h
matrix4x4.o: mysticmath/matrix4x4.cpp mysticmath/matrix4x4.h
distanceconstraint.o: world/physics/constraints/distanceconstraint.cpp world/physics/constraints/constraint.h
physicsspace.o: world/physics/physicsspace.cpp world/physics/physicsspace.h
airship.o: world/physics/airship.cpp world/physics/airship.h
entity.o: world/entities/entity.cpp world/entities/entity.h
physicsentity.o: world/entities/physicsentity.cpp world/entities/physicsentity.h
dynamicentity.o: world/entities/dynamicentity.cpp world/entities/dynamicentity.h
shape.o: world/physics/shapes/shape.cpp world/physics/shapes/shape.h
sphereshape.o: world/physics/shapes/sphereshape.cpp world/physics/shapes/sphereshape.h

# utility
clean:
	rm -f *.o $(TARGETS)
