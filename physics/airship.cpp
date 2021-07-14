// File: airship.cpp
// Purpose: Implements airship.h

#include "airship.h"

Airship::Airship(const PhysicsSpace& space)
{
	this->space = space;
	this->space.AddBody(rigidbody);
}

Matrix4x4 Airship::GetTransformMatrix()
{
	return getTRSMatrix(rigidbody.position, rigidbody.rotation, rigidbody.scale);
}

void Airship::RunTick()
{
	// Apply thrust
	rigidbody.AddForce(GetForward() * throttle);

	// Apply yaw steering
	rigidbody.AddTorque(GetUp() * yaw);

	// Apply pitch steering
	rigidbody.AddForce(GetRight() * pitch);
}

Double3 Airship::GetForward()
{
	return Double3(0, 0, 1) * rigidbody.rotation;
}

Double3 Airship::GetRight()
{
	return Double3(1, 0, 0) * rigidbody.rotation;
}

Double3 Airship::GetUp()
{
	return Double3(0, 1, 0) * rigidbody.rotation;
}