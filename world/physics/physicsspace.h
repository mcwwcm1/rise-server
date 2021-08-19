// File: physicsspace.h
// Purpose: Contains physics bodies and runs the main physics tick

#ifndef PHYSICSSPACE_H
#define PHYSICSSPACE_H

#include <vector>
#include "../entities/physicsentity.h"

class PhysicsSpace
{
	public:
		std::vector<PhysicsEntity*> entities = std::vector<PhysicsEntity*>(0);

		double updateRate;
		double fixedDT;

		PhysicsSpace(double updateRate);
		PhysicsSpace(); // I know why this has to be here :) It's silly

		void RegisterEntity(PhysicsEntity* boentitydy);
		void UnregisterEntity(PhysicsEntity* entity);

		void RunTick();
		void CheckCollision();
};

#endif