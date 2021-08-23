// File: airship.cpp
// Purpose: Implements airship.h

#include "airship.h"

size_t Airship::_currentAirshipIndex = 0;  // Dumb.

Airship::Airship(std::string id) : Airship(id, Double3(0, 0, 0), Quaternion::identity) {}

Airship::Airship(std::string id, Double3 position, Quaternion rotation) : DynamicEntity(id, position, rotation)
{
	// HARDCODED COLLIDER SHAPE = BAD
	btSphereShape* sphereShape = new btSphereShape(2);

	Shape->addChildShape(btTransform(btQuaternion(0, 0, 0), btVector3(0, 0, 2.5)), sphereShape);
	Shape->addChildShape(btTransform(btQuaternion(0, 0, 0), btVector3(0, 0, 0.5)), sphereShape);
	Shape->addChildShape(btTransform(btQuaternion(0, 0, 0), btVector3(0, 0, -2)), sphereShape);

	Throttle = 0;
	Pitch    = 0;
	Yaw      = 0;
}

void Airship::RunTick(float dt)
{
	Double3 forward, right, up;

	forward = GetForward();
	right   = GetRight();
	up      = GetUp();

	float speed = RigidBody->getLinearVelocity().length();

	RigidBody->activate();  // Activate the body because it likes to be innactive

	RigidBody->setGravity(btVector3(0, 0, 0));  // Dumb. Move it later

	// Apply thrust
	RigidBody->applyCentralForce(forward * Throttle * 35);

	// Apply yaw steering
	RigidBody->applyTorque(up * Yaw * speed * .15 * -1);

	// Apply pitch steering
	RigidBody->applyTorque(right * Pitch * speed * .15 * -1);

	// Self-righting
	RigidBody->applyTorque(Cross(Double3(0, -1, 0), up) * 3);

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