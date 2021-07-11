// File: collisiondetection.h
// Purpose: Performs the GJK algorithm for finding collisions between arbitrary convex shapes and then performs the EPA to collect collision information

#include "float3.h"
#include "shape.h"
#include <vector>

struct Simplex
{
	private:
		bool line(Float3& direction);
		bool triangle(Float3& direction);
		bool tetrahedron(Float3& direction);

	public:
		Float3 points [4];
		int pointCount;		

		void pushFront(Float3 point);
		bool doSimplex(Float3& direction);
};

struct CollisionResult
{
	public:
		bool collided;
		Float3 displacement;
		Float3 normal;
		Float3 point;
};

Float3 support(Shape shape1, Shape shape2, Float3 direction);

CollisionResult checkCollision(Shape shape1, Shape shape2);

bool sameDirection(Float3 a, Float3 b);