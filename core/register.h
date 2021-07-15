// File: register.h
// Purpose: Registers the current user as the headless session

#ifndef REGISTER_H
#define REGISTER_H

void RegisterUser(std::string& userID, session* sessionPointer)
{
	std::lock_guard<std::mutex> lck(headlessPointerMutex);
	registeredUsers[userID] = sessionPointer;
}

void UnRegisterUser(std::string& userID)
{
	std::lock_guard<std::mutex> lck(headlessPointerMutex);
	registeredUsers.erase(userID);
}

#endif
