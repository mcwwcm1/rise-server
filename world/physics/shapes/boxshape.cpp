// File: boxShape.cpp
// Purpose: Implements boxShape.h

#include "boxShape.h"
#include "mysticmath.h"

BoxShape::BoxShape(Float3 size)
{
	this->size = size;
}

BoxShape::BoxShape(float x, float y, float z)
{
	this->size = Float3(x, y, z);
}

Float3 BoxShape::getFurthestPointInDirection(Float3 direction)
{
	return ((direction * rotation).sign() * size) * rotation.conjugate();
}