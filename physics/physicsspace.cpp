// File: physicsspace.cpp
// Purpose: Implements physicsspace.h

#include "physicsspace.h"
#include "shapes/sphereshape.h"
//#include "../core/send.h"

PhysicsSpace::PhysicsSpace() { PhysicsSpace(0); }

PhysicsSpace::PhysicsSpace(double updateRate)
{
	this->updateRate = updateRate;
	this->fixedDT = 1 / updateRate;
}

void PhysicsSpace::RegisterBody(Rigidbody* rigidbody)
{
	bodies.push_back(rigidbody);
}

void PhysicsSpace::RegisterStaticCollider(Shape* collider)
{
	staticColliders.push_back(collider);
}

void PhysicsSpace::RunTick()
{
	for(Rigidbody* body : bodies)
		body->RunTick(fixedDT);
	
	CheckCollision();
}

void PhysicsSpace::CheckCollision()
{
	for(Rigidbody* rb1 : bodies)
	{
		// Collide with other bodies
		for(Rigidbody* rb2 : bodies)
		{
			if(rb1 == rb2 || rb1 == NULL || rb2 == NULL)
			{
				continue;
			}
			
			// Not the same object, check for collision
			Matrix4x4 matrix1 = rb1->GetTransformMatrix();
			Matrix4x4 matrix2 = rb2->GetTransformMatrix();

			for(Shape* s1 : rb1->colliders)
			{
				for(Shape* s2 : rb2->colliders)
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

							Double3 displacement = pushDirection * offset * -0.5f;

							rb1->position += displacement;
							rb2->position -= displacement;

							Double3 contactPoint = pos1 + (delta.normalized() * size1.x);

							// Apply forces
							Double3 relativeVelocity = rb2->velocity - rb1->velocity;
							rb1->AddImpulseForceAtPosition(relativeVelocity * dot(relativeVelocity.normalized(), pushDirection * -1), contactPoint);
							rb2->AddImpulseForceAtPosition(relativeVelocity * dot(relativeVelocity.normalized(), pushDirection), contactPoint);
						}
					}
				}
			}
		}

		for(Shape* collider : rb1->colliders)
		{
			for(Shape* other : staticColliders)
			{
				// Sphere-sphere
				if(collider->type == ShapeType::Sphere && other->type == ShapeType::Sphere)
				{
					// Check collision
						SphereShape* sphere1 = (SphereShape*)collider;
						SphereShape* sphere2 = (SphereShape*)other;

						Double3 pos1 = sphere1->position * rb1->GetTransformMatrix();

						Double3 delta = other->position - pos1;

						Double3 size1 = sphere1->size * sphere1->rotation;
						Double3 size2 = sphere2->size * sphere2->rotation;

						if(delta.magnitude() < size1.x + size2.x)
						{
							// Colliding, handle displacement
							Double3 pushDirection = delta.magnitudeSquared() == 0 ? Double3(0, 1, 0) : delta.normalized();
							
							float offset = (size1.x + size2.x) - delta.magnitude();
							offset += 0.001f;

							Double3 displacement = pushDirection * offset * -1;

							rb1->position += displacement;

							Double3 contactPoint = pos1 + (delta.normalized() * size1.x);

							// Apply forces
							rb1->AddImpulseForce(rb1->velocity * -1);
							rb1->AddImpulseForceAtPosition(rb1->GetReflectedForce(rb1->velocity, pushDirection * -1), contactPoint);
						}
				}
			}
		}
	}
}