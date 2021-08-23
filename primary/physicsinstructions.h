// File: physicsinstructions.h
// Purpose: For registering colliders and changing physical properties of entities

#pragma once

#include <stdio.h>

#include <string>

#include "core/commands.h"
#include "../utilities.h"
#include "../world/entities/dynamicentity.h"
#include "../world/entities/entity.h"
#include "../world/physics/physicsspace.h"
#include "../world/world.h"

DynamicEntity* GetDynamicEntity(const std::string& id)
{
	return dynamic_cast<DynamicEntity*>(World::Singleton->Entities[id]);
}

void RegisterStaticCollider()
{
	//btCollisionShape* colliderShape    = new btSphereShape(Commands::GetArgument<float>());
	//std::string positionString         = Commands::GetArgument<std::string>();
	//btCollisionObject* collisionObject = new btCollisionObject();
	//collisionObject->setCollisionShape(colliderShape);
	//collisionObject->setWorldTransform(Double3FromString(positionString))
	//
	//		auto staticColliders = World::Singleton->Entities.find("staticColliders");
	//
	//if (staticColliders == World::Singleton->Entities.end()) {
	//	World::Singleton->RegisterEntity(new PhysicsEntity("staticColliders"));
	//	staticColliders = World::Singleton->Entities.find("staticColliders");
	//}
	//
	//PhysicsEntity* e = dynamic_cast<PhysicsEntity*>(staticColliders->second);
	//e->Colliders.push_back(colliderShape);
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
	//Commands::argumentBuffer.Put(radius);
	//Commands::argumentBuffer.Put(positionString);
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
			pe->RigidBody->applyForce(force, position);
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

	// ADD CONSTRAINT HERE
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