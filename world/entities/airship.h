// File: airship.h
// Purpose: An instance of an airship who's data will be synced to Neos

#pragma once

#include "../../mysticmath/matrix4x4.h"
#include "../entities/dynamicentity.h"
#include "../physics/physicsspace.h"

class Airship : public DynamicEntity
{
 private:
	static size_t _currentAirshipIndex;

 public:
	float Throttle, Pitch, Yaw = 0;

	Airship();
	Airship(std::string id);
	Airship(std::string id, Double3 position, Quaternion rotation);

	void RunTick(float dt);

	btVector3 GetForward();
	btVector3 GetRight();
	btVector3 GetUp();

	static std::string GetNextID();  // Provides the next airship id
};