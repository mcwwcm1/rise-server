// File: physicsspace.h
// Purpose: Contains physics bodies and runs the main physics tick

#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletDynamics/ConstraintSolver/btTypedConstraint.h>
#include <vector>
#include <unordered_map>

#include "../entities/dynamicentity.h"

class DynamicEntity;

class PhysicsSpace
{
 private:
	btDefaultCollisionConfiguration* CollissionConfiuration;
	btCollisionDispatcher* CollissionDispatcher;
	btBroadphaseInterface* OverlappingPairChache;
	btSequentialImpulseConstraintSolver* ConstraintsSolver;

 public:
	PhysicsSpace(double updateRate);
	PhysicsSpace();  // I know why this has to be here :) It's silly

	btDiscreteDynamicsWorld* DynamicsWorld;

	std::vector<DynamicEntity*> entities = std::vector<DynamicEntity*>(0);
	std::unordered_map<std::string, btTypedConstraint*> Constraints;

	double UpdateRate;
	double FixedDT;

	void RegisterEntity(DynamicEntity* boentitydy);
	void UnregisterEntity(DynamicEntity* entity);

	void RegisterConstraint(std::string id, btTypedConstraint* constraint);
	void UnregisterConstraint(std::string id);
	btTypedConstraint* GetConstraint(std::string id);

	void RunTick();
	void CheckCollision();
};