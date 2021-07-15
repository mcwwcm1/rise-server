// File: physicstick.h
// Purpose: Sets physics parameters that are sent from Neos

#ifndef PHYSICSTICK_H
#define PHYSICSTICK_H

#include <stdio.h>
#include <string>

#include "../physics/physicsspace.h"
#include "../physics/airship.h"

PhysicsSpace space = PhysicsSpace(20);
Airship airship = Airship(space);

void PhysicsTick()
{
	airship.RunTick();
	airship.rigidbody.RunTick(1.0f/20.0f);
	//space.RunTick();

	std::string response = "P" + airship.GetPosition().str() + "R" + airship.GetRotation().str();

	// Call Send function with string pointer from the argument buffer
	Send(&response);

	printf("Physics tick was run\n");
}

void PhysicsTickParser(std::string& arguments)
{
	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.put(PhysicsTick);

	// Unlock buffers
	bufferAccessMutex.unlock();
	
	printf("Parsed Physics Tick\n");
}

void SetThrottle()
{
	airship.throttle = argumentBuffer.get().var.fval;
	printf("Set throttle to %1.2f \n", airship.throttle);
}

void SetThrottleParser(std::string& arguments)
{
	float throttle = stof(arguments);

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.put(SetThrottle);
	
	// Put argument
	argumentBuffer.put(PrimaryArgument(throttle));

	// Unlock buffers
	bufferAccessMutex.unlock();
	
	printf("Parsed Set Throttle\n");
}

void SetPitch()
{
	airship.pitch = argumentBuffer.get().var.fval;
	printf("Set Pitch was run\n");
}

void SetPitchParser(std::string& arguments)
{
	float pitch = stof(arguments) * 2 - 1;

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.put(SetPitch);

	// Put argument
	argumentBuffer.put(PrimaryArgument(pitch));

	// Unlock buffers
	bufferAccessMutex.unlock();
	
	printf("Parsed Set Pitch\n");
}

void SetYaw()
{
	airship.yaw = argumentBuffer.get().var.fval;
	printf("Set Yaw was run\n");
}

void SetYawParser(std::string& arguments)
{
	float yaw = stof(arguments) * 2 - 1;

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.put(SetYaw);

	// Put argument
	argumentBuffer.put(PrimaryArgument(yaw));

	// Unlock buffers
	bufferAccessMutex.unlock();
	
	printf("Parsed Set Yaw\n");
}

#endif