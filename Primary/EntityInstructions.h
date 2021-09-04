// File: entityinstructions.h
// Purpose: For registering entities and assigning various properties about them

#include "MysticMath/Double3.h"
#include "MysticMath/Quaternion.h"
#include "World/Entities/Entity.h"
#include "Core/Commands.h"
#include "Utilities.h"
#include "World/World.h"

// catchbug <entityID> <userID>
void CatchBug()
{
	std::string entityID = Commands::GetArgument<std::string>();
	std::string userID   = Commands::GetArgument<std::string>();

	auto entityPair = World::Singleton->Entities.find(entityID);
	auto userPair   = World::Singleton->Users.find(userID);

	if (entityPair == World::Singleton->Entities.end() || userPair == World::Singleton->Users.end()) return;

	BugSwarmEntity* bugSwarm = dynamic_cast<BugSwarmEntity*>(entityPair->second);

	if (bugSwarm != nullptr)
		bugSwarm->CatchBug(userPair->second);
}

void CatchBugParser(const std::string& arguments)
{
	auto parts = Split(arguments, '|');
	Commands::ValidateArgumentCount(parts, 2);

	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(CatchBug);

	Commands::argumentBuffer.Put(parts[0]);  // EntityID
	Commands::argumentBuffer.Put(parts[1]);  // UserID
}