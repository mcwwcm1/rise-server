// File: dynamicentity.h
// Purpose: A PhysicsEntity that has a rigidbody

#pragma once

#include "../physics/constraints/constraint.h"
#include "../physics/physicsspace.h"
#include "../physics/shapes/shape.h"
#include "entity.h"
#include "physicsentity.h"

class DynamicEntity : public PhysicsEntity
{
 private:
	Double3 _bufferedForce         = Double3(0, 0, 0);
	Double3 _bufferedTorque        = Double3(0, 0, 0);
	Double3 _bufferedImpulseForce  = Double3(0, 0, 0);
	Double3 _bufferedImpulseTorque = Double3(0, 0, 0);

 public:
	DynamicEntity();
	DynamicEntity(std::string id);
	virtual ~DynamicEntity();

	Double3 Velocity     = Double3(0, 0, 0);
	Double3 Torque       = Double3(0, 0, 0);
	float Drag           = 1;
	float RotationalDrag = 1;
	float Bounciness     = 0.2f;
	float Friction       = 0.2f;
	float Mass           = 10.0f;

	std::vector<Constraint*> Constraints;

	// Apply forces
	void AddForce(const Double3& force);
	void AddImpulseForce(const Double3& force);

	void AddTorque(const Double3& torque);
	void AddTorque(const Quaternion& torque);
	void AddImpulseTorque(const Double3& torque);

	void AddForceAtPosition(const Double3& force, const Double3& position);
	void AddImpulseForceAtPosition(const Double3& force, const Double3& position);
	Double3 GetReflectedForce(const Double3& force, const Double3& normal);

	virtual void RunTick(float dt);
	void CheckCollision();
	void HandleConstraints(Movement& movement);
};

struct Movement {
 public:
	Movement(Double3 fromPosition,
	         Double3 toPosition,
	         Quaternion fromRotation,
	         Quaternion toRotation);

	Double3 FromPosition;
	Double3 ToPosition;
	Quaternion FromRotation;
	Quaternion ToRotation;
};