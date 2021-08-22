// File: physicstick.h
// Purpose: Sets physics parameters that are sent from Neos

#pragma once

#include <stdio.h>

#include <string>

#include "core/commands.h"
#include "../utilities.h"
#include "../world/entities/airship.h"
#include "../world/entities/dynamicentity.h"
#include "../world/entities/entity.h"
#include "../world/physics/constraints/distanceconstraint.h"
#include "../world/physics/physicsspace.h"
#include "../world/physics/shapes/sphereshape.h"
#include "../world/world.h"

Airship* GetAirship(const std::string& id)
{
	return dynamic_cast<Airship*>(World::Singleton->Entities[id]);
}

DynamicEntity* GetDynamicEntity(const std::string& id)
{
	return dynamic_cast<DynamicEntity*>(World::Singleton->Entities[id]);
}

void RegisterStaticCollider()
{
	Shape* colliderShape       = new SphereShape(Commands::GetArgument<float>());
	std::string positionString = Commands::GetArgument<std::string>();
	colliderShape->Position    = Double3FromString(positionString);

	auto staticColliders = World::Singleton->Entities.find("staticColliders");

	if (staticColliders == World::Singleton->Entities.end()) {
		World::Singleton->RegisterEntity(new PhysicsEntity("staticColliders"));
		staticColliders = World::Singleton->Entities.find("staticColliders");
	}

	PhysicsEntity* e = dynamic_cast<PhysicsEntity*>(staticColliders->second);
	e->Colliders.push_back(colliderShape);
}

void RegisterStaticColliderParser(const std::string& arguments)
{
	std::string radiusString   = arguments.substr(0, arguments.find(' '));
	std::string positionString = arguments.substr(arguments.find(' '));

	float radius = stof(radiusString);

	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(RegisterStaticCollider);

	// Put arguments
	Commands::argumentBuffer.Put(radius);
	Commands::argumentBuffer.Put(positionString);
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
	size_t spaceIndex = arguments.find(' ');
	float throttle    = stof(arguments.substr(spaceIndex + 1));

	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(SetThrottle);

	// Put argument
	Commands::argumentBuffer.Put(arguments.substr(0, spaceIndex));
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
	size_t spaceIndex = arguments.find(' ');
	float pitch       = stof(arguments.substr(spaceIndex + 1)) * 2 - 1;

	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(SetPitch);

	// Put argument
	Commands::argumentBuffer.Put(arguments.substr(0, spaceIndex));
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
	size_t spaceIndex = arguments.find(' ');
	float yaw         = stof(arguments.substr(spaceIndex + 1)) * 2 - 1;

	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(SetYaw);

	// Put argument
	Commands::argumentBuffer.Put(arguments.substr(0, spaceIndex));
	Commands::argumentBuffer.Put(yaw);
}

// addforce <airshipID> <force>
void AddForce()
{
	std::string airshipID = Commands::GetArgument<std::string>();
	std::string force     = Commands::GetArgument<std::string>();
	std::string position  = Commands::GetArgument<std::string>();

	Airship* airship = GetAirship(airshipID);

	if (airship != nullptr)
		airship->AddForceAtPosition(Double3FromString(force),
		                            Double3FromString(position));
}

void AddForceParser(const std::string& arguments)
{
	size_t separator1 = arguments.find('|');
	size_t separator2 = arguments.find('|', separator1 + 1);

	std::string force    = arguments.substr(separator1 + 1, separator2);
	std::string position = arguments.substr(separator2 + 1);

	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(AddForce);

	// Put argument
	Commands::argumentBuffer.Put(arguments.substr(0, separator1));
	Commands::argumentBuffer.Put(force);
	Commands::argumentBuffer.Put(position);
}

// registerentity <entityID> <position> <rotation> <scale>
void RegisterEntity()
{
	std::string entityID = Commands::GetArgument<std::string>();
	std::string position = Commands::GetArgument<std::string>();
	std::string rotation = Commands::GetArgument<std::string>();
	std::string scale    = Commands::GetArgument<std::string>();

	Entity* entity   = new Entity(entityID);
	entity->Position = Double3FromString(position);
	entity->Rotation = QuaternionFromString(rotation);
	entity->Scale    = Double3FromString(scale);

	World::Singleton->RegisterEntity(entity);
}

void RegisterEntityParser(const std::string& arguments)
{
	auto parts = Split(arguments, '|');
	Commands::ValidateArgumentCount(parts, 4);

	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(RegisterEntity);

	// Put argument
	Commands::argumentBuffer.Put(parts[0]);  // EntityId
	Commands::argumentBuffer.Put(parts[1]);  // Position
	Commands::argumentBuffer.Put(parts[2]);  // Rotation
	Commands::argumentBuffer.Put(parts[3]);  // Scale
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

// adddistanceconstraint <constraintID> <entityID> <position> <entityID> <position> <distance>
void AddDistanceConstraint()
{
	std::string constraintID = Commands::GetArgument<std::string>();
	std::string entityId1    = Commands::GetArgument<std::string>();
	std::string position1str = Commands::GetArgument<std::string>();
	std::string entityId2    = Commands::GetArgument<std::string>();
	std::string position2str = Commands::GetArgument<std::string>();
	float distance           = Commands::GetArgument<float>();

	Double3 position1 = Double3FromString(position1str);
	Double3 position2 = Double3FromString(position2str);

	DynamicEntity* entity1    = GetDynamicEntity(entityId1);
	auto entity2PairThingFUCK = World::Singleton->Entities.find(entityId2);

	if (entity1 == nullptr ||
	    entity2PairThingFUCK == World::Singleton->Entities.end() ||
	    entity2PairThingFUCK->second == nullptr)
		return;  // One of the entities does not exist

	Entity* entity2 = entity2PairThingFUCK->second;

	// Do the thing
	DistanceConstraint* constraint = new DistanceConstraint(constraintID);
	constraint->AttachmentPoint    = position1;
	constraint->TargetPoint        = position2;
	constraint->TargetEntity       = entity2;
	constraint->Distance           = distance;
	constraint->IsRigid            = true;
	entity1->Constraints.push_back(constraint);
}

void AddDistanceConstraintParser(const std::string& arguments)
{
	auto parts = Split(arguments, '|');
	Commands::ValidateArgumentCount(parts, 6);
	float distance = stof(parts[6]);

	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(AddDistanceConstraint);

	// The first 5 arguments are strings :D
	for (size_t i = 0; i < 5; i++) {
		// Put argument
		Commands::argumentBuffer.Put(parts[i]);
	}

	Commands::argumentBuffer.Put(distance);
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

// requestairship <locationID> <position> <rotation> <userID>
void RequestAirship()
{
	std::string locationID  = Commands::GetArgument<std::string>();
	std::string positionStr = Commands::GetArgument<std::string>();
	std::string rotationStr = Commands::GetArgument<std::string>();
	std::string userID      = Commands::GetArgument<std::string>();

	bool succeeded = true;

	auto location = World::Singleton->Entities.find(locationID);

	if (location == World::Singleton->Entities.end()) {
		printf(
				"Attempted to request airship atocation with entity id \"%s\" but the location entity does not exist.\n",
				locationID.c_str());
		succeeded = false;
	}

	Double3 position;
	Quaternion rotation;

	if (!TryDouble3FromString(positionStr, position)) {
		printf("Unable to parse position string during \"requestairship\": %s\n",
		       positionStr.c_str());
		succeeded = false;
	}

	if (!TryQuaternionFromString(rotationStr, rotation)) {
		printf("Unable to parse rotation string during \"requestairship\": %s\n",
		       rotationStr.c_str());
		succeeded = false;
	}

	if (succeeded) {
		position = location->second->LocalPointToGlobal(position);
		rotation = location->second->LocalRotationToGlobal(rotation);

		Airship* airship = new Airship(Airship::GetNextID());
		World::Singleton->RegisterEntity(airship);

		std::string instruction = "SpawnAirship " + airship->ID + "|" +
		                          positionStr + "|" + rotationStr + "|";

		Send(instruction);
	}
}

void RequestAirshipParser(const std::string& arguments)
{
	auto parts = Split(arguments, '|');
	Commands::ValidateArgumentCount(parts, 4);

	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(RequestAirship);

	Commands::argumentBuffer.Put(parts[0]);  // LocationID
	Commands::argumentBuffer.Put(parts[1]);  // Position
	Commands::argumentBuffer.Put(parts[2]);  // Rotation
	Commands::argumentBuffer.Put(parts[3]);  // UserID
}