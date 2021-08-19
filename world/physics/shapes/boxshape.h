// File: boxShape.h
// Purpose: Defines a box shaped collider

#include "shape.h"

class BoxShape: public Shape
{
	public:
		BoxShape(Float3 size);
		BoxShape(float x, float y, float z);
		Float3 size;
		virtual Float3 getFurthestPointInDirection(Float3 direction);
};