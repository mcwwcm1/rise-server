// File: world.h
// Purpose: Contains entities and properties of the world

#pragma once

#include <unordered_map>
#include <vector>

#include "entities/entity.h"
#include "physics/physicsspace.h"
#include "world/distributors/bugswarmdistributor.h"

class World
{
 private:
	long unsigned long static currentEntityIndex;  // :)

 public:
	static World* Singleton;

	PhysicsSpace* Space = new PhysicsSpace(20);

	std::unordered_map<std::string, Entity*> Entities;
	std::vector<Distributor*> Distributors;

	World();

	void RunTick();

	static std::string GetNextID();

	bool RegisterEntity(Entity* entity);
	bool UnregisterEntity(std::string entityId);
	bool UnregisterEntity(Entity* entity);
};