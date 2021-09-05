// File: airshipcontrol.h
// Purpose: defines commands for setting various parameters on an airship

#include <string>
#include "Core/Commands.h"
#include "World/Entities/Airship.h"
#include "World/World.h"
#include "Utilities.h"
#include "Core/Send.h"
#include "MysticMath/Double3.h"
#include "MysticMath/Quaternion.h"

Airship* GetAirship(const std::string& id)
{
	auto it = World::Singleton->Entities.find(id);
	if (it != World::Singleton->Entities.end()) {
		return dynamic_cast<Airship*>(it->second);
	}

	return nullptr;
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

	Airship* airship = new Airship(position, rotation);

	airship->SetLocalPosition(position);
	airship->SetLocalRotation(rotation);

	World::Singleton->RegisterEntity(airship);
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
	float throttle        = Commands::GetArgument<float>();
	Airship* airship      = GetAirship(airshipID);

	if (airship != nullptr) { airship->Throttle = throttle; }
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
	float pitch           = Commands::GetArgument<float>();
	Airship* airship      = GetAirship(airshipID);

	if (airship != nullptr) { airship->Pitch = pitch; }
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
	float yaw             = Commands::GetArgument<float>();
	Airship* airship      = GetAirship(airshipID);

	if (airship != nullptr) { airship->Yaw = yaw; }
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
