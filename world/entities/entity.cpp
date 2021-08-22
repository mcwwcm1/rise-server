// File: entity.cpp
// Purpose: Implements entity.h

#include "entity.h"

Entity::Entity() {}

Entity::Entity(string id) { this->id = id; }

void Entity::SubmitChange(string field, string change)
{
	dirty              = true;
	changeTable[field] = change;
}

void Entity::SetLocalPosition(const Double3& newPosition)
{
	this->position = newPosition;
	SubmitChange("position", newPosition.str());
}

void Entity::SetLocalRotation(const Quaternion& newRotation)
{
	this->rotation = newRotation;
	SubmitChange("rotation", newRotation.str());
}

void Entity::SetLocalScale(const Double3& newScale)
{
	this->scale = newScale;
	SubmitChange("scale", newScale.str());
}

Matrix4x4 Entity::GetTransformMatrix()
{
	return getTRSMatrix(position, rotation, scale);
}

Double3 Entity::LocalPointToGlobal(Double3 point)
{
	return point * GetTransformMatrix();
}

Quaternion Entity::LocalRotationToGlobal(Quaternion rotation)
{
	return rotation * this->rotation;
}

Double3 Entity::LocalVectorToGlobal(Double3 vector)
{
	return (vector * rotation) * scale;
}