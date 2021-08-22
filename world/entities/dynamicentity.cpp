// File: dynamicentity.cpp
// Purpose: Implements dynamicentity.h

#include "dynamicentity.h"
#include "../physics/shapes/sphereshape.h"

DynamicEntity::DynamicEntity() { }

DynamicEntity::DynamicEntity(string id) : PhysicsEntity(id) { }

DynamicEntity::~DynamicEntity()
{
	space->UnregisterEntity(this);
	for (Constraint* c : constraints)
	{
		delete c;
	}
}

Movement::Movement(Double3 fromPosition, Double3 toPosition, Quaternion fromRotation, Quaternion toRotation)
{
	this->fromPosition = fromPosition;
	this->toPosition = toPosition;
	this->fromRotation = fromRotation;
	this->toRotation = toRotation;
}

void DynamicEntity::AddForce(const Double3& force)
{
	// Buffer the force and apply it later when we know the delta time
	bufferedForce += force / mass;
}

void DynamicEntity::AddImpulseForce(const Double3& force)
{
	bufferedImpulseForce += force / mass;
}

void DynamicEntity::AddTorque(const Double3& torque)
{
	// Buffer the torque and apply it later when we know the delta time
	bufferedTorque += torque / mass;
}

void DynamicEntity::AddTorque(const Quaternion& torque)
{
	AddTorque(torque.toEuler());
}

void DynamicEntity::AddImpulseTorque(const Double3& torque)
{
	bufferedImpulseTorque += torque / mass;
}

void DynamicEntity::AddForceAtPosition(const Double3& force, const Double3& position)
{
	Double3 delta = position - this->position;
	AddTorque(cross(force, delta) * -PI);
	AddForce(force.normalized() * abs(dot(force, delta.normalized())));
}

void DynamicEntity::AddImpulseForceAtPosition(const Double3& force, const Double3& position)
{
	Double3 delta = position - this->position;
	AddImpulseTorque(cross(force, delta) * -PI);
	AddImpulseForce(force.normalized() * abs(dot(force, delta.normalized())));
}

Double3 DynamicEntity::GetReflectedForce(const Double3& force, const Double3& normal)
{
	return lerp(force, reflect(force, normal), (bounciness + 1) * 0.5f);
}

void DynamicEntity::RunTick(float dt)
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

	Movement movement = Movement(position, position + velocity * dt, rotation, Quaternion::fromEuler(torque * dt) * rotation);

	HandleConstraints(movement);

	// Apply position / rotation change 
	SetLocalPosition(movement.toPosition);
	SetLocalRotation(movement.toRotation);

	CheckCollision();
}

void DynamicEntity::CheckCollision()
{
	// Collide with other bodies
	for(PhysicsEntity* entity2 : space->entities)
	{
		if(this == entity2 || entity2 == NULL)
		{
			continue;
		}
		
		// Not the same object, check for collision
		Matrix4x4 matrix1 = this->GetTransformMatrix();
		Matrix4x4 matrix2 = entity2->GetTransformMatrix();

		for(Shape* s1 : this->colliders)
		{
			for(Shape* s2 : entity2->colliders)
			{
				// Check collision type
				if(s1->type == ShapeType::Sphere && s2->type == ShapeType::Sphere)
				{
					// Check collision
					SphereShape* sphere1 = (SphereShape*)s1;
					SphereShape* sphere2 = (SphereShape*)s2;

					Double3 pos1 = sphere1->position * matrix1;
					Double3 pos2 = sphere2->position * matrix2;

					Double3 delta = pos2 - pos1;

					Double3 size1 = sphere1->size * sphere1->rotation;
					Double3 size2 = sphere2->size * sphere2->rotation;

					if(delta.magnitude() < size1.x + size2.x)
					{
						// Colliding, handle displacement
						Double3 pushDirection = delta.magnitudeSquared() == 0 ? Double3(0, 1, 0) : delta.normalized();
						
						float offset = (size1.x + size2.x) - delta.magnitude();
						offset += 0.001f;

						Double3 displacement = pushDirection * offset;

						Double3 contactPoint = pos1 + (delta.normalized() * size1.x);

						DynamicEntity* dynamicEntity2 = dynamic_cast<DynamicEntity*>(entity2);
						if (dynamicEntity2 != nullptr) 
						{ 
							// Cast succeeded, we have a dynamic entity

							float massRatio = this->mass / dynamicEntity2->mass;
							float displacementRatio = 1 / (massRatio + 1); // :D

							this->position -= displacement * displacementRatio;
							entity2->position += displacement * (1 - displacementRatio);

							// Apply forces
							Double3 relativeVelocity = dynamicEntity2->velocity - this->velocity;

							this->AddImpulseForceAtPosition(relativeVelocity * dot(relativeVelocity.normalized(), pushDirection * -1) / massRatio, contactPoint);
							dynamicEntity2->AddImpulseForceAtPosition(relativeVelocity * dot(relativeVelocity.normalized(), pushDirection) * massRatio, contactPoint);
						}
						else
						{
							// Colliding with static object so assume infinite mass :)
							this->position -= displacement;
							this->AddImpulseForceAtPosition(velocity * dot(velocity.normalized(), pushDirection * -1), contactPoint);
						}
					}
				}
			}
		}
	}
}

void DynamicEntity::HandleConstraints(Movement& movement)
{
	for(Constraint* c : constraints)
	{
		c->ApplyConstraint(this, &movement);
	}
}