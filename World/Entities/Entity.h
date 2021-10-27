// File: entity.h
// Purpose: a thing in the world that has a location

#pragma once

#include <unordered_map>
#include <optional>
#include <string>
#include <vector>

#include <btBulletDynamicsCommon.h>

#include "MysticMath/Double3.h"
#include "MysticMath/Matrix4x4.h"
#include "MysticMath/Quaternion.h"

class Entity
{
 public:
	Entity();
	Entity(Double3 position, Quaternion rotation);
	virtual ~Entity();

	virtual void OnRegistered() {}

	bool DontSync = false;

	std::string ID;
	std::optional<std::string> Owner;
	std::vector<std::pair<std::string, std::string>> ChangeTable;
	bool Dirty = false;

	Double3 Position;
	Quaternion Rotation;

	virtual std::string GetCreationCommand() = 0;
	virtual std::string GetDestructionCommand();

	virtual void RunTick(float dt);

	void SetLocalPosition(const Double3& newPosition);
	void SetLocalRotation(const Quaternion& newRotation);

	void SubmitChange(std::string field, std::string change, bool override = true);

	Matrix4x4 GetTransformMatrix();
	Matrix4x4 GetInverseTransformMatrix();

	Double3 LocalPointToGlobal(Double3 point);
	Quaternion LocalRotationToGlobal(Quaternion rotation);
	Double3 LocalVectorToGlobal(Double3 vector);
	Double3 GlobalPointToLocal(Double3 point);
	Quaternion GlobalRotationToLocal(Quaternion rotation);
	Double3 GlobalVectorToLocal(Double3 point);
};