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
	Constraint();
	Constraint(string id);
	string ID;
	Double3 AttachmentPoint;
	Quaternion AttachmentRotation;
	virtual void ApplyConstraint(DynamicEntity* entity, Movement* movement) = 0;
};

#endif