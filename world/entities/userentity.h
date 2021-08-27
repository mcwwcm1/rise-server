// File: userentity.h
// Purpose: Keeps track of user information and supplies useful methods for changing properties of a user in game

#include "entity.h"
#include <string>

class UserEntity : public Entity
{
 public:
	UserEntity(std::string userID) : UserID(userID){};
	std::string UserID;

	virtual std::string GetCreationCommand();
};
