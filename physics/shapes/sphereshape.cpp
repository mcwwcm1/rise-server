// File: sphereShape.cpp
// Purpose: Implements sphereShape.h

#include "sphereShape.h"
#include "matrix4x4.h"

SphereShape::SphereShape(float radius)
{
	this->size = Float3(radius, radius, radius);
}

SphereShape::SphereShape(Float3 size)
{
	this-> size = size;
}

Float3 SphereShape::getFurthestPointInDirection(Float3 direction)
{
	if(size.isUniform())
		return direction * size.x;

	Float3 d = direction * rotation;
	return d * getScaleMatrix(size) * rotation.conjugate();
};