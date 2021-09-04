// File: HullComponent.h
// Purpose: defines the hull component of an airship

#include "hullcomponent.h"

void HullComponent::DealDamage(float damage)
{
	Health -= damage;
	if (Health <= 0)
		Health = 0;
}