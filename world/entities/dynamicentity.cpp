// File: dynamicentity.cpp
// Purpose: Implements dynamicentity.h

#include "dynamicentity.h"

#include "../physics/shapes/sphereshape.h"

DynamicEntity::DynamicEntity() {}

DynamicEntity::DynamicEntity(std::string id) : PhysicsEntity(id) {}

DynamicEntity::~DynamicEntity()
{
	Space->UnregisterEntity(this);
	for (Constraint* c : Constraints) { delete c; }
}

Movement::Movement(Double3 fromPosition,
                   Double3 toPosition,
                   Quaternion fromRotation,
                   Quaternion toRotation)
{
	this->FromPosition = fromPosition;
	this->ToPosition   = toPosition;
	this->FromRotation = fromRotation;
	this->ToRotation   = toRotation;
}

void DynamicEntity::AddForce(const Double3& force)
{
	// Buffer the force and apply it later when we know the delta time
	_bufferedForce += force / Mass;
}

void DynamicEntity::AddImpulseForce(const Double3& force)
{
	_bufferedImpulseForce += force / Mass;
}

void DynamicEntity::AddTorque(const Double3& torque)
{
	// Buffer the torque and apply it later when we know the delta time
	_bufferedTorque += torque / Mass;
}

void DynamicEntity::AddTorque(const Quaternion& torque)
{
	AddTorque(torque.ToEuler());
}

void DynamicEntity::AddImpulseTorque(const Double3& torque)
{
	_bufferedImpulseTorque += torque / Mass;
}

void DynamicEntity::AddForceAtPosition(const Double3& force,
                                       const Double3& position)
{
	Double3 delta = position - this->Position;
	AddTorque(Cross(force, delta) * -PI);
	AddForce(force.Normalized() * abs(Dot(force, delta.Normalized())));
}

void DynamicEntity::AddImpulseForceAtPosition(const Double3& force,
                                              const Double3& position)
{
	Double3 delta = position - this->Position;
	AddImpulseTorque(Cross(force, delta) * -PI);
	AddImpulseForce(force.Normalized() * abs(Dot(force, delta.Normalized())));
}

Double3 DynamicEntity::GetReflectedForce(const Double3& force,
                                         const Double3& normal)
{
	return Lerp(force, Reflect(force, normal), (Bounciness + 1) * 0.5f);
}

void DynamicEntity::RunTick(float dt)
{
	// Apply drag
	AddForce(Velocity * Drag * -1 * Mass);
	AddTorque(Torque * RotationalDrag * -1 * Mass);

	// Apply buffered forces
	Velocity += _bufferedImpulseForce + _bufferedForce * dt;
	Torque += _bufferedImpulseTorque + _bufferedTorque * dt;

	// Clear buffered forces
	_bufferedForce         = Double3();
	_bufferedTorque        = Double3();
	_bufferedImpulseForce  = Double3();
	_bufferedImpulseTorque = Double3();

	Movement movement = Movement(Position,
	                             Position + Velocity * dt,
	                             Rotation,
	                             Quaternion::FromEuler(Torque * dt) * Rotation);

	HandleConstraints(movement);

	// Apply position / rotation change
	SetLocalPosition(movement.ToPosition);
	SetLocalRotation(movement.ToRotation);

	CheckCollision();
}

void DynamicEntity::CheckCollision()
{
	// Collide with other bodies
	for (PhysicsEntity* entity2 : Space->entities) {
		if (this == entity2 || entity2 == NULL) { continue; }

		// Not the same object, check for collision
		Matrix4x4 matrix1 = this->GetTransformMatrix();
		Matrix4x4 matrix2 = entity2->GetTransformMatrix();

		for (Shape* s1 : this->Colliders) {
			for (Shape* s2 : entity2->Colliders) {
				// Check collision type
				if (s1->Type == ShapeType::Sphere && s2->Type == ShapeType::Sphere) {
					// Check collision
					SphereShape* sphere1 = (SphereShape*) s1;
					SphereShape* sphere2 = (SphereShape*) s2;

					Double3 pos1 = sphere1->Position * matrix1;
					Double3 pos2 = sphere2->Position * matrix2;

					Double3 delta = pos2 - pos1;

					Double3 size1 = sphere1->size * sphere1->Rotation;
					Double3 size2 = sphere2->size * sphere2->Rotation;

					if (delta.Magnitude() < size1.x + size2.x) {
						// Colliding, handle displacement
						Double3 pushDirection = delta.MagnitudeSquared() == 0
						                            ? Double3(0, 1, 0)
						                            : delta.Normalized();

						float offset = (size1.x + size2.x) - delta.Magnitude();
						offset += 0.001f;

						Double3 displacement = pushDirection * offset;

						Double3 contactPoint = pos1 + (delta.Normalized() * size1.x);

						DynamicEntity* dynamicEntity2 =
								dynamic_cast<DynamicEntity*>(entity2);
						if (dynamicEntity2 != nullptr) {
							// Cast succeeded, we have a dynamic entity

							float massRatio         = this->Mass / dynamicEntity2->Mass;
							float displacementRatio = 1 / (massRatio + 1);  // :D

							this->Position -= displacement * displacementRatio;
							entity2->Position += displacement * (1 - displacementRatio);

							// Apply forces
							Double3 relativeVelocity =
									dynamicEntity2->Velocity - this->Velocity;

							this->AddImpulseForceAtPosition(
									relativeVelocity *
											Dot(relativeVelocity.Normalized(), pushDirection * -1) /
											massRatio,
									contactPoint);
							dynamicEntity2->AddImpulseForceAtPosition(
									relativeVelocity *
											Dot(relativeVelocity.Normalized(), pushDirection) *
											massRatio,
									contactPoint);
						} else {
							// Colliding with static object so assume infinite mass :)
							this->Position -= displacement;
							this->AddImpulseForceAtPosition(
									Velocity * Dot(Velocity.Normalized(), pushDirection * -1),
									contactPoint);
						}
					}
				}
			}
		}
	}
}

void DynamicEntity::HandleConstraints(Movement& movement)
{
	for (Constraint* c : Constraints) { c->ApplyConstraint(this, &movement); }
}