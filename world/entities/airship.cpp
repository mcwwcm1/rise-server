// File: airship.cpp
// Purpose: Implements airship.h

#include "airship.h"

Airship::Airship() : Airship::Airship(Double3(0, 0, 0), Quaternion::identity) {}

Airship::Airship(Double3 position, Quaternion rotation) : DynamicEntity(position, rotation, 10)
{
	// HARDCODED COLLIDER SHAPE = BAD
	btSphereShape* sphereShape = new btSphereShape(2);

	Shape->addChildShape(btTransform(btQuaternion(0, 0, 0), btVector3(0, 0, 2.5)), sphereShape);
	Shape->addChildShape(btTransform(btQuaternion(0, 0, 0), btVector3(0, 0, 0.5)), sphereShape);
	Shape->addChildShape(btTransform(btQuaternion(0, 0, 0), btVector3(0, 0, -2)), sphereShape);

	btVector3 inertia;
	Shape->calculateLocalInertia(RigidBody->getMass(), inertia);
	RigidBody->setMassProps(RigidBody->getMass(), inertia);

	Throttle = 0;
	Pitch    = 0;
	Yaw      = 0;
}

std::string Airship::GetCreationCommand()
{
	return "SpawnEntity StarterAirship|" + ID + "|";
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
	RigidBody->applyCentralForce(forward * Throttle * 20);

	// Apply yaw steering
	RigidBody->applyTorque(up * Dot(Yaw, speed) * -5);

	// Apply pitch steering
	RigidBody->applyTorque(right * Dot(Pitch, speed) * 5);

	// Self-righting
	RigidBody->applyTorque(Cross(Double3(0, -1, 0), up) * 200);
}

btVector3 Airship::GetForward()
{
	return Double3(0, 0, 1) * Rotation;
}

btVector3 Airship::GetUp()
{
	return Double3(0, 1, 0) * Rotation;
}

btVector3 Airship::GetRight()
{
	return Double3(1, 0, 0) * Rotation;
}
