// File: physicsspace.cpp
// Purpose: Implements physicsspace.h

#include "physicsspace.h"

PhysicsSpace::PhysicsSpace() { PhysicsSpace(0); }

PhysicsSpace::PhysicsSpace(double updateRate)
{
	this->updateRate = updateRate;
	this->fixedDT = 1 / updateRate;
}

void PhysicsSpace::AddBody(Rigidbody rigidbody)
{
	bodies.push_back(rigidbody);
}

void PhysicsSpace::RunTick()
{
	for(Rigidbody body : bodies)
	{
		body.RunTick(fixedDT);
	}
}