// File: entity.cpp
// Purpose: Implements entity.h

#include "entity.h"
#include "world/world.h"

Entity::Entity() : Entity(Double3(0, 0, 0), Quaternion::identity) {}

Entity::Entity(Double3 position, Quaternion rotation)
{
	ID       = World::GetNextID();
	Position = position;
	Rotation = rotation;
}

Entity::~Entity()
{
	World::Singleton->UnregisterEntity(this);
}

std::string Entity::GetDestructionCommand()
{
	return "DestroyEntity " + ID + "|";
}

void Entity::RunTick(float dt) {}

void Entity::SubmitChange(std::string field, std::string change, bool override)
{
	Dirty = true;
	if (override)
		ChangeTable[field] = {change};
	else
		ChangeTable[field].push_back(change);
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

Double3 Entity::LocalPointToGlobal(Double3 point) { return point * GetTransformMatrix(); }
Quaternion Entity::LocalRotationToGlobal(Quaternion rotation) { return rotation * Rotation; }
Double3 Entity::LocalVectorToGlobal(Double3 vector) { return vector * Rotation; }

Double3 Entity::GlobalPointToLocal(Double3 point) { return point - Position * Rotation.Conjugate(); }
Quaternion Entity::GlobalRotationToLocal(Quaternion rotation) { return rotation * Rotation.Conjugate(); }
Double3 Entity::GlobalVectorToLocal(Double3 vector) { return vector * Rotation.Conjugate(); }