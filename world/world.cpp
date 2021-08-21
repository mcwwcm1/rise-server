// File: world.cpp
// Purpose: Implements world.h

#include "world.h"

World* World::singleton = new World();

bool World::RegisterEntity(Entity* entity)
{
	auto e = entities.find(entity->id);
	if(e != entities.end())
	{	
		// Entity is already registered :(
		return false;
	}

	entities.insert({entity->id, entity});
	
	printf("Registered entity: %s\n", entity->id.c_str());
	
	return true;
}

bool World::UnregisterEntity(string entityId)
{
	auto e = entities.find(entityId);
	if(e == entities.end()) 
		return false;
	
	return UnregisterEntity(e->second);
}

bool World::UnregisterEntity(Entity* entity)
{
	auto e = entities.find(entity->id);
	if(e == entities.end())
	{	
		// Entity is not registered :(
		return false;
	}

	entities.erase(e);

	printf("Unregistered entity: %s\n", entity->id.c_str());

	return true;
}