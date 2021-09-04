// File: send.h
// Purpose: Implements sending data to the headless

#pragma once

#include <string>

#include "Core/Session.h"

void Send(const std::string& sendString);

void Send(const std::string& userID, const std::string& sendString);