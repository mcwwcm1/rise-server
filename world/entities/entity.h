// File: entity.h
// Purpose: a thing in the world that has a location

#pragma once

#include <unordered_map>
#include <optional>
#include <string>

#include <btBulletDynamicsCommon.h>

#include "../../mysticmath/double3.h"
#include "../../mysticmath/matrix4x4.h"
#include "../../mysticmath/quaternion.h"

class Entity
{
 public:
	Entity();
	Entity(Double3 position, Quaternion rotation);

	virtual ~Entity() = default;

	bool DontSync = false;

	std::string ID;
	std::optional<std::string> Owner;
	std::unordered_map<std::string, std::string> ChangeTable;
	bool Dirty = false;

	Double3 Position;
	Quaternion Rotation;

	virtual std::string GetCreationCommand() = 0;

	virtual void RunTick(float dt);

	void SetLocalPosition(const Double3& newPosition);
	void SetLocalRotation(const Quaternion& newRotation);

	void SubmitChange(std::string field, std::string change);

	Matrix4x4 GetTransformMatrix();
	Double3 LocalPointToGlobal(Double3 point);
	Quaternion LocalRotationToGlobal(Quaternion rotation);
	Double3 LocalVectorToGlobal(Double3 vector);
};