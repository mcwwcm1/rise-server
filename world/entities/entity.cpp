// File: entity.cpp
// Purpose: Implements entity.h

#include "entity.h"

Matrix4x4 Entity::GetTransformMatrix()
{
	return getTRSMatrix(position, rotation, scale);
}

Double3 Entity::LocalPointToGlobal(Double3 point)
{
	return point * GetTransformMatrix();
}