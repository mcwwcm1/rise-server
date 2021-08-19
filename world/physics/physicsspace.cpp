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
	if(entities.size() > 1)
	{
		// Add test constraint
		DistanceConstraint* ropeConstraint = new DistanceConstraint();
		ropeConstraint->isRigid = true;
		ropeConstraint->distance = 10;
		ropeConstraint->point = Double3(0, 0, 2);
		ropeConstraint->attachmentPoint = Double3(0, 0, -2);
		ropeConstraint->targetEntity = entities[1];
		
		PhysicsEntity* entity = entities[0];
		DynamicEntity* dynEntity = dynamic_cast<DynamicEntity*>(entity);
		if(dynEntity != nullptr)
		{
			dynEntity->constraints.push_back(ropeConstraint);
			printf("ADDED DEBUG CONSTRAINT \n");
		}
	}
}

void PhysicsSpace::UnregisterEntity(PhysicsEntity* entity)
{
    for (auto it = entities.begin(); it != entities.end(); ++it) {
        if (*it == entity) {
            entities.erase(it);
            break;
        }
    }
}

void PhysicsSpace::RunTick()
{
	for(PhysicsEntity* entity : entities)
		entity->RunTick(fixedDT);
}