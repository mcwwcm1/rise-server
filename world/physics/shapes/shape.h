// File: shape.h
// Purpose: An abstract file for defining collission shapes

#ifndef SHAPE_H
#define SHAPE_H

#include "../../../mysticmath/double3.h"
#include "../../../mysticmath/quaternion.h"

enum ShapeType { Sphere, Box };

class Shape
{
 public:
	ShapeType Type;
	Double3 Position;
	Quaternion Rotation;
	virtual Double3 GetFurthestPointInDirection(const Double3& direction);

	Shape();
};

#endif