// File: HullComponent.h
// Purpose: defines the hull component of an airship

#pragma once

#include "RootComponent.h"
#include "Interfaces/Damageable.h"
#include <vector>

class HullComponent : public RootComponent, public Damageable
{
 public:
	HullComponent(std::string name, ComponentType type, std::vector<ComponentSlot> slots, btCompoundShape collisionShape, float maxHealth) : RootComponent(name, type, slots, collisionShape), MaxHealth(maxHealth), Health(maxHealth){};

	float MaxHealth = 0;
	float Health    = 0;

	void DealDamage(float damage) override;
	void GetProblems(std::vector<std::string>& problems) override;
};