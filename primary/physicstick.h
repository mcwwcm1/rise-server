// File: physicstick.h
// Purpose: Sets physics parameters that are sent from Neos

#ifndef PHYSICSTICK_H
#define PHYSICSTICK_H

#include <stdio.h>

#include <string>

#include "../utilities.h"
#include "../world/entities/airship.h"
#include "../world/entities/dynamicentity.h"
#include "../world/entities/entity.h"
#include "../world/physics/constraints/distanceconstraint.h"
#include "../world/physics/physicsspace.h"
#include "../world/physics/shapes/sphereshape.h"
#include "../world/world.h"

Airship* GetAirship(string* id)
{
	return dynamic_cast<Airship*>(World::singleton->entities[*id]);
}

DynamicEntity* GetDynamicEntity(string* id)
{
	return dynamic_cast<DynamicEntity*>(World::singleton->entities[*id]);
}

void RegisterStaticCollider()
{
	Shape* colliderShape        = new SphereShape(argumentBuffer.Get().var.fval);
	std::string* positionString = argumentBuffer.Get().var.sval;
	colliderShape->position     = double3FromString(*positionString);

	auto staticColliders = World::singleton->entities.find("staticColliders");

	if (staticColliders == World::singleton->entities.end()) {
		World::singleton->RegisterEntity(new PhysicsEntity("staticColliders"));
		staticColliders = World::singleton->entities.find("staticColliders");
	}

	PhysicsEntity* e = dynamic_cast<PhysicsEntity*>(staticColliders->second);
	e->colliders.push_back(colliderShape);

	delete positionString;
}

void RegisterStaticColliderParser(std::string& arguments)
{
	std::string radiusString   = arguments.substr(0, arguments.find(' '));
	std::string positionString = arguments.substr(arguments.find(' '));

	float radius = stof(radiusString);

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.Put(RegisterStaticCollider);

	// Put arguments
	argumentBuffer.Put(PrimaryArgument(radius));
	argumentBuffer.Put(PrimaryArgument(new std::string(positionString)));

	// Unlock buffers
	bufferAccessMutex.unlock();
}

// setthrottle <airshipID> <throttle value between 0 and 1>
void SetThrottle()
{
	std::string* airshipID = argumentBuffer.Get().var.sval;
	Airship* airship       = GetAirship(airshipID);

	if (airship != nullptr) airship->throttle = argumentBuffer.Get().var.fval;

	delete airshipID;
}

void SetThrottleParser(std::string& arguments)
{
	size_t spaceIndex = arguments.find(' ');
	float throttle    = stof(arguments.substr(spaceIndex + 1));

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.Put(SetThrottle);

	// Put argument
	argumentBuffer.Put(
			PrimaryArgument(new std::string(arguments.substr(0, spaceIndex))));
	argumentBuffer.Put(PrimaryArgument(throttle));

	// Unlock buffers
	bufferAccessMutex.unlock();
}

// setpitch <airshipID> <pitch value between 0 and 1>
void SetPitch()
{
	std::string* airshipID = argumentBuffer.Get().var.sval;
	Airship* airship       = GetAirship(airshipID);

	if (airship != nullptr) airship->pitch = argumentBuffer.Get().var.fval;

	delete airshipID;
}

void SetPitchParser(std::string& arguments)
{
	size_t spaceIndex = arguments.find(' ');
	float pitch       = stof(arguments.substr(spaceIndex + 1)) * 2 - 1;

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.Put(SetPitch);

	// Put argument
	argumentBuffer.Put(
			PrimaryArgument(new std::string(arguments.substr(0, spaceIndex))));
	argumentBuffer.Put(PrimaryArgument(pitch));

	// Unlock buffers
	bufferAccessMutex.unlock();
}

// setyaw <airshipID> <yaw value between 0 and 1>
void SetYaw()
{
	std::string* airshipID = argumentBuffer.Get().var.sval;
	Airship* airship       = GetAirship(airshipID);

	if (airship != nullptr) airship->yaw = argumentBuffer.Get().var.fval;

	delete airshipID;
}

void SetYawParser(std::string& arguments)
{
	size_t spaceIndex = arguments.find(' ');
	float yaw         = stof(arguments.substr(spaceIndex + 1)) * 2 - 1;

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.Put(SetYaw);

	// Put argument
	argumentBuffer.Put(
			PrimaryArgument(new std::string(arguments.substr(0, spaceIndex))));
	argumentBuffer.Put(PrimaryArgument(yaw));

	// Unlock buffers
	bufferAccessMutex.unlock();
}

// addforce <airshipID> <force>
void AddForce()
{
	std::string* airshipID = argumentBuffer.Get().var.sval;
	std::string* force     = argumentBuffer.Get().var.sval;
	std::string* position  = argumentBuffer.Get().var.sval;

	Airship* airship = GetAirship(airshipID);

	if (airship != nullptr)
		airship->AddForceAtPosition(double3FromString(*force),
		                            double3FromString(*position));

	delete airshipID;
	delete force;
	delete position;
}

void AddForceParser(std::string& arguments)
{
	size_t separator1 = arguments.find('|');
	size_t separator2 = arguments.find('|', separator1 + 1);

	string force    = arguments.substr(separator1 + 1, separator2);
	string position = arguments.substr(separator2 + 1);

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.Put(AddForce);

	// Put argument
	argumentBuffer.Put(
			PrimaryArgument(new std::string(arguments.substr(0, separator1))));
	argumentBuffer.Put(new std::string(force));
	argumentBuffer.Put(new std::string(position));

	// Unlock buffers
	bufferAccessMutex.unlock();
}

// registerentity <entityID> <position> <rotation> <scale>
void RegisterEntity()
{
	std::string* entityID = argumentBuffer.Get().var.sval;
	std::string* position = argumentBuffer.Get().var.sval;
	std::string* rotation = argumentBuffer.Get().var.sval;
	std::string* scale    = argumentBuffer.Get().var.sval;

	Entity* entity   = new Entity(*entityID);
	entity->position = double3FromString(*position);
	entity->rotation = quaternionFromString(*rotation);
	entity->scale    = double3FromString(*scale);

	World::singleton->RegisterEntity(entity);

	delete entityID;
	delete position;
	delete rotation;
	delete scale;
}

void RegisterEntityParser(std::string& arguments)
{
	auto parts = Split(arguments, '|');

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.Put(RegisterEntity);

	// Put argument
	argumentBuffer.Put(new std::string(parts[0]));  // EntityId
	argumentBuffer.Put(new std::string(parts[1]));  // Position
	argumentBuffer.Put(new std::string(parts[2]));  // Rotation
	argumentBuffer.Put(new std::string(parts[3]));  // Scale

	// Unlock buffers
	bufferAccessMutex.unlock();
}

// unregisterentity <entityID>
void UnregisterEntity()
{
	std::string* entityId = argumentBuffer.Get().var.sval;

	World::singleton->UnregisterEntity(*entityId);

	delete entityId;
}

void UnregisterEntityParser(std::string& arguments)
{
	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.Put(UnregisterEntity);

	// Put argument
	argumentBuffer.Put(new std::string(arguments));  // EntityId

	// Unlock buffers
	bufferAccessMutex.unlock();
}

// adddistanceconstraint <constraintID> <entityID> <position> <entityID> <position> <distance>
void AddDistanceConstraint()
{
	std::string* constraintID = argumentBuffer.Get().var.sval;
	std::string* entityId1    = argumentBuffer.Get().var.sval;
	std::string* position1str = argumentBuffer.Get().var.sval;
	std::string* entityId2    = argumentBuffer.Get().var.sval;
	std::string* position2str = argumentBuffer.Get().var.sval;
	float distance            = argumentBuffer.Get().var.fval;

	Double3 position1 = double3FromString(*position1str);
	Double3 position2 = double3FromString(*position2str);

	DynamicEntity* entity1    = GetDynamicEntity(entityId1);
	auto entity2PairThingFUCK = World::singleton->entities.find(*entityId2);

	if (entity1 == nullptr ||
	    entity2PairThingFUCK == World::singleton->entities.end() ||
	    entity2PairThingFUCK->second == nullptr)
		return;  // One of the entities does not exist

	Entity* entity2 = entity2PairThingFUCK->second;

	// Do the thing
	DistanceConstraint* constraint = new DistanceConstraint(*constraintID);
	constraint->attachmentPoint    = position1;
	constraint->targetPoint        = position2;
	constraint->targetEntity       = entity2;
	constraint->distance           = distance;
	constraint->isRigid            = true;
	entity1->constraints.push_back(constraint);

	delete constraintID;
	delete entityId1;
	delete position1str;
	delete entityId1;
	delete position2str;
}

void AddDistanceConstraintParser(std::string& arguments)
{
	auto parts     = Split(arguments, '|');
	float distance = stof(parts[7]);

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.Put(AddDistanceConstraint);

	// The first 7 arguments are strings :D
	for (size_t i = 0; i < 5; i++) {
		// Put argument
		argumentBuffer.Put(new std::string(parts[i]));
	}

	argumentBuffer.Put(distance);

	// Unlock buffers
	bufferAccessMutex.unlock();
}

// setowner <entityID> <ownerID>
void SetOwner()
{
	std::string* entity = argumentBuffer.Get().var.sval;
	std::string* owner  = argumentBuffer.Get().var.sval;

	if (World::singleton->entities.find(*entity) ==
	    World::singleton->entities.end()) {
		// Entity is not present in world D:
		printf("Attempted to set owner of non-existent entity (%s) to %s\n",
		       entity->c_str(),
		       owner->c_str());
		delete entity;
		delete owner;
		return;
	}

	World::singleton->entities[*entity]->owner = owner;

	delete entity;
	// IMPORTANT: We are not deleting the owner here because it is used as a pointer by the entity!
	// If this ever changes we should definitely add a delete here to avoid memory leak
}

void SetOwnerParser(std::string& arguments)
{
	auto parts = Split(arguments, '|');

	if (parts.size() != 2) {
		printf(
				"Incorrect number of arguments for \"setowner\". Recieved %li but expected 2: %s",
				parts.size(),
				arguments.c_str());
		return;
	}

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.Put(SetOwner);

	argumentBuffer.Put(new std::string(parts[0]));  // EntityID
	argumentBuffer.Put(new std::string(parts[1]));  // OwnerID

	// Unlock buffers
	bufferAccessMutex.unlock();
}

// requestairship <locationID> <position> <rotation> <userID>
void RequestAirship()
{
	std::string* locationID  = argumentBuffer.Get().var.sval;
	std::string* positionStr = argumentBuffer.Get().var.sval;
	std::string* rotationStr = argumentBuffer.Get().var.sval;
	std::string* userID      = argumentBuffer.Get().var.sval;

	bool succeeded = true;

	auto location = World::singleton->entities.find(*locationID);

	if (location == World::singleton->entities.end()) {
		printf(
				"Attempted to request airship atocation with entity id \"%s\" but the location entity does not exist.\n",
				locationID->c_str());
		succeeded = false;
	}

	Double3 position;
	Quaternion rotation;

	if (!tryDouble3FromString(*positionStr, position)) {
		printf("Unable to parse position string during \"requestairship\": %s\n",
		       positionStr->c_str());
		succeeded = false;
	}

	if (!tryQuaternionFromString(*rotationStr, rotation)) {
		printf("Unable to parse rotation string during \"requestairship\": %s\n",
		       rotationStr->c_str());
		succeeded = false;
	}

	if (succeeded) {
		position = location->second->LocalPointToGlobal(position);
		rotation = location->second->LocalRotationToGlobal(rotation);

		Airship* airship = new Airship(Airship::GetNextID());
		World::singleton->RegisterEntity(airship);

		string* instruction = new string("SpawnAirship " + airship->id + "|" +
		                                 *positionStr + "|" + *rotationStr + "|");

		Send(instruction);

		delete instruction;
	}

	delete locationID;
	delete positionStr;
	delete rotationStr;
	delete userID;
}

void RequestAirshipParser(std::string& arguments)
{
	auto parts = Split(arguments, '|');

	if (parts.size() != 4) {
		printf(
				"Incorrect number of arguments for \"requestairship\". Recieved %li but expected 4: %s\n",
				parts.size(),
				arguments.c_str());
		return;
	}

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.Put(RequestAirship);

	argumentBuffer.Put(new std::string(parts[0]));  // LocationID
	argumentBuffer.Put(new std::string(parts[1]));  // Position
	argumentBuffer.Put(new std::string(parts[2]));  // Rotation
	argumentBuffer.Put(new std::string(parts[3]));  // UserID

	// Unlock buffers
	bufferAccessMutex.unlock();
}

#endif