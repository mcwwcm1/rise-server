// File: world.h
// Purpose: Contains entities and properties of the world

#pragma once

#include <unordered_map>
#include <vector>

#include "Entities/Entity.h"
#include "Entities/UserEntity.h"
#include "Physics/PhysicsSpace.h"
#include "World/Distributors/BugSwarmDistributor.h"

class World
{
 private:
	long unsigned long static currentEntityIndex;  // :)
	clock_t static lastSaveTime;
	bool initialized = false;

 public:
	static World* Singleton;

	PhysicsSpace* Space = new PhysicsSpace(20);

	std::unordered_map<std::string, UserEntity*> Users;
	std::unordered_map<std::string, Entity*> Entities;
	std::vector<Distributor*> Distributors;

	World();

	void RunTick();

	static std::string GetNextID();

	bool RegisterEntity(Entity* entity);
	bool UnregisterEntity(std::string entityId);
	bool UnregisterEntity(Entity* entity);
	Entity* GetEntity(std::string entityId);
	bool HasEntity(std::string entityId);
};