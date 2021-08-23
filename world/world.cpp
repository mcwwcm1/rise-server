// File: world.cpp
// Purpose: Implements world.h

#include "world.h"
#include <bullet/btBulletDynamicsCommon.h>
#include "world/distributors/bugswarmdistributor.h"
#include "world/distributors/distributor.h"
#include "core/send.h"

unsigned long long World::currentEntityIndex = 0;

World* World::Singleton = new World();

World::World()
{
	auto d = new BugSwarmDistributor(200, 2, 20, "WobbleFly");
	Distributors.push_back(d);
}

void World::RunTick()
{
	// Tick physics
	Space->RunTick();

	// Handle change tables
	for (auto entity : Entities) {
		if (entity.second == nullptr) {
			printf("FOUND NULL ENTITY D:\n");
			continue;
		}
		if (entity.second->Dirty && !entity.second->DontSync) {
			std::string changes = "ChangeTable " + entity.second->ID + "|";
			for (auto change : entity.second->ChangeTable) {
				changes += change.first + "|" + change.second + "|";
			}

			if (!entity.second->Owner.has_value()) {
				// Unable to send changetable due to the owner of the entity not being assigned
				continue;
			}
			if (Session::GetUserSession(entity.second->Owner.value()) == nullptr) {
				// Unable to send changetable due to the owner of the entity not being a registered user
				continue;
			}

			Send(entity.second->Owner.value(), changes);
			entity.second->ChangeTable.clear();
			entity.second->Dirty = false;
		}
	}
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

	DynamicEntity* de = dynamic_cast<DynamicEntity*>(entity);

	if (de != nullptr) { Space->RegisterEntity(de); }

	// Tell Neos that a new entity is in town
	if (!entity->DontSync) {
		if (entity->Owner)
			Send(entity->Owner.value(), entity->GetCreationCommand());
		else
			Send(entity->GetCreationCommand());
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

	DynamicEntity* de = dynamic_cast<DynamicEntity*>(entity);

	if (de != nullptr) { Space->UnregisterEntity(de); }

	printf("Unregistered entity: %s\n", entity->ID.c_str());

	return true;
}