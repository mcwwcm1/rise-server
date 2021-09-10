// File: RootComponent.h
// Purpose: defines the base type for root airship components

#pragma once

#include "AirshipComponent.h"

class RootComponent : public AirshipComponent
{
 public:
	RootComponent(std::string name, ComponentType type, std::vector<ComponentSlot> slots, btCompoundShape collisionShape) : AirshipComponent(name, type, slots, collisionShape){};
};