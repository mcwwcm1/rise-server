// File: physicsentity.cpp
// Purpose: Implements physicsentity.h

#include "physicsentity.h"

PhysicsEntity::PhysicsEntity() { }

PhysicsEntity::PhysicsEntity(string id) : Entity(id) { }

PhysicsEntity::PhysicsEntity(PhysicsSpace* space)
{
	this->space = space;
}

PhysicsEntity::~PhysicsEntity()
{
}

void PhysicsEntity::RunTick(float delta) { }