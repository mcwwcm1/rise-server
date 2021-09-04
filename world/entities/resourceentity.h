// File: resourceentity.h
// Purpose: Implements generic methods for holding and distributing resources "securely"

#include "entity.h"
#include <unordered_map>

class ResourceEntity : Entity
{
 public:
	std::string ResourceID;

	virtual std::string GetCreationCommand();
};