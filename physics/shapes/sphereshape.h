// File: sphereShape.h
// Purpose: Defines a sphere shape for collision

#include "shape.h"

struct SphereShape: Shape
{
	public:
		SphereShape(float radius);
		SphereShape(Float3 size);
		Float3 size;
		virtual Float3 getFurthestPointInDirection(Float3 direction);
};