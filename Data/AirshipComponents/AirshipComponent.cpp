// File: AirshipComponent.cpp
// Purpose: Implements AirshipComponent.h

#include "AirshipComponent.h"
#include "Slots/ComponentSlot.h"

bool AirshipComponent::IsValid()
{
	std::vector<std::string> problems;
	GetProblems(problems);
	return problems.size() == 0;
}

void AirshipComponent::GetProblems(std::vector<std::string>& problems) {}