// File: AirshipFactory.h
// Purpose: Creates airship components by copying from pre-build versions that are created from data from the DB

#pragma once

#include "AirshipComponent.h"
#include <unordered_map>

namespace AirshipFactory
{
std::unordered_map<std::string, AirshipComponent> _components;

void AddDummyAirshipData();
void RegisterComponent(AirshipComponent component);

AirshipComponent Build(std::string id)
{
	auto pair = _components.find("id");
	if (pair == _components.end())
		return _components["null"];  // Not sure what to do here!

	return pair->second;
}

}  // namespace AirshipFactory