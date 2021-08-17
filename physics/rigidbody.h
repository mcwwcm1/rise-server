// File: rigidbody.h
// Purpose: Defines properties of a rigid physics body

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../mysticmath/double3.h"
#include "../mysticmath/quaternion.h"
#include "../mysticmath/matrix4x4.h"
#include "shapes/shape.h"
#include <vector>

class Rigidbody
{
	private:
		Double3 bufferedForce = Double3(0, 0, 0);
		Double3 bufferedTorque = Double3(0, 0, 0);
		Double3 bufferedImpulseForce = Double3(0, 0, 0);
		Double3 bufferedImpulseTorque = Double3(0, 0, 0);

	public:
		Double3 position = Double3(0, 0, 0);
		Quaternion rotation = Quaternion::identity;
		Double3 scale = Double3(1, 1, 1);
		Double3 velocity = Double3(0, 0, 0);
		Double3 torque = Double3(0, 0, 0);
		float drag = 1;
		float rotationalDrag = 1;
		float bounciness = 0.2f;
		float friction = 0.2f;
		float mass = 10.0f;

		std::vector<Shape*> colliders;

		// Constructors
		Rigidbody();
		Rigidbody(string id);

		Matrix4x4 GetTransformMatrix();

		// Apply forces
		void AddForce(const Double3& force);
		void AddImpulseForce(const Double3& force);

		void AddTorque(const Double3& torque);
		void AddTorque(const Quaternion& torque);
		void AddImpulseTorque(const Double3& torque);

		void AddForceAtPosition(const Double3& force, const Double3& position);
		void AddImpulseForceAtPosition(const Double3& force, const Double3& position);
		Double3 GetReflectedForce(const Double3& force, const Double3& normal);

		void RunTick(float dt);
};

#endif