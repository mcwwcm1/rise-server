// File: sphereShape.cpp
// Purpose: Implements sphereShape.h

#include "sphereshape.h"

SphereShape::SphereShape(float radius)
{
	this->size = Double3(radius, radius, radius);
	this->type = ShapeType::Sphere;
}

SphereShape::SphereShape(Double3 size)
{
	this->size = size;
	this->type = ShapeType::Sphere;
}

Double3 SphereShape::getFurthestPointInDirection(const Double3& direction)
{
	if (size.isUniform()) return direction * size.x;

	Double3 d = direction * rotation;
	return d * getScaleMatrix(size) * rotation.conjugate();
};