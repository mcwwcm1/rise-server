// File: airship.cpp
// Purpose: Implements airship.h

#include "airship.h"

Airship::Airship(const PhysicsSpace& space)
{
	this->space = space;
	this->space.AddBody(rigidbody);
}

Matrix4x4 Airship::GetTransformMatrix() { return getTRSMatrix(rigidbody.position, rigidbody.rotation, rigidbody.scale); }

Double3 Airship::GetPosition() { return rigidbody.position; }
Quaternion Airship::GetRotation() { return rigidbody.rotation; }
Double3 Airship::GetScale() { return rigidbody.scale; }

void Airship::RunTick()
{
	Double3 forward, right, up;

	forward = GetForward();
	right = GetRight();
	up = GetUp();
	
	float speed = rigidbody.velocity.magnitude();

	// Apply thrust
	rigidbody.AddForce(forward * throttle * 2);

	// Apply yaw steering
	rigidbody.AddTorque(up * yaw * speed * 10);

	// Apply pitch steering
	rigidbody.AddTorque(right * pitch * speed);

	// Self-righting
	rigidbody.AddTorque(cross(Double3(0, -1, 0), up) * 50);
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