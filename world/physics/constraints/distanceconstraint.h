// File: distanceconstraint.h
// Purpose: Defines a constraint that limits the distance to a specified point

#ifndef DISTANCECONSTRAINT_H
#define DISTANCECONSTRAINT_H

#include "../../../mysticmath/double3.h"
#include "../../entities/entity.h"
#include "constraint.h"

class DistanceConstraint : public Constraint
{
 public:
	DistanceConstraint(string id);
	Entity* targetEntity;
	Double3 targetPoint;
	float distance;
	bool isRigid;
	float tensileForce;

	void ApplyConstraint(DynamicEntity* entity, Movement* movement);
};

#endif