// File: physicstick.h
// Purpose: Sets physics parameters that are sent from Neos

#ifndef PHYSICSTICK_H
#define PHYSICSTICK_H

#include <stdio.h>
#include <string>

#include "../physics/physicsspace.h"
#include "../physics/airship.h"

PhysicsSpace* space = new PhysicsSpace(20);
std::unordered_map<std::string, Airship*> airships;

void PhysicsTick()
{
	std::string* userString = argumentBuffer.get().var.sval;

	if(airships.find(*userString) == airships.end())
	{
		// No airship associated with this user, create new
		airships[*userString] = new Airship(space);
	}

	Airship* airship = airships[*userString];
	airship->RunTick();
	airship->rigidbody->RunTick(1.0f/20.0f);

	space->CheckCollision();

	std::string response = "P" + airship->GetPosition().str() + "R" + airship->GetRotation().str();

	// Call Send function with string pointer from the argument buffer
	Send(userString, &response);

	delete userString; // Delet the string to avoid memory leak
}

void PhysicsTickParser(std::string& arguments)
{
	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.put(PhysicsTick);

	// Put arguments
	argumentBuffer.put(PrimaryArgument(new std::string(arguments)));

	// Unlock buffers
	bufferAccessMutex.unlock();
}

// setthrottle <airshipID> <throttle value between 0 and 1>
void SetThrottle()
{
	std::string* airshipID = argumentBuffer.get().var.sval;
	airships[*airshipID]->throttle = argumentBuffer.get().var.fval;
	delete airshipID;

	printf("I'm on the telly\n");
}

void SetThrottleParser(std::string& arguments)
{
	size_t spaceIndex = arguments.find(' ');
	float throttle = stof(arguments.substr(spaceIndex + 1));

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.put(SetThrottle);
	
	// Put argument
	argumentBuffer.put(PrimaryArgument(new std::string(arguments.substr(0, spaceIndex))));
	argumentBuffer.put(PrimaryArgument(throttle));

	// Unlock buffers
	bufferAccessMutex.unlock();
}

// setpitch <airshipID> <pitch value between 0 and 1>
void SetPitch()
{
	std::string* airshipID = argumentBuffer.get().var.sval;
	airships[*airshipID]->pitch = argumentBuffer.get().var.fval;
	delete airshipID;
}

void SetPitchParser(std::string& arguments)
{
	size_t spaceIndex = arguments.find(' ');
	float pitch = stof(arguments.substr(spaceIndex + 1)) * 2 - 1;

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.put(SetPitch);

	// Put argument
	argumentBuffer.put(PrimaryArgument(new std::string(arguments.substr(0, spaceIndex))));
	argumentBuffer.put(PrimaryArgument(pitch));

	// Unlock buffers
	bufferAccessMutex.unlock();
}

// setyaw <airshipID> <yaw value between 0 and 1>
void SetYaw()
{
	std::string* airshipID = argumentBuffer.get().var.sval;
	airships[*airshipID]->yaw = argumentBuffer.get().var.fval;
	delete airshipID;
}

void SetYawParser(std::string& arguments)
{
	size_t spaceIndex = arguments.find(' ');
	float yaw = stof(arguments.substr(spaceIndex + 1)) * 2 - 1;

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.put(SetYaw);

	// Put argument
	argumentBuffer.put(PrimaryArgument(new std::string(arguments.substr(0, spaceIndex))));
	argumentBuffer.put(PrimaryArgument(yaw));

	// Unlock buffers
	bufferAccessMutex.unlock();
}

#endif