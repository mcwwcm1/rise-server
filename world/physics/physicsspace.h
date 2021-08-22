// File: physicsspace.h
// Purpose: Contains physics bodies and runs the main physics tick

#pragma once

#include <vector>

#include "../entities/physicsentity.h"

class PhysicsSpace
{
 public:
	std::vector<PhysicsEntity*> entities = std::vector<PhysicsEntity*>(0);

	double UpdateRate;
	double FixedDT;

	PhysicsSpace(double updateRate);
	PhysicsSpace();  // I know why this has to be here :) It's silly

	void RegisterEntity(PhysicsEntity* boentitydy);
	void UnregisterEntity(PhysicsEntity* entity);

	void RunTick();
	void CheckCollision();
};