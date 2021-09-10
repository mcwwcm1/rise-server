// File: BalloonComponent.h
// Purpose: Implements BalloonComponent.h

#include "BalloonComponent.h"
#include "Slots/ComponentSlot.h"

void BalloonComponent::GetProblems(std::vector<std::string>& problems)
{
	for (auto s : Slots) {
		if (s.Required && s.Components.size() == 0)
			problems.push_back("Missing required component");
	}
}