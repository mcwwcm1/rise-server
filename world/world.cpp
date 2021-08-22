// File: world.cpp
// Purpose: Implements world.h

#include "world.h"

World* World::Singleton = new World();

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

	PhysicsEntity* pe = dynamic_cast<PhysicsEntity*>(entity);

	if (pe != nullptr) { Space->RegisterEntity(pe); }

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

	PhysicsEntity* pe = dynamic_cast<PhysicsEntity*>(entity);

	if (pe != nullptr) { Space->UnregisterEntity(pe); }

	printf("Unregistered entity: %s\n", entity->ID.c_str());

	return true;
}