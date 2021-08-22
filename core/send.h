// File: send.h
// Purpose: Implements sending data to the headless

#pragma once

void Send(std::string* sendString)
{
	// Invoke the send function of the headless' websocket session
	headlessSession->Send(
			boost::make_shared<std::string const>(std::move(*sendString)));
	// Debug
	//printf("Sent: %s\n", (*sendString).c_str());
}

void Send(const std::string* userID, const std::string* sendString)
{
	// Key is not present
	if (registeredUsers.find(*userID) == registeredUsers.end()) {
		printf(("Requested userID (" + *userID + ") not found\n").c_str());
	} else {
		// Invoke the send function of the headless' websocket session
		registeredUsers[*userID]->Send(
				boost::make_shared<std::string const>(std::move(*sendString)));
		// Debug
		//printf(("Sent: " + *sendString + " To: " + *userID + "\n").c_str());
	}
}
