// File: world.h
// Purpose: Contains entities and properties of the world

#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <vector>

#include "entities/entity.h"
#include "physics/physicsspace.h"

class World
{
 public:
	static World* Singleton;

	PhysicsSpace* Space = new PhysicsSpace(20);

	unordered_map<string, Entity*> Entities;

	bool RegisterEntity(Entity* entity);
	bool UnregisterEntity(string entityId);
	bool UnregisterEntity(Entity* entity);
};

#endif