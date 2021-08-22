// File: physicstick.h
// Purpose: Sets physics parameters that are sent from Neos

#pragma once

#include <stdio.h>

#include <string>

#include "core/commands.h"
#include "../utilities.h"
#include "../world/entities/dynamicentity.h"
#include "../world/entities/entity.h"
#include "../world/physics/constraints/distanceconstraint.h"
#include "../world/physics/physicsspace.h"
#include "../world/physics/shapes/sphereshape.h"
#include "../world/world.h"

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

// addforce <entityID> <force> <position>
void AddForce()
{
	std::string entityID = Commands::GetArgument<std::string>();
	Double3 force        = Commands::GetArgument<Double3>();
	Double3 position     = Commands::GetArgument<Double3>();

	auto entity = World::Singleton->Entities.find(entityID);

	if (entity != World::Singleton->Entities.end()) {
		DynamicEntity* pe = dynamic_cast<DynamicEntity*>(entity->second);
		if (pe != nullptr)
			pe->AddForceAtPosition(force, position);
	}
}

void AddForceParser(const std::string& arguments)
{
	auto parts = Split(arguments, '|');
	Commands::ValidateArgumentCount(parts, 3);

	Double3 force    = Double3FromString(parts[1]);
	Double3 position = Double3FromString(parts[2]);

	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(AddForce);

	// Put argument
	Commands::argumentBuffer.Put(parts[0]);
	Commands::argumentBuffer.Put(force);
	Commands::argumentBuffer.Put(position);
}

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
	float distance = stof(parts[5]);

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