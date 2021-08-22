// File: entity.cpp
// Purpose: Implements entity.h

#include "entity.h"

Entity::Entity() {}

Entity::Entity(string id) { this->ID = id; }

void Entity::SubmitChange(string field, string change)
{
	Dirty              = true;
	ChangeTable[field] = change;
}

void Entity::SetLocalPosition(const Double3& newPosition)
{
	this->Position = newPosition;
	SubmitChange("position", newPosition.ToString());
}

void Entity::SetLocalRotation(const Quaternion& newRotation)
{
	this->Rotation = newRotation;
	SubmitChange("rotation", newRotation.ToString());
}

void Entity::SetLocalScale(const Double3& newScale)
{
	this->Scale = newScale;
	SubmitChange("scale", newScale.ToString());
}

Matrix4x4 Entity::GetTransformMatrix()
{
	return GetTRSMatrix(Position, Rotation, Scale);
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
	return (vector * Rotation) * Scale;
}