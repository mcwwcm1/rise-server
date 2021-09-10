// File: HullComponent.h
// Purpose: defines the hull component of an airship

#include "HullComponent.h"
#include "Slots/ComponentSlot.h"

void HullComponent::DealDamage(float damage)
{
	Health -= damage;
	if (Health <= 0)
		Health = 0;
}

void HullComponent::GetProblems(std::vector<std::string>& problems)
{
	if (Health <= 0)
		problems.push_back("Too damaged");

	for (auto s : Slots) {
		if (s.Required && s.Components.size() == 0)
			problems.push_back("Missing required component");
	}
}