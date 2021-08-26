// File: userentity.cpp
// Purpose: Implements userentity.h

#include "userentity.h"

std::string UserEntity::GetCreationCommand()
{
	return "RegisteredUser " + UserID;
}