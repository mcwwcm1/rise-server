// File: BalloonComponent.h
// Purpose: defines the balloon component of an airship

#pragma once

#include "RootComponent.h"
#include "Interfaces/Damageable.h"
#include <vector>

class BalloonComponent : public RootComponent
{
 public:
	BalloonComponent(std::string name, ComponentType type, std::vector<ComponentSlot> slots, btCompoundShape collisionShape) : RootComponent(name, type, slots, collisionShape){};

	void GetProblems(std::vector<std::string>& problems) override;
};