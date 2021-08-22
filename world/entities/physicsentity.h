// File: physicsentity.h
// Purpose: An entity that has a rigidbody

#ifndef PHYSICSENTITY_H
#define PHYSICSENTITY_H

#include <vector>

#include "../physics/shapes/shape.h"
#include "entity.h"

class PhysicsSpace;

class PhysicsEntity : public Entity
{
 public:
	PhysicsEntity();
	PhysicsEntity(string id);
	virtual ~PhysicsEntity();

	PhysicsSpace* Space;
	std::vector<Shape*> Colliders;

	virtual void RunTick(float dt);
};

#endif