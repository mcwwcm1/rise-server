// File: physicsspace.cpp
// Purpose: Implements physicsspace.h

#include "physicsspace.h"
#include "shapes/sphereshape.h"
#include "constraints/distanceconstraint.h"
#include "../entities/dynamicentity.h"

PhysicsSpace::PhysicsSpace() { PhysicsSpace(0); }

PhysicsSpace::PhysicsSpace(double updateRate)
{
	this->updateRate = updateRate;
	this->fixedDT = 1 / updateRate;
}

void PhysicsSpace::RegisterEntity(PhysicsEntity* entity)
{
	entities.push_back(entity);
	entity->space = this;
}

void PhysicsSpace::UnregisterEntity(PhysicsEntity* entity)
{
    for (auto it = entities.begin(); it != entities.end(); ++it) {
        if (*it == entity) {
            entities.erase(it);
            break;
        }
    }

	entity->space = nullptr;
}

void PhysicsSpace::RunTick()
{
	for(PhysicsEntity* entity : entities){
		entity->RunTick(fixedDT);
	}
}