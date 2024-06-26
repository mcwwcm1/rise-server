// File: physicsinstructions.h
// Purpose: For registering colliders and changing physical properties of entities

#pragma once

#include <stdio.h>

#include <string>

#include "Core/Commands.h"
#include "Utilities.h"
#include "World/Entities/DynamicEntity.h"
#include "World/Entities/Entity.h"
#include "World/Physics/PhysicsSpace.h"
#include "World/World.h"
#include "World/Physics/Constraints/DistanceConstraint.h"

DynamicEntity* GetDynamicEntity(const std::string& id)
{
	auto e = World::Singleton->Entities.find(id);
	if (e == World::Singleton->Entities.end()) { return nullptr; }
	return dynamic_cast<DynamicEntity*>(e->second);
}

// This will not exist later once we have structure colliders defined in the db
// registerstaticcollider <entityID> <position> <radius>
void RegisterStaticCollider()
{
	std::string entityID = Commands::GetArgument<std::string>();
	Double3 position     = Commands::GetArgument<Double3>();
	btSphereShape* shape = new btSphereShape(Commands::GetArgument<float>());

	auto entity = World::Singleton->Entities.find(entityID);

	if (entity == World::Singleton->Entities.end()) {
		printf("Tried to attach static collider to non-existent entity!\n");
		return;
	}

	DynamicEntity* e = dynamic_cast<DynamicEntity*>(entity->second);
	e->Shape->addChildShape(btTransform(btQuaternion(0, 0, 0), btVector3(position.x, position.y, position.z)), shape);
}

void RegisterStaticColliderParser(const std::string& arguments)
{
	auto parts = Split(arguments, '|');

	Commands::ValidateArgumentCount(parts, 3);

	Double3 position = Double3FromString(parts[1]);
	float radius     = stof(parts[2]);

	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(RegisterStaticCollider);

	// Put arguments
	Commands::argumentBuffer.Put(parts[0]);  // EntityID
	Commands::argumentBuffer.Put(position);
	Commands::argumentBuffer.Put(radius);
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
		if (pe != nullptr) pe->RigidBody->applyForce(force, position);
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

	btVector3 position1 = Double3FromString(position1str);
	btVector3 position2 = Double3FromString(position2str);

	DynamicEntity* entity1 = GetDynamicEntity(entityId1);
	DynamicEntity* entity2 = GetDynamicEntity(entityId2);

	if (entity1 == nullptr || entity2 == nullptr) {
		printf("Attempted to add constraint to a non-dynamic / non-existent entity.\n");
		return;
	}

	auto constraint = new DistanceConstraint(*entity1, *entity2, position1, position2, distance);

	World::Singleton->Space->RegisterConstraint(constraintID, constraint);
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

// removeconstraint <constraintID>
void RemoveConstraint()
{
	std::string constraintID = Commands::GetArgument<std::string>();
	auto constraint          = World::Singleton->Space->GetConstraint(constraintID);
	if (constraint != nullptr) {
		World::Singleton->Space->UnregisterConstraint(constraintID);
		delete constraint;
	}
}

void RemoveConstraintParser(const std::string& arguments)
{
	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(RemoveConstraint);

	Commands::argumentBuffer.Put(arguments);  // ConstraintID
}