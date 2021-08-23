// File: distributor.cpp
// Purpose: Implements distributor.h

#include "distributor.h"
#include "world/world.h"

void Distributor::CleanupDistant(std::vector<Double3> positions)
{
	for (auto it = Entities.rend(); it != Entities.rbegin(); ++it) {
		for (Double3 p : positions) {
			if (((*it)->Position - p).MagnitudeSquared() > Range * Range) {
				DestroyEntity(*it);
			}
		}
	}
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