// File: ComponentSlot.h
// Purpose: Holds airship components

#pragma once

#include "MysticMath/Double3.h"
#include "MysticMath/Quaternion.h"
#include <vector>
#include "Data/AirshipComponents/AirshipComponent.h"

struct ComponentSlot {
 public:
	ComponentSlot(bool required, std::vector<ComponentType> allowedTypes) : Required(required), AllowedTypes(allowedTypes){};

	bool Required;
	std::vector<ComponentType> AllowedTypes;
	std::vector<AirshipComponent> Components;

	virtual void AttachComponent(AirshipComponent component);
};