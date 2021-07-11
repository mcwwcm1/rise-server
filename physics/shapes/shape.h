// File: shape.h
// Purpose: An abstract file for defining collission shapes

#ifndef SHAPE_H
#define SHAPE_H

#include "float3.h"
#include "quaternion.h"

class Shape
{
	public:
		Float3 position;
		Quaternion rotation;
		virtual Float3 getFurthestPointInDirection(Float3 direction);		
};

#endif