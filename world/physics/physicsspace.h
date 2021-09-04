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
	btDefaultCollisionConfiguration* CollissionConfiuration = nullptr;
	btCollisionDispatcher* CollissionDispatcher             = nullptr;
	btBroadphaseInterface* OverlappingPairChache            = nullptr;
	btSequentialImpulseConstraintSolver* ConstraintsSolver  = nullptr;

 public:
	PhysicsSpace(double updateRate);
	PhysicsSpace();  // I know why this has to be here :) It's silly

	btDiscreteDynamicsWorld* DynamicsWorld = nullptr;

	std::vector<DynamicEntity*> entities = std::vector<DynamicEntity*>(0);
	std::unordered_map<std::string, btTypedConstraint*> Constraints;

	double UpdateRate = 0.0;
	double FixedDT    = 0.0;

	void RegisterEntity(DynamicEntity* boentitydy);
	void UnregisterEntity(DynamicEntity* entity);

	void RegisterConstraint(std::string id, btTypedConstraint* constraint);
	void UnregisterConstraint(std::string id);
	btTypedConstraint* GetConstraint(std::string id);

	void RunTick();
	void CheckCollision();
};