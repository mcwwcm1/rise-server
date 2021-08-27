// File: userdata.h
// Purpose: For getting data about a users actions in game

#include <string>
#include "utilities.h"
#include "core/commands.h"
#include "world/world.h"

// This will not exist later once we have structure colliders defined in the db
// setuserposition <userID> <position>
void SetUserPositionCollider()
{
	std::string userID = Commands::GetArgument<std::string>();
	Double3 position   = Commands::GetArgument<Double3>();

	UserEntity* user;

	// It is unwise to be creating a new user at this precise location. Ideally we would have Neos tell the server when a user enters and leaves the world.
	// We could have it send a warning command to Neos if this happens though, it probably would mean that the user was not registered / it was missed, so send it again?

	auto pair = World::Singleton->Users.find(userID);
	if (pair == World::Singleton->Users.end()) {
		user = new UserEntity(userID);
		World::Singleton->RegisterEntity(user);
	} else
		user = pair->second;

	user->Position = position;
}

void SetUserPositionParser(const std::string& arguments)
{
	auto parts = Split(arguments, '|');

	Commands::ValidateArgumentCount(parts, 2);

	Double3 position = Double3FromString(parts[1]);

	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(SetUserPositionCollider);

	// Put arguments
	Commands::argumentBuffer.Put(parts[0]);  // UserID
	Commands::argumentBuffer.Put(position);
}