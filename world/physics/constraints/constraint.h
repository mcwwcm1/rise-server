// File: constraint.h
// Purpose: Defines a physical constraint for things like joints

#pragma once

#include <string>

#include "../../../mysticmath/quaternion.h"

class DynamicEntity;
class Movement;

class Constraint
{
 public:
	Constraint();
	Constraint(std::string id);

	std::string ID;
	Double3 AttachmentPoint;
	Quaternion AttachmentRotation;
	virtual void ApplyConstraint(DynamicEntity* entity, Movement* movement) = 0;
};