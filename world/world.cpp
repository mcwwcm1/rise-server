// File: world.cpp
// Purpose: Implements world.h

#include "world.h"
#include <bullet/btBulletDynamicsCommon.h>
#include "world/distributors/bugswarmdistributor.h"
#include "world/distributors/distributor.h"
#include "core/send.h"
#include "entities/structureentity.h"
#include <thread>
#include <chrono>

unsigned long long World::currentEntityIndex = 0;
clock_t World::lastSaveTime                  = 0;

World* World::Singleton = new World();

World::World()
{
	// Initialize a single structure entity to act as the starter island (Terrible solution)
	StructureEntity* starterIsland = new StructureEntity(Double3(0, 0, 0), Quaternion::identity);
	RegisterEntity(starterIsland);

	auto d = new BugSwarmDistributor(35, 2, 10, "wobbleFly");
	Distributors.push_back(d);
}

void World::RunTick()
{
	if (!initialized) {
		Send("CleanAll");
		initialized = true;
	}

	// Tick Entities
	for (auto entity : Entities)
		entity.second->RunTick(Space->FixedDT);

	// Tick physics
	Space->RunTick();

	// Tick distributors
	std::vector<Double3> positions;
	positions.reserve(Users.size());

	for (auto u : Users) {
		positions.push_back(u.second->Position);
	}

	for (Distributor* d : Distributors) {
		//d->CleanupDistant(positions);
		d->TryDistribute(positions);
	}

	// Handle change tables
	for (auto entity : Entities) {
		if (entity.second == nullptr) {
			printf("FOUND NULL ENTITY DURING WORLD TICK D:\n");
			continue;
		}
		if (entity.second->Dirty && !entity.second->DontSync) {
			std::string changes = "ChangeTable " + entity.second->ID + "|";
			for (auto change : entity.second->ChangeTable) {
				changes += change.first + "|" + change.second + "|";
			}

			Send(changes);
			entity.second->ChangeTable.clear();
			entity.second->Dirty = false;
		}
	}

	// Save data to DB if enough time has passed
}

std::string World::GetNextID()
{
	currentEntityIndex++;
	return "ent_" + std::to_string(currentEntityIndex);
}

bool World::RegisterEntity(Entity* entity)
{
	auto e = Entities.find(entity->ID);
	if (e != Entities.end()) {
		// Entity is already registered :(
		printf("Attempted to register entity that is already registered: %s\n",
		       entity->ID.c_str());
		return false;
	}

	Entities.insert({entity->ID, entity});

	// If the entity is a UserEntity, add them to Users
	UserEntity* ue = dynamic_cast<UserEntity*>(entity);
	if (ue != nullptr) { Users[ue->UserID] = ue; }

	// If the entity is a DynamicEntity, register it to the physics space
	DynamicEntity* de = dynamic_cast<DynamicEntity*>(entity);
	if (de != nullptr) { Space->RegisterEntity(de); }

	// Tell Neos that a new entity is in town
	if (!entity->DontSync) {
		Send(entity->GetCreationCommand());  // Send creation to headless
	}

	printf("Registered entity: %s\n", entity->ID.c_str());

	return true;
}

bool World::UnregisterEntity(std::string entityId)
{
	auto e = Entities.find(entityId);
	if (e == Entities.end()) return false;

	return UnregisterEntity(e->second);
}

bool World::UnregisterEntity(Entity* entity)
{
	auto e = Entities.find(entity->ID);
	if (e == Entities.end()) {
		// Entity is not registered :(
		printf("Attempted to unregister entity that is not registered: %s\n",
		       entity->ID.c_str());
		return false;
	}

	Entities.erase(e);

	// If the entity is a UserEntity, remove them from Users
	UserEntity* ue = dynamic_cast<UserEntity*>(entity);
	if (ue != nullptr) { Users.erase(ue->ID); }

	// If the entity is a DynamicEntity then unregister it from the physics space
	DynamicEntity* de = dynamic_cast<DynamicEntity*>(entity);
	if (de != nullptr) { Space->UnregisterEntity(de); }

	// Tell Neos that an entity is no longer in town
	if (!entity->DontSync) {
		Send(entity->GetDestructionCommand());  // Send creation to headless
	}
	printf("Unregistered entity: %s\n", entity->ID.c_str());

	return true;
}