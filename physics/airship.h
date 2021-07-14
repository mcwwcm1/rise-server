// File: airship.h
// Purpose: An instance of an airship who's data will be synced to Neos

#ifndef AIRSHIP_H
#define AIRSHIP_H

#include "rigidbody.h"
#include "../mysticmath/matrix4x4.h"
#include "physicsspace.h"

class Airship
{
	public:
		PhysicsSpace space;
		Rigidbody rigidbody;
		float throttle, pitch, yaw;

		Airship(const PhysicsSpace& space);

		Matrix4x4 GetTransformMatrix();

		void RunTick();

		Double3 GetForward();
		Double3 GetRight();
		Double3 GetUp();
};

#endif