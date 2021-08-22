// File: entityinstructions.h
// Purpose: For registering entities and assigning various properties about them

#include "mysticmath/double3.h"
#include "mysticmath/quaternion.h"
#include "world/entities/entity.h"
#include "core/commands.h"
#include "utilities.h"
#include "world/world.h"

// registerentity <entityID> <position> <rotation> <scale>
void RegisterEntity()
{
	std::string entityID = Commands::GetArgument<std::string>();
	Double3 position     = Commands::GetArgument<Double3>();
	Quaternion rotation  = Commands::GetArgument<Quaternion>();
	Double3 scale        = Commands::GetArgument<Double3>();

	Entity* entity   = new Entity(entityID);
	entity->Position = position;
	entity->Rotation = rotation;
	entity->Scale    = scale;

	World::Singleton->RegisterEntity(entity);
}

void RegisterEntityParser(const std::string& arguments)
{
	auto parts = Split(arguments, '|');
	Commands::ValidateArgumentCount(parts, 4);

	Double3 position    = Double3FromString(parts[1]);
	Quaternion rotation = QuaternionFromString(parts[2]);
	Double3 scale       = Double3FromString(parts[3]);

	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(RegisterEntity);

	// Put argument
	Commands::argumentBuffer.Put(parts[0]);  // EntityId
	Commands::argumentBuffer.Put(position);  // Position
	Commands::argumentBuffer.Put(rotation);  // Rotation
	Commands::argumentBuffer.Put(scale);     // Scale
}

// unregisterentity <entityID>
void UnregisterEntity()
{
	std::string entityId = Commands::GetArgument<std::string>();

	World::Singleton->UnregisterEntity(entityId);
}

void UnregisterEntityParser(const std::string& arguments)
{
	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(UnregisterEntity);

	// Put argument
	Commands::argumentBuffer.Put(arguments);  // EntityId
}

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