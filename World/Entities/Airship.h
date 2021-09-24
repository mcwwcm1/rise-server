// File: airship.h
// Purpose: An instance of an airship who's data will be synced to Neos

#pragma once

#include <memory>

#include "MysticMath/Matrix4x4.h"
#include "World/Entities/DynamicEntity.h"
#include "World/Physics/PhysicsSpace.h"
#include "World/Entities/Airship/VesselComponent.h"

class Airship : public DynamicEntity
{
 public:
	float Throttle, Pitch, Yaw = 0;
	std::shared_ptr<VesselComponent> RootComponent;

	Airship();
	Airship(Double3 position, Quaternion rotation);

	std::string virtual GetCreationCommand() override;

	virtual void RunTick(float dt) override;
	virtual void OnRegistered() override;

	btVector3 GetForward();
	btVector3 GetRight();
	btVector3 GetUp();
};