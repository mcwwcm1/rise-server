// File: airship.cpp
// Purpose: Implements airship.h

#include "airship.h"

size_t Airship::_currentAirshipIndex = 0;  // Dumb.

Airship::Airship(std::string id) : Airship(id, Double3(0, 0, 0), Quaternion::identity) {}

Airship::Airship(std::string id, Double3 position, Quaternion rotation) : DynamicEntity(id, position, rotation)
{
	// HARDCODED COLLIDER SHAPE = BAD
	//SphereShape* s1 = new SphereShape(2);
	//s1->Position    = Double3(0, 0, -2.5);
	//Colliders.push_back(s1);
	//
	//SphereShape* s2 = new SphereShape(2);
	//s2->Position    = Double3(0, 0, -0.5);
	//Colliders.push_back(s2);
	//
	//SphereShape* s3 = new SphereShape(2);
	//s3->Position    = Double3(0, 0, 1.5);
	//Colliders.push_back(s3);

	Throttle = 0;
	Pitch    = 0;
	Yaw      = 0;
}

void Airship::RunTick(float dt)
{
	return;
	Double3 forward, right, up;

	forward = GetForward();
	right   = GetRight();
	up      = GetUp();

	float speed = RigidBody->getLinearVelocity().length();

	// Apply thrust
	RigidBody->applyCentralForce(forward * Throttle * 30);

	// Apply yaw steering
	Rigidbody->applyTorque(up * Yaw * speed * -70);

	// Apply pitch steering
	RigidBody->applyTorque(right * Pitch * speed * 30);

	// Self-righting
	RigidBody->applyTorque(Cross(Double3(0, -1, 0), up) * 500);

	DynamicEntity::RunTick(dt);
}

btVector3 Airship::GetForward() { return Double3(0, 0, 1) * Rotation; }

btVector3 Airship::GetUp() { return Double3(0, 1, 0) * Rotation; }

btVector3 Airship::GetRight() { return Double3(1, 0, 0) * Rotation; }

std::string Airship::GetNextID()
{
	_currentAirshipIndex++;
	return "airship_" + std::to_string(_currentAirshipIndex);
}