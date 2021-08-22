// File: distanceconstraint.h
// Purpose: Defines a constraint that limits the distance to a specified point

#pragma once

#include "constraint.h"
#include "mysticmath/double3.h"
#include "world/entities/entity.h"

class DistanceConstraint : public Constraint
{
 public:
	DistanceConstraint(const std::string& id);
	Entity* TargetEntity;
	Double3 TargetPoint;
	float Distance;
	bool IsRigid;
	float TensileForce;

	void ApplyConstraint(DynamicEntity* entity, Movement* movement);
};