// File: physicstick.h
// Purpose: Sets physics parameters that are sent from Neos

#ifndef PHYSICSTICK_H
#define PHYSICSTICK_H

#include <stdio.h>
#include <string>

#include "../world/physics/physicsspace.h"
#include "../world/entities/airship.h"
#include "../world/physics/shapes/sphereshape.h"
#include "../world/world.h"
#include "../world/entities/entity.h"
#include "../utilities.h"
#include "../world/physics/constraints/distanceconstraint.h"
#include "../world/entities/dynamicentity.h"

Airship* GetAirship(string* id) { return dynamic_cast<Airship*>(World::singleton->entities[*id]); }

DynamicEntity* GetDynamicEntity(string* id) { return dynamic_cast<DynamicEntity*>(World::singleton->entities[*id]); }


void RegisterAirship()
{
	std::string* airshipID = argumentBuffer.get().var.sval;

	World::singleton->RegisterEntity(new Airship(*airshipID));

	delete airshipID;
}

void RegisterAirshipParser(std::string& arguments)
{
	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.put(RegisterAirship);

	// Put arguments
	argumentBuffer.put(PrimaryArgument(new std::string(arguments))); // airshipID

	// Unlock buffers
	bufferAccessMutex.unlock();
}

void RegisterStaticCollider()
{
	Shape* colliderShape = new SphereShape(argumentBuffer.get().var.fval);
	std::string* positionString = argumentBuffer.get().var.sval;
	colliderShape->position = double3FromString(*positionString);
	delete positionString;
}

void RegisterStaticColliderParser(std::string& arguments)
{
	std::string radiusString = arguments.substr(0, arguments.find(' '));
	std::string positionString = arguments.substr(arguments.find(' '));

	float radius = stof(radiusString);

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.put(RegisterStaticCollider);

	// Put arguments
	argumentBuffer.put(PrimaryArgument(stof(arguments)));
	argumentBuffer.put(PrimaryArgument(new std::string(positionString)));

	// Unlock buffers
	bufferAccessMutex.unlock();
}

// setthrottle <airshipID> <throttle value between 0 and 1>
void SetThrottle()
{
	std::string* airshipID = argumentBuffer.get().var.sval;
 	Airship* airship = GetAirship(airshipID);

	if(airship != nullptr)
		airship->throttle = argumentBuffer.get().var.fval;
	
	delete airshipID;
}

void SetThrottleParser(std::string& arguments)
{
	size_t spaceIndex = arguments.find(' ');
	float throttle = stof(arguments.substr(spaceIndex + 1));

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.put(SetThrottle);
	
	// Put argument
	argumentBuffer.put(PrimaryArgument(new std::string(arguments.substr(0, spaceIndex))));
	argumentBuffer.put(PrimaryArgument(throttle));

	// Unlock buffers
	bufferAccessMutex.unlock();
}

// setpitch <airshipID> <pitch value between 0 and 1>
void SetPitch()
{
	std::string* airshipID = argumentBuffer.get().var.sval;
	Airship* airship = GetAirship(airshipID);
	
	if(airship != nullptr)
		airship->pitch = argumentBuffer.get().var.fval;

	delete airshipID;
}

void SetPitchParser(std::string& arguments)
{
	size_t spaceIndex = arguments.find(' ');
	float pitch = stof(arguments.substr(spaceIndex + 1)) * 2 - 1;

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.put(SetPitch);

	// Put argument
	argumentBuffer.put(PrimaryArgument(new std::string(arguments.substr(0, spaceIndex))));
	argumentBuffer.put(PrimaryArgument(pitch));

	// Unlock buffers
	bufferAccessMutex.unlock();
}

// setyaw <airshipID> <yaw value between 0 and 1>
void SetYaw()
{
	std::string* airshipID = argumentBuffer.get().var.sval;
	Airship* airship = GetAirship(airshipID);
	
	if(airship != nullptr)
		airship->yaw = argumentBuffer.get().var.fval;
	
	delete airshipID;
}

void SetYawParser(std::string& arguments)
{
	size_t spaceIndex = arguments.find(' ');
	float yaw = stof(arguments.substr(spaceIndex + 1)) * 2 - 1;

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.put(SetYaw);

	// Put argument
	argumentBuffer.put(PrimaryArgument(new std::string(arguments.substr(0, spaceIndex))));
	argumentBuffer.put(PrimaryArgument(yaw));

	// Unlock buffers
	bufferAccessMutex.unlock();
}

// addforce <airshipID> <force>
void AddForce()
{
	std::string* airshipID = argumentBuffer.get().var.sval;
	std::string* force = argumentBuffer.get().var.sval;
	std::string* position = argumentBuffer.get().var.sval;

	Airship* airship = GetAirship(airshipID);
	
	if(airship != nullptr)
		airship->AddForceAtPosition(double3FromString(*force), double3FromString(*position));
	
	delete airshipID;
	delete force;
	delete position;
}

void AddForceParser(std::string& arguments)
{
	size_t separator1 = arguments.find('|');
	size_t separator2 = arguments.find('|', separator1 + 1);

	string force = arguments.substr(separator1 + 1, separator2);
	string position = arguments.substr(separator2 + 1);

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();
 
	// Put function pointer
	functionBuffer.put(AddForce);

	// Put argument
	argumentBuffer.put(PrimaryArgument(new std::string(arguments.substr(0, separator1))));
	argumentBuffer.put(new std::string(force));
	argumentBuffer.put(new std::string(position));

	// Unlock buffers
	bufferAccessMutex.unlock();
}

// registerentity <entityID> <position> <rotation> <scale>
void RegisterEntity()
{
	std::string* entityID = argumentBuffer.get().var.sval;
	std::string* position = argumentBuffer.get().var.sval;
	std::string* rotation = argumentBuffer.get().var.sval;
	std::string* scale = argumentBuffer.get().var.sval;

	Entity* entity = new Entity(*entityID);
	entity->position = double3FromString(*position);
	entity->rotation = quaternionFromString(*rotation);
	entity->scale = double3FromString(*scale);

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
	functionBuffer.put(RegisterEntity);

	// Put argument
	argumentBuffer.put(new std::string(parts[0])); // EntityId
	argumentBuffer.put(new std::string(parts[1])); // Position
	argumentBuffer.put(new std::string(parts[2])); // Rotation
	argumentBuffer.put(new std::string(parts[3])); // Scale

	// Unlock buffers
	bufferAccessMutex.unlock();
}

// unregisterentity <entityID>
void UnregisterEntity()
{
	std::string* entityId = argumentBuffer.get().var.sval;

	World::singleton->UnregisterEntity(*entityId);
	
	delete entityId;
}

void UnregisterEntityParser(std::string& arguments)
{
	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();
 
	// Put function pointer
	functionBuffer.put(UnregisterEntity);

	// Put argument
	argumentBuffer.put(new std::string(arguments)); // EntityId

	// Unlock buffers
	bufferAccessMutex.unlock();
}

// adddistanceconstraint <constraintID> <entityID> <position> <entityID> <position> <distance>
void AddDistanceConstraint()
{
	std::string* constraintID 	= argumentBuffer.get().var.sval;
	std::string* entityId1 		= argumentBuffer.get().var.sval;
	std::string* position1str 	= argumentBuffer.get().var.sval;
	std::string* entityId2 		= argumentBuffer.get().var.sval;
	std::string* position2str 	= argumentBuffer.get().var.sval;
	float distance 				= argumentBuffer.get().var.fval;

	Double3 position1 		= double3FromString(*position1str);
	Double3 position2 		= double3FromString(*position2str);

	DynamicEntity* entity1 = GetDynamicEntity(entityId1);
	auto entity2PairThingFUCK = World::singleton->entities.find(*entityId2);

	if(entity1 == nullptr || entity2PairThingFUCK == World::singleton->entities.end() || entity2PairThingFUCK->second == nullptr)
		return; // One of the entities does not exist

	Entity* entity2 = entity2PairThingFUCK->second;

	// Do the thing
	DistanceConstraint* constraint = new DistanceConstraint(*constraintID);
	constraint->attachmentPoint = position1;
	constraint->targetPoint = position2;
	constraint->targetEntity = entity2;
	constraint->distance = distance;
	constraint->isRigid = true;
	entity1->constraints.push_back(constraint);
	
	delete constraintID;
	delete entityId1;
	delete position1str;
	delete entityId1;
	delete position2str;
}

void AddDistanceConstraintParser(std::string& arguments)
{
	auto parts = Split(arguments, '|');
	float distance = stof(parts[7]);

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();
 
	// Put function pointer
	functionBuffer.put(AddDistanceConstraint);
	
	// The first 7 arguments are strings :D
	for (size_t i = 0; i < 5; i++)
	{
		// Put argument
		argumentBuffer.put(new std::string(parts[i]));
	}

	argumentBuffer.put(distance);

	// Unlock buffers
	bufferAccessMutex.unlock();
}

// setowner <entityID> <ownerID>
void SetOwner()
{
	std::string* entity = argumentBuffer.get().var.sval;
	std::string* owner = argumentBuffer.get().var.sval;

	World::singleton->entities[*entity]->owner = owner;

	delete entity;
	// IMPORTANT: We are not deleting the owner here because it is used as a pointer by the entity!
	// If this ever changes we should definitely add a delete here to avoid memory leak
}

void SetOwnerParser(std::string& arguments)
{
	auto parts = Split(arguments, '|');

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();
 
	// Put function pointer
	functionBuffer.put(SetOwner);

	argumentBuffer.put(new std::string(parts[0])); // EntityID
	argumentBuffer.put(new std::string(parts[1])); // OwnerID

	// Unlock buffers
	bufferAccessMutex.unlock();
}

#endif