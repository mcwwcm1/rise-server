// File: entity.cpp
// Purpose: Implements entity.h

#include "entity.h"

Entity::Entity() {}

Entity::Entity(std::string id) : Entity(id, Double3(0, 0, 0), Quaternion::identity) {}

Entity::Entity(std::string id, Double3 position, Quaternion rotation)
{
	ID       = id;
	Position = position;
	Rotation = rotation;
}

void Entity::RunTick(float dt) {}

void Entity::SubmitChange(std::string field, std::string change)
{
	Dirty              = true;
	ChangeTable[field] = change;
}

void Entity::SetLocalPosition(const Double3& newPosition)
{
	Position = newPosition;
	SubmitChange("Position", newPosition.ToString());
}

void Entity::SetLocalRotation(const Quaternion& newRotation)
{
	Rotation = newRotation;
	SubmitChange("Rotation", newRotation.ToString());
}

Matrix4x4 Entity::GetTransformMatrix()
{
	return GetTRSMatrix(Position, Rotation, Double3(1, 1, 1));
}

Double3 Entity::LocalPointToGlobal(Double3 point)
{
	return point * GetTransformMatrix();
}

Quaternion Entity::LocalRotationToGlobal(Quaternion rotation)
{
	return rotation * this->Rotation;
}

Double3 Entity::LocalVectorToGlobal(Double3 vector)
{
	return vector * Rotation;
}