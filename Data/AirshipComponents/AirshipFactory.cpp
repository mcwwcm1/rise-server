// File: AirshipFactory.cpp
// Purpose: Implements AirshipFactory.h

#include "AirshipFactory.h"
#include "Slots/ComponentSlot.h"

#include "HullComponent.h"
#include "BalloonComponent.h"

void AirshipFactory::AddDummyAirshipData()
{
	RegisterComponent(HullComponent("StarterHull",
	                                ComponentType::SmallHull,
	                                {ComponentSlot(true, {ComponentType::SmallBalloon})},
	                                btCompoundShape(),
	                                10));

	RegisterComponent(BalloonComponent("StarterBalloon",
	                                   ComponentType::SmallBalloon,
	                                   {},
	                                   btCompoundShape()));
}

void AirshipFactory::RegisterComponent(AirshipComponent component)
{
	auto pair = _components.find(component.Name);
	if (pair == _components.end()) {
		printf("Tried to register component but a component with that name is already registered: %s\n", component.Name.c_str());
		return;
	}

	_components[component.Name] = component;
}