// File: entityinstructions.h
// Purpose: For registering entities and assigning various properties about them

#include "mysticmath/double3.h"
#include "mysticmath/quaternion.h"
#include "world/entities/entity.h"
#include "core/commands.h"
#include "utilities.h"
#include "world/world.h"

// setowner <entityID> <ownerID>
void SetOwner()
{
	std::string entity = Commands::GetArgument<std::string>();
	std::string owner  = Commands::GetArgument<std::string>();

	if (World::Singleton->Entities.find(entity) ==
	    World::Singleton->Entities.end()) {
		// Entity is not present in world D:
		printf("Attempted to set owner of non-existent entity (%s) to %s\n",
		       entity.c_str(),
		       owner.c_str());
		return;
	}

	World::Singleton->Entities[entity]->Owner = owner;
}

void SetOwnerParser(const std::string& arguments)
{
	auto parts = Split(arguments, '|');
	Commands::ValidateArgumentCount(parts, 2);

	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(SetOwner);

	Commands::argumentBuffer.Put(parts[0]);  // EntityID
	Commands::argumentBuffer.Put(parts[1]);  // OwnerID
}