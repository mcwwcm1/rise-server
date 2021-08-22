// File: physicsspace.cpp
// Purpose: Implements physicsspace.h

#include "physicsspace.h"

#include "../entities/dynamicentity.h"
#include "constraints/distanceconstraint.h"
#include "shapes/sphereshape.h"

PhysicsSpace::PhysicsSpace() { PhysicsSpace(0); }

PhysicsSpace::PhysicsSpace(double updateRate)
{
	this->UpdateRate = updateRate;
	this->FixedDT    = 1 / updateRate;
}

void PhysicsSpace::RegisterEntity(PhysicsEntity* entity)
{
	entities.push_back(entity);
	entity->Space = this;
}

void PhysicsSpace::UnregisterEntity(PhysicsEntity* entity)
{
	for (auto it = entities.begin(); it != entities.end(); ++it) {
		if (*it == entity) {
			entities.erase(it);
			break;
		}
	}

	entity->Space = nullptr;
}

void PhysicsSpace::RunTick()
{
	for (PhysicsEntity* entity : entities) { entity->RunTick(FixedDT); }
}