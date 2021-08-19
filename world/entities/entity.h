// File: entity.h
// Purpose: a thing in the world that has a location

#pragma once

#include "../../mysticmath/double3.h"
#include "../../mysticmath/quaternion.h"
#include "../../mysticmath/matrix4x4.h"

class Entity
{
	public:
		virtual ~Entity() = default;

		Double3 position = Double3(0, 0, 0);
		Quaternion rotation = Quaternion::identity;
		Double3 scale = Double3(1, 1, 1);

		Matrix4x4 GetTransformMatrix();
		Double3 LocalPointToGlobal(Double3 point);
};