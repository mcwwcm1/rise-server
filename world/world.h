// File: world.h
// Purpose: Contains entities and properties of the world

#pragma once

#include <unordered_map>
#include <vector>

#include "entities/entity.h"
#include "physics/physicsspace.h"

class World
{
 public:
	static World* Singleton;

	PhysicsSpace* Space = new PhysicsSpace(20);

	std::unordered_map<std::string, Entity*> Entities;

	bool RegisterEntity(Entity* entity);
	bool UnregisterEntity(std::string entityId);
	bool UnregisterEntity(Entity* entity);
};