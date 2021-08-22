// File: airship.cpp
// Purpose: Implements airship.h

#include "airship.h"

#include "../physics/constraints/distanceconstraint.h"
#include "../physics/shapes/sphereshape.h"

size_t Airship::_currentAirshipIndex = 0;  // Dumb.

Airship::Airship(string id) : DynamicEntity(id)
{
	// HARDCODED COLLIDER SHAPE = BAD
	SphereShape* s1 = new SphereShape(2);
	s1->Position    = Double3(0, 0, -2.5);
	Colliders.push_back(s1);

	SphereShape* s2 = new SphereShape(2);
	s2->Position    = Double3(0, 0, -0.5);
	Colliders.push_back(s2);

	SphereShape* s3 = new SphereShape(2);
	s3->Position    = Double3(0, 0, 1.5);
	Colliders.push_back(s3);

	Throttle = 0;
	Pitch    = 0;
	Yaw      = 0;
	Drag     = 0.5f;
}

Matrix4x4 Airship::GetTransformMatrix()
{
	return GetTRSMatrix(Position, Rotation, Scale);
}

Double3 Airship::GetPosition() { return Position; }
Quaternion Airship::GetRotation() { return Rotation; }
Double3 Airship::GetScale() { return Scale; }

void Airship::RunTick(float dt)
{
	Double3 forward, right, up;

	forward = GetForward();
	right   = GetRight();
	up      = GetUp();

	float speed = Velocity.Magnitude();

	// Apply thrust
	AddForce(forward * Throttle * 30);

	// Apply yaw steering
	AddTorque(up * Yaw * speed * -70);

	// Apply pitch steering
	AddTorque(right * Pitch * speed * 30);

	// Self-righting
	AddTorque(Cross(Double3(0, -1, 0), up) * 500);

	DynamicEntity::RunTick(dt);
}

Double3 Airship::GetForward() { return Double3(0, 0, 1) * Rotation; }

Double3 Airship::GetRight() { return Double3(1, 0, 0) * Rotation; }

Double3 Airship::GetUp() { return Double3(0, 1, 0) * Rotation; }

string Airship::GetNextID()
{
	_currentAirshipIndex++;
	return "airship_" + std::to_string(_currentAirshipIndex);
}