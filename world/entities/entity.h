// File: entity.h
// Purpose: a thing in the world that has a location

#pragma once

#include <unordered_map>

#include "../../mysticmath/double3.h"
#include "../../mysticmath/matrix4x4.h"
#include "../../mysticmath/quaternion.h"

class Entity
{
 public:
	Entity();
	Entity(string id);
	virtual ~Entity() = default;

	string id;
	string* owner;
	unordered_map<string, string> changeTable;
	bool dirty;

	void SubmitChange(string field, string change);

	void SetLocalPosition(const Double3& newPosition);
	void SetLocalRotation(const Quaternion& newRotation);
	void SetLocalScale(const Double3& scale);

	Double3 position    = Double3(0, 0, 0);
	Quaternion rotation = Quaternion::identity;
	Double3 scale       = Double3(1, 1, 1);

	Matrix4x4 GetTransformMatrix();
	Double3 LocalPointToGlobal(Double3 point);
	Quaternion LocalRotationToGlobal(Quaternion rotation);
	Double3 LocalVectorToGlobal(Double3 vector);
};