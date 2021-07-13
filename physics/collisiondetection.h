// File: collisiondetection.h
// Purpose: Performs the GJK algorithm for finding collisions between arbitrary convex shapes and then performs the EPA to collect collision information

#include "float3.h"
#include "float4.h"
#include "shape.h"
#include <vector>

struct Edge
{
	public:
		int index;
		float distance;
		Float3 normal;
};

struct Simplex
{
	private:
		bool line(Float3& direction);
		bool triangle(Float3& direction);
		bool tetrahedron(Float3& direction);

	public:
		std::vector<Float3> points;
		int pointCount;		

		void pushFront(Float3 point);
		bool doSimplex(Float3& direction);
		Edge findClosestEdge();

		auto begin() const { return points.begin(); }
		auto end()   const { return points.end() - (4 - pointCount); }
};

struct CollisionResult
{
	public:
		bool collided;
		float displacement;
		Float3 normal;
		Float3 point;
};

Float3 support(Shape shape1, Shape shape2, Float3 direction);

CollisionResult checkCollision(Shape shape1, Shape shape2);

CollisionResult handleCollision(Shape shape1, Shape shape2, Simplex simplex);

bool sameDirection(Float3 a, Float3 b);