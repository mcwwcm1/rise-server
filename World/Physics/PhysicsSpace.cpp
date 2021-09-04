// File: physicsspace.cpp
// Purpose: Implements physicsspace.h

#include "PhysicsSpace.h"

#include "World/Entities/DynamicEntity.h"
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

void PhysicsSpace::RegisterConstraint(std::string id, btTypedConstraint* constraint)
{
	if (GetConstraint(id) != nullptr) {
		printf("Warning: Attempted to register constraint but a constraint with the id %s is already registered!\n", id.c_str());
		return;
	}

	DynamicsWorld->addConstraint(constraint);
	Constraints[id] = constraint;
}
void PhysicsSpace::UnregisterConstraint(std::string id)
{
	auto c = GetConstraint(id);
	if (c == nullptr) {
		printf("Warning: Attempted to unregister constraint (%s) but it was not found!\n", id.c_str());
		return;
	}

	DynamicsWorld->removeConstraint(c);
	Constraints.erase(id);
}
btTypedConstraint* PhysicsSpace::GetConstraint(std::string id)
{
	auto constraint = Constraints.find(id);
	if (constraint == Constraints.end())
		return nullptr;
	return constraint->second;
}

void PhysicsSpace::RunTick()
{
	DynamicsWorld->stepSimulation(1 / UpdateRate, 0);  // Step simulation
}