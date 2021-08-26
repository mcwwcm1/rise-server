// File: userentity.cpp
// Purpose: Implements userentity.h

#include "userentity.h"
#include <algorithm>
#include "data/items.h"

std::string UserEntity::GetCreationCommand()
{
	return "RegisteredUser " + UserID;
}