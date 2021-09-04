// File: distributor.cpp
// Purpose: Implements distributor.h

#include "Distributor.h"
#include "World/World.h"

void Distributor::CleanupDistant(std::vector<Double3> positions)
{
	std::vector<Entity*> dueForCleanup;
	dueForCleanup.reserve(Entities.size());  // To avoid memory re-allocation

	for (auto e : Entities) {
		bool inBounds = false;
		for (Double3 p : positions) {
			if ((e->Position - p).MagnitudeSquared() < Range * Range) {
				inBounds = true;
				break;
			}
		}
		if (!inBounds)
			dueForCleanup.push_back(e);
	}

	for (auto e : dueForCleanup)
		DestroyEntity(e);
}

void Distributor::AddEntity(Entity* entity)
{
	Entities.push_back(entity);
	World::Singleton->RegisterEntity(entity);
}

void Distributor::DestroyEntity(Entity* entity)
{
	auto e = std::find(Entities.begin(), Entities.end(), entity);
	if (e != Entities.end()) {
		delete *e;
		Entities.erase(e);
	}
}