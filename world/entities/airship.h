// File: airship.h
// Purpose: An instance of an airship who's data will be synced to Neos

#ifndef AIRSHIP_H
#define AIRSHIP_H

#include "../../mysticmath/matrix4x4.h"
#include "../physics/physicsspace.h"
#include "../entities/dynamicentity.h"

class Airship : public DynamicEntity
{
	private:
		static size_t currentAirshipIndex;

	public:
		float throttle, pitch, yaw = 0;

		Airship();
		Airship(string id);

		Matrix4x4 GetTransformMatrix();
		Double3 GetPosition();
		Quaternion GetRotation();
		Double3 GetScale();

		void RunTick(float dt);

		Double3 GetForward();
		Double3 GetRight();
		Double3 GetUp();

		static string GetNextID(); // Provides the next airship id
};

#endif