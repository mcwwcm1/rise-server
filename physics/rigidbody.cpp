// File: rigidbody.cpp
// Purpose: Implements rigidbody.h

#include "rigidbody.h"
#include "shapes/sphereshape.h"

Rigidbody::Rigidbody() 
{
	SphereShape* s1 = new SphereShape(2);
	s1->position = Double3(0, 0, -2.5);
	colliders.push_back(s1);

	SphereShape* s2 = new SphereShape(2);
	s2->position = Double3(0, 0, -0.5);
	colliders.push_back(s2);

	SphereShape* s3 = new SphereShape(2);
	s3->position = Double3(0, 0, 1.5);
	colliders.push_back(s3);
}

Matrix4x4 Rigidbody::GetTransformMatrix()
{
	return getTRSMatrix(position, rotation, scale);
}

void Rigidbody::AddForce(const Double3& force)
{
	// Buffer the force and apply it later when we know the delta time
	bufferedForce += force / mass;
}

void Rigidbody::AddImpulseForce(const Double3& force)
{
	bufferedImpulseForce += force / mass;
}

void Rigidbody::AddTorque(const Double3& torque)
{
	// Buffer the torque and apply it later when we know the delta time
	bufferedTorque += torque / mass;
}

void Rigidbody::AddTorque(const Quaternion& torque)
{
	AddTorque(torque.toEuler());
}

void Rigidbody::AddImpulseTorque(const Double3& torque)
{
	bufferedImpulseTorque += torque / mass;
}

void Rigidbody::AddForceAtPosition(const Double3& force, const Double3& position)
{
	Double3 delta = position - this->position;
	AddTorque(cross(force, delta) * -PI);
	AddForce(force.normalized() * abs(dot(force, delta.normalized())));
}

void Rigidbody::AddImpulseForceAtPosition(const Double3& force, const Double3& position)
{
	Double3 delta = position - this->position;
	AddImpulseTorque(cross(force, delta) * -PI);
	AddImpulseForce(force.normalized() * abs(dot(force, delta.normalized())));
}

Double3 Rigidbody::GetReflectedForce(const Double3& force, const Double3& normal)
{
	return lerp(force, reflect(force, normal), (bounciness + 1) * 0.5f);
}


void Rigidbody::RunTick(float dt)
{
	// Apply drag
	AddForce(velocity * drag * -1 * mass);
	AddTorque(torque * rotationalDrag * -1 * mass);

	// Apply buffered forces
	velocity += bufferedImpulseForce + bufferedForce * dt;
	torque += bufferedImpulseTorque + bufferedTorque * dt;

	// Clear buffered forces
	bufferedForce = Double3();
	bufferedTorque = Double3();
	bufferedImpulseForce = Double3();
	bufferedImpulseTorque = Double3();

	// Apply position / rotation change 
	position = position + velocity * dt;
	rotation = Quaternion::fromEuler(torque * dt) * rotation;
}