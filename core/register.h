// File: register.h
// Purpose: Registers the current user as the headless session

#pragma once

void RegisterUser(std::string& userID, Session* sessionPointer)
{
	std::lock_guard<std::mutex> lck(headlessPointerMutex);
	registeredUsers[userID] = sessionPointer;
	printf(("Registered: " + userID + "\n").c_str());
}

void UnRegisterUser(std::string& userID)
{
	std::lock_guard<std::mutex> lck(headlessPointerMutex);
	registeredUsers.erase(userID);
	printf(("UnRegistered: " + userID + "\n").c_str());
}
