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
	printf("ADDED BODY\n");
	bodies[0] = rigidbody;
}

void PhysicsSpace::RunTick()
{
	printf("STARTED RUNNING TICK FOR BODIES\n");
	printf("%ld\n", bodies.size());
	for(Rigidbody body : bodies)
	{
		printf("RUNNING TICK FOR BODY\n");
		body.RunTick(fixedDT);
	}
}