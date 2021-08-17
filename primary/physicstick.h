// File: physicstick.h
// Purpose: Sets physics parameters that are sent from Neos

#ifndef PHYSICSTICK_H
#define PHYSICSTICK_H

#include <stdio.h>
#include <string>

#include "../physics/physicsspace.h"
#include "../physics/airship.h"
#include "../physics/shapes/sphereshape.h"

void RegisterAirship()
{
	std::string* userString = argumentBuffer.get().var.sval;

	// No airship associated with this user, create new
	airships[*userString] = new Airship(space);

	delete userString; // Delet the string to avoid memory leak
}

void RegisterAirshipParser(std::string& arguments)
{
	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.put(RegisterAirship);

	// Put arguments
	argumentBuffer.put(PrimaryArgument(new std::string(arguments)));

	// Unlock buffers
	bufferAccessMutex.unlock();
}

void RegisterStaticCollider()
{
	Shape* colliderShape = new SphereShape(argumentBuffer.get().var.fval);
	std::string* positionString = argumentBuffer.get().var.sval;
	colliderShape->position = double3FromString(*positionString);
	delete positionString;
	// No airship associated with this user, create new
	space->RegisterStaticCollider(colliderShape);
}

void RegisterStaticColliderParser(std::string& arguments)
{
	std::string radiusString = arguments.substr(0, arguments.find(' '));
	std::string positionString = arguments.substr(arguments.find(' '));

	float radius = stof(radiusString);

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.put(RegisterStaticCollider);

	// Put arguments
	argumentBuffer.put(PrimaryArgument(stof(arguments)));
	argumentBuffer.put(PrimaryArgument(new std::string(positionString)));

	// Unlock buffers
	bufferAccessMutex.unlock();
}

// setthrottle <airshipID> <throttle value between 0 and 1>
void SetThrottle()
{
	std::string* airshipID = argumentBuffer.get().var.sval;
	airships[*airshipID]->throttle = argumentBuffer.get().var.fval;
	delete airshipID;
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

// addForce <airshipID> <force>
void AddForce()
{
	std::string* airshipID = argumentBuffer.get().var.sval;
	std::string* force = argumentBuffer.get().var.sval;
	std::string* position = argumentBuffer.get().var.sval;
	
	cout<<"ADDING FORCE "<< *airshipID << " Force: " << *force << " Position: " << *position << "\n";

	airships[*airshipID]->rigidbody->AddForceAtPosition(double3FromString(*force), double3FromString(*position));
	
	delete airshipID;
	delete force;
	delete position;
}

void AddForceParser(std::string& arguments)
{
	size_t separator1 = arguments.find('|');
	size_t separator2 = arguments.find('|', separator1 + 1);

	string force = arguments.substr(separator1 + 1, separator2);
	string position = arguments.substr(separator2 + 1);

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();
 
	// Put function pointer
	functionBuffer.put(AddForce);

	// Put argument
	argumentBuffer.put(PrimaryArgument(new std::string(arguments.substr(0, separator1))));
	argumentBuffer.put(new std::string(force));
	argumentBuffer.put(new std::string(position));

	// Unlock buffers
	bufferAccessMutex.unlock();
}



#endif