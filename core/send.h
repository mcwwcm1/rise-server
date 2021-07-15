// File: send.h
// Purpose: Implements sending data to the headless

#ifndef SEND_H
#define SEND_H

void Send(std::string* sendString)
{
	// Invoke the send function of the headless' websocket session
	headlessSession->send(boost::make_shared<std::string const>(std::move(*sendString)));
	// Debug
	printf(("Sent: " + *sendString + "\n").c_str());
}

void Send(std::string* sendString, std::string* userID)
{
	// Invoke the send function of the headless' websocket session
	registeredUsers[*userID]->send(boost::make_shared<std::string const>(std::move(*sendString)));
	// Debug
	printf(("Sent: " + *sendString + " To: " + *userID + "\n").c_str());
}

#endif
