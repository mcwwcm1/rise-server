// File: physicsspace.h
// Purpose: Contains physics bodies and runs the main physics tick

#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include <vector>

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

	double UpdateRate;
	double FixedDT;

	void RegisterEntity(DynamicEntity* boentitydy);
	void UnregisterEntity(DynamicEntity* entity);

	void RunTick();
	void CheckCollision();
};