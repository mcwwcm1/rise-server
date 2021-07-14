// File: rigidbody.h
// Purpose: Defines properties of a rigid physics body

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../mysticmath/double3.h"
#include "../mysticmath/quaternion.h"

class Rigidbody
{
	private:
		Double3 bufferedForce;
		Double3 bufferedTorque;

	public:
		Double3 position;
		Quaternion rotation;
		Double3 scale;
		Double3 velocity;
		Double3 torque;
		float drag = 1;
		float rotationalDrag = 1;

		// Constructors
		Rigidbody();

		// Apply forces
		void AddForce(const Double3& force);
		void AddImpulseForce(const Double3& force);

		void AddTorque(const Double3& torque);
		void AddImpulseTorque(const Double3& torque);

		void RunTick(float dt);
};

#endif