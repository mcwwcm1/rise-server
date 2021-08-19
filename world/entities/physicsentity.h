// File: physicsentity.h
// Purpose: An entity that has a rigidbody

#ifndef PHYSICSENTITY_H
#define PHYSICSENTITY_H

#include "entity.h"
#include "../physics/shapes/shape.h"
#include <vector>

class PhysicsSpace;

class PhysicsEntity : public Entity
{
	public:
		PhysicsEntity();
		PhysicsEntity(PhysicsSpace* space);
		virtual ~PhysicsEntity();

		PhysicsSpace* space;
		std::vector<Shape*> colliders;

		virtual void RunTick(float dt);
};

#endif