// File: physicsspace.h
// Purpose: Contains physics bodies and runs the main physics tick

#ifndef PHYSICSSPACE_H
#define PHYSICSSPACE_H

#include <vector>
#include "rigidbody.h"

class PhysicsSpace
{
	public:
		std::vector<Rigidbody> bodies = std::vector<Rigidbody>(1);

		double updateRate;
		double fixedDT;

		PhysicsSpace(double updateRate);
		PhysicsSpace(); // I know why this has to be here :) It's silly

		void AddBody(Rigidbody body);

		void RunTick();
};

#endif