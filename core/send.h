// File: send.h
// Purpose: Implements sending data to the headless

#pragma once

#include <string>

#include "core/session.h"

void Send(const std::string& sendString)
{
	Session* session = Session::GetHeadless();
	if (session != nullptr) {
		// Invoke the send function of the headless' websocket session
		session->Send(sendString);
	}
}

void Send(const std::string& userID, const std::string& sendString)
{
	Session* session = Session::GetUserSession(userID);
	if (session != nullptr) {
		// Invoke the send function of the headless' websocket session
		session->Send(sendString);
	} else {
		printf("Tried to send message to unregistered user: %s\n", userID.c_str());
	}
}
