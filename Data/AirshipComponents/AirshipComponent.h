// File: AirshipComponent.h
// Purpose: The base type for airship components (Hulls, wings, engines, etc...)

#pragma once

#include "MysticMath/Double3.h"
#include "MysticMath/Quaternion.h"
#include "btBulletDynamicsCommon.h"
#include <vector>

class ComponentSlot;

enum ComponentType {
	SmallHull      = 100,
	SmallWing      = 101,
	SmallRudder    = 102,
	SmallBalloon   = 103,
	SmallEngine    = 104,
	SmallPropeller = 105,

	MediumHull      = 200,
	MediumWing      = 201,
	MediumRudder    = 202,
	MediumBalloon   = 203,
	MediumEngine    = 204,
	MediumPropeller = 205,

	LargeHull      = 300,
	LargeWing      = 301,
	LargeRudder    = 302,
	LargeBalloon   = 303,
	LargeEngine    = 304,
	LargePropeller = 305,

	GridAttachment = 1000
};

class AirshipComponent
{
 public:
	AirshipComponent(){};
	AirshipComponent(std::string name, ComponentType type, std::vector<ComponentSlot> slots, btCompoundShape collisionShape) : Name(name), Type(type), Slots(slots), CollisionShape(collisionShape){};

	std::string Name;

	ComponentType Type;
	std::vector<ComponentSlot> Slots;
	btCompoundShape CollisionShape;

	bool IsValid();
	virtual void GetProblems(std::vector<std::string>& problems);
};