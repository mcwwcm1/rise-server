// File: dynamicentity.h
// Purpose: A PhysicsEntity that has a rigidbody

#include "entity.h"
#include "physicsentity.h"
#include "../physics/shapes/shape.h"
#include "../physics/constraints/constraint.h"
#include "../physics/physicsspace.h"

class DynamicEntity : public PhysicsEntity
{
	private:
		Double3 bufferedForce = Double3(0, 0, 0);
		Double3 bufferedTorque = Double3(0, 0, 0);
		Double3 bufferedImpulseForce = Double3(0, 0, 0);
		Double3 bufferedImpulseTorque = Double3(0, 0, 0);

	public:
		DynamicEntity();
		DynamicEntity(PhysicsSpace* space);
		virtual ~DynamicEntity();

		Double3 velocity = Double3(0, 0, 0);
		Double3 torque = Double3(0, 0, 0);
		float drag = 1;
		float rotationalDrag = 1;
		float bounciness = 0.2f;
		float friction = 0.2f;
		float mass = 10.0f;

		std::vector<Constraint*> constraints;

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

struct Movement
{
	public:
		Movement(Double3 fromPosition, Double3 toPosition, Quaternion fromRotation, Quaternion toRotation);

		Double3 fromPosition;
		Double3 toPosition;
		Quaternion fromRotation;
		Quaternion toRotation;
};