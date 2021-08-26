// File: structureentity.h
// Purpose: An entity that has structure data required for constructing islands and stuff

#include "dynamicentity.h"

class StructureEntity : public DynamicEntity
{
 public:
	StructureEntity(Double3 position, Quaternion rotation);
	virtual std::string GetCreationCommand();
};