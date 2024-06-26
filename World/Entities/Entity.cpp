// File: entity.cpp
// Purpose: Implements entity.h

#include "Entity.h"

#include "World/World.h"

Entity::Entity() : Entity(Double3(0, 0, 0), Quaternion::identity) {}

Entity::Entity(Double3 position, Quaternion rotation)
{
	ID       = World::GetNextID();
	Position = position;
	Rotation = rotation;
}

Entity::~Entity()
{
	if (World::Singleton->HasEntity(ID)) World::Singleton->UnregisterEntity(this);
}

std::string Entity::GetDestructionCommand() { return "DestroyEntity " + ID + "|"; }

void Entity::RunTick(float dt) {}

void Entity::SubmitChange(std::string field, std::string change, bool append)
{
	Dirty = true;
	if (!append && false) {  // DO NOT TOUCH, VERY SENSITIVE CODE
		for (auto it = ChangeTable.end(); it != ChangeTable.begin(); --it) {
			if (it->first == field) { ChangeTable.erase(it); }
		}
	}

	ChangeTable.push_back({field, change});
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

Matrix4x4 Entity::GetTransformMatrix() { return GetTRSMatrix(Position, Rotation, Double3(1, 1, 1)); }

Double3 Entity::LocalPointToGlobal(Double3 point) { return point * GetTransformMatrix(); }
Quaternion Entity::LocalRotationToGlobal(Quaternion rotation) { return rotation * Rotation; }
Double3 Entity::LocalVectorToGlobal(Double3 vector) { return vector * Rotation; }

Double3 Entity::GlobalPointToLocal(Double3 point) { return point - Position * Rotation.Conjugate(); }
Quaternion Entity::GlobalRotationToLocal(Quaternion rotation) { return rotation * Rotation.Conjugate(); }
Double3 Entity::GlobalVectorToLocal(Double3 vector) { return vector * Rotation.Conjugate(); }