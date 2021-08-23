// File: airshipcontrol.h
// Purpose: defines commands for setting various parameters on an airship

#include <string>
#include "core/commands.h"
#include "world/entities/airship.h"
#include "world/world.h"
#include "../utilities.h"
#include "core/send.h"
#include "mysticmath/double3.h"
#include "mysticmath/quaternion.h"

Airship* GetAirship(const std::string& id)
{
	return dynamic_cast<Airship*>(World::Singleton->Entities[id]);
}

// requestairship <locationID> <position> <rotation> <userID>
void RequestAirship()
{
	std::string locationID = Commands::GetArgument<std::string>();
	Double3 position       = Commands::GetArgument<Double3>();
	Quaternion rotation    = Commands::GetArgument<Quaternion>();
	std::string userID     = Commands::GetArgument<std::string>();

	auto location = World::Singleton->Entities.find(locationID);

	if (location == World::Singleton->Entities.end()) {
		printf(
				"Attempted to request airship at location with entity id \"%s\" but the location entity does not exist.\n",
				locationID.c_str());
		return;
	}

	position = location->second->LocalPointToGlobal(position);
	rotation = location->second->LocalRotationToGlobal(rotation);

	Airship* airship = new Airship(Airship::GetNextID(), position, rotation);

	airship->SetLocalPosition(position);
	airship->SetLocalRotation(rotation);

	World::Singleton->RegisterEntity(airship);

	std::string instruction = "SpawnAirship " + airship->ID + "|" + position.ToString() + "|" + rotation.ToString() + "|";

	Send(instruction);
}

void RequestAirshipParser(const std::string& arguments)
{
	auto parts = Split(arguments, '|');
	Commands::ValidateArgumentCount(parts, 4);

	Double3 position    = Double3FromString(parts[1]);
	Quaternion rotation = QuaternionFromString(parts[2]);

	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(RequestAirship);

	Commands::argumentBuffer.Put(parts[0]);  // LocationID
	Commands::argumentBuffer.Put(position);  // Position
	Commands::argumentBuffer.Put(rotation);  // Rotation
	Commands::argumentBuffer.Put(parts[3]);  // UserID
}

// setthrottle <airshipID> <throttle value between 0 and 1>
void SetThrottle()
{
	std::string airshipID = Commands::GetArgument<std::string>();
	Airship* airship      = GetAirship(airshipID);

	if (airship != nullptr) airship->Throttle = Commands::GetArgument<float>();
}

void SetThrottleParser(const std::string& arguments)
{
	auto parts = Split(arguments, '|');
	Commands::ValidateArgumentCount(parts, 2);

	float throttle = stof(parts[1]);

	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(SetThrottle);

	// Put argument
	Commands::argumentBuffer.Put(parts[0]);
	Commands::argumentBuffer.Put(throttle);
}

// setpitch <airshipID> <pitch value between 0 and 1>
void SetPitch()
{
	std::string airshipID = Commands::GetArgument<std::string>();
	Airship* airship      = GetAirship(airshipID);

	if (airship != nullptr) { airship->Pitch = Commands::GetArgument<float>(); }
}

void SetPitchParser(const std::string& arguments)
{
	auto parts = Split(arguments, '|');
	Commands::ValidateArgumentCount(parts, 2);

	float pitch = stof(parts[1]) * 2 - 1;

	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(SetPitch);

	// Put argument
	Commands::argumentBuffer.Put(parts[0]);
	Commands::argumentBuffer.Put(pitch);
}

// setyaw <airshipID> <yaw value between 0 and 1>
void SetYaw()
{
	std::string airshipID = Commands::GetArgument<std::string>();
	Airship* airship      = GetAirship(airshipID);

	if (airship != nullptr) { airship->Yaw = Commands::GetArgument<float>(); }
}

void SetYawParser(const std::string& arguments)
{
	auto parts = Split(arguments, '|');
	Commands::ValidateArgumentCount(parts, 2);

	float yaw = stof(parts[1]) * 2 - 1;

	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(SetYaw);

	// Put argument
	Commands::argumentBuffer.Put(parts[0]);
	Commands::argumentBuffer.Put(yaw);
}
