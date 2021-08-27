// File: physicsspace.cpp
// Purpose: Implements physicsspace.h

#include "physicsspace.h"

#include "../entities/dynamicentity.h"
#include <bullet/btBulletDynamicsCommon.h>

PhysicsSpace::PhysicsSpace() { PhysicsSpace(0); }

PhysicsSpace::PhysicsSpace(double updateRate)
{
	this->UpdateRate = updateRate;
	this->FixedDT    = 1 / updateRate;

	CollissionConfiuration = new btDefaultCollisionConfiguration();
	CollissionDispatcher   = new btCollisionDispatcher(CollissionConfiuration);
	OverlappingPairChache  = new btDbvtBroadphase();
	ConstraintsSolver      = new btSequentialImpulseConstraintSolver;
	DynamicsWorld          = new btDiscreteDynamicsWorld(CollissionDispatcher, OverlappingPairChache, ConstraintsSolver, CollissionConfiuration);

	DynamicsWorld->setGravity(btVector3(0, -9.812, 0));
}

void PhysicsSpace::RegisterEntity(DynamicEntity* entity)
{
	entities.push_back(entity);
	entity->Space = this;

	entity->RegisterToDynamicsWorld(DynamicsWorld);
}

void PhysicsSpace::UnregisterEntity(DynamicEntity* entity)
{
	for (auto it = entities.begin(); it != entities.end(); ++it) {
		if (*it == entity) {
			entities.erase(it);
			break;
		}
	}

	entity->Space = nullptr;

	entity->UnregisterFromDynamicsWorld(DynamicsWorld);
}

void PhysicsSpace::RunTick()
{
	for (DynamicEntity* entity : entities)
		entity->RunTick(FixedDT);

	DynamicsWorld->stepSimulation(1 / UpdateRate, 0);  // Step simulation
}