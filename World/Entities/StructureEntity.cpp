// File: structureentity.cpp
// Purpose: Implements structureentity.h

#include "StructureEntity.h"

StructureEntity::StructureEntity(Double3 position, Quaternion rotation) : DynamicEntity(position, rotation, 0) {}

std::string StructureEntity::GetCreationCommand() { return "No :)"; }  // To be implemented!