// File: physicsentity.cpp
// Purpose: Implements physicsentity.h

#include "physicsentity.h"

#include "../world.h"

PhysicsEntity::PhysicsEntity() {}

PhysicsEntity::PhysicsEntity(std::string id) : Entity(id) {}

PhysicsEntity::~PhysicsEntity() {}

void PhysicsEntity::RunTick(float delta) {}