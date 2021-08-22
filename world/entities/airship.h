// File: airship.h
// Purpose: An instance of an airship who's data will be synced to Neos

#ifndef AIRSHIP_H
#define AIRSHIP_H

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

	Matrix4x4 GetTransformMatrix();
	Double3 GetPosition();
	Quaternion GetRotation();
	Double3 GetScale();

	void RunTick(float dt);

	Double3 GetForward();
	Double3 GetRight();
	Double3 GetUp();

	static std::string GetNextID();  // Provides the next airship id
};

#endif