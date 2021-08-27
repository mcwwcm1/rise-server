// File: airship.h
// Purpose: An instance of an airship who's data will be synced to Neos

#pragma once

#include "../../mysticmath/matrix4x4.h"
#include "../entities/dynamicentity.h"
#include "../physics/physicsspace.h"

class Airship : public DynamicEntity
{
 public:
	float Throttle, Pitch, Yaw = 0;

	Airship();
	Airship(Double3 position, Quaternion rotation);

	std::string virtual GetCreationCommand() override;

	virtual void RunTick(float dt) override;

	btVector3 GetForward();
	btVector3 GetRight();
	btVector3 GetUp();
};