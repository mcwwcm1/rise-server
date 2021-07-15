// File: rigidbody.cpp
// Purpose: Implements rigidbody.h

#include "rigidbody.h"

Rigidbody::Rigidbody() {}

void Rigidbody::AddForce(const Double3& force)
{
	// Buffer the force and apply it later when we know the delta time
	bufferedForce += force;
}

void Rigidbody::AddImpulseForce(const Double3& force)
{
	velocity += force;
}

void Rigidbody::AddTorque(const Double3& torque)
{
	// Buffer the torque and apply it later when we know the delta time
	bufferedTorque += torque;
}

void Rigidbody::AddTorque(const Quaternion& torque)
{
	AddTorque(torque.toEuler());
}

void Rigidbody::AddImpulseTorque(const Double3& torque)
{
	this->torque += torque;
}

void Rigidbody::RunTick(float dt)
{
	// Apply drag
	AddForce(velocity * drag * -1);
	AddTorque(torque * rotationalDrag * -1);

	// Apply buffered forces
	AddImpulseForce(bufferedForce * dt);
	AddImpulseTorque(bufferedTorque * dt);

	// Clear buffered forces
	bufferedForce = Double3();
	bufferedTorque = Double3();

	// Apply position / rotation change 
	position += velocity * dt;
	rotation = Quaternion::fromEuler(torque * dt) * rotation;
}