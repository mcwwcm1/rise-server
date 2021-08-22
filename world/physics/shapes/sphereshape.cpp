// File: sphereShape.cpp
// Purpose: Implements sphereShape.h

#include "sphereshape.h"

SphereShape::SphereShape(float radius)
{
	this->size = Double3(radius, radius, radius);
	this->Type = ShapeType::Sphere;
}

SphereShape::SphereShape(Double3 size)
{
	this->size = size;
	this->Type = ShapeType::Sphere;
}

Double3 SphereShape::GetFurthestPointInDirection(const Double3& direction)
{
	if (size.IsUniform()) return direction * size.x;

	Double3 d = direction * Rotation;
	return d * GetScaleMatrix(size) * Rotation.Conjugate();
};