// File: world.cpp
// Purpose: Implements world.h

#include "world.h"

World* World::singleton = new World();

bool World::RegisterEntity(Entity* entity)
{
  auto e = entities.find(entity->id);
  if (e != entities.end()) {
    // Entity is already registered :(
    printf("Attempted to register entity that is already registered: %s\n",
           entity->id.c_str());
    return false;
  }

  entities.insert({entity->id, entity});

  PhysicsEntity* pe = dynamic_cast<PhysicsEntity*>(entity);

  if (pe != nullptr) { space->RegisterEntity(pe); }

  printf("Registered entity: %s\n", entity->id.c_str());

  return true;
}

bool World::UnregisterEntity(string entityId)
{
  auto e = entities.find(entityId);
  if (e == entities.end()) return false;

  return UnregisterEntity(e->second);
}

bool World::UnregisterEntity(Entity* entity)
{
  auto e = entities.find(entity->id);
  if (e == entities.end()) {
    // Entity is not registered :(
    printf("Attempted to unregister entity that is not registered: %s\n",
           entity->id.c_str());
    return false;
  }

  entities.erase(e);

  PhysicsEntity* pe = dynamic_cast<PhysicsEntity*>(entity);

  if (pe != nullptr) { space->UnregisterEntity(pe); }

  printf("Unregistered entity: %s\n", entity->id.c_str());

  return true;
}