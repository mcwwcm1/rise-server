// File: physicsspace.h
// Purpose: Contains physics bodies and runs the main physics tick

#ifndef PHYSICSSPACE_H
#define PHYSICSSPACE_H

#include <vector>
#include "rigidbody.h"

class PhysicsSpace
{
	public:
		std::vector<Rigidbody*> bodies = std::vector<Rigidbody*>(0);
		std::vector<Shape*> staticColliders = std::vector<Shape*>(0);

		double updateRate;
		double fixedDT;

		PhysicsSpace(double updateRate);
		PhysicsSpace(); // I know why this has to be here :) It's silly

		void RegisterBody(Rigidbody* body);
		void RegisterStaticCollider(Shape* collider);

		void RunTick();
		void CheckCollision();
};

#endif