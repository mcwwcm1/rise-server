// File: constraint.h
// Purpose: Defines a physical constraint for things like joints

#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "../../../mysticmath/quaternion.h"
class DynamicEntity;
class Movement;

class Constraint
{
	public:
		Double3 attachmentPoint;
		Quaternion attachmentRotation;
		virtual void ApplyConstraint(DynamicEntity* entity, Movement* movement) = 0;
};

#endif