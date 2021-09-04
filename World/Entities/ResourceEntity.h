// File: resourceentity.h
// Purpose: Implements generic methods for holding and distributing resources "securely"

#include "World/Entities/Entity.h"
#include <unordered_map>

class ResourceEntity : Entity
{
 public:
	std::string ResourceID;

	virtual std::string GetCreationCommand();
};