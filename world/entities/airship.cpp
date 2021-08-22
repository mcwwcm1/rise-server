// File: airship.cpp
// Purpose: Implements airship.h

#include "airship.h"

#include "../physics/constraints/distanceconstraint.h"
#include "../physics/shapes/sphereshape.h"

size_t Airship::currentAirshipIndex = 0;  // Dumb.

Airship::Airship(string id) : DynamicEntity(id)
{
  // HARDCODED COLLIDER SHAPE = BAD
  SphereShape* s1 = new SphereShape(2);
  s1->position    = Double3(0, 0, -2.5);
  colliders.push_back(s1);

  SphereShape* s2 = new SphereShape(2);
  s2->position    = Double3(0, 0, -0.5);
  colliders.push_back(s2);

  SphereShape* s3 = new SphereShape(2);
  s3->position    = Double3(0, 0, 1.5);
  colliders.push_back(s3);

  throttle = 0;
  pitch    = 0;
  yaw      = 0;
  drag     = 0.5f;
}

Matrix4x4 Airship::GetTransformMatrix()
{
  return getTRSMatrix(position, rotation, scale);
}

Double3 Airship::GetPosition() { return position; }
Quaternion Airship::GetRotation() { return rotation; }
Double3 Airship::GetScale() { return scale; }

void Airship::RunTick(float dt)
{
  Double3 forward, right, up;

  forward = GetForward();
  right   = GetRight();
  up      = GetUp();

  float speed = velocity.magnitude();

  // Apply thrust
  AddForce(forward * throttle * 30);

  // Apply yaw steering
  AddTorque(up * yaw * speed * -70);

  // Apply pitch steering
  AddTorque(right * pitch * speed * 30);

  // Self-righting
  AddTorque(cross(Double3(0, -1, 0), up) * 500);

  DynamicEntity::RunTick(dt);
}

Double3 Airship::GetForward() { return Double3(0, 0, 1) * rotation; }

Double3 Airship::GetRight() { return Double3(1, 0, 0) * rotation; }

Double3 Airship::GetUp() { return Double3(0, 1, 0) * rotation; }

string Airship::GetNextID()
{
  currentAirshipIndex++;
  return "airship_" + std::to_string(currentAirshipIndex);
}