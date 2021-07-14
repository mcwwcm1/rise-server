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
	space.RunTick();
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
	
	printf("Parsed Physics Tick");
}

void SetThrottle()
{
	airship.throttle = argumentBuffer.get().var.fval;
	printf("Physics tick was run\n");
}

void SetThrottleParser(std::string& arguments)
{
	float throttle = stof(arguments);

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.put(SetThrottle);

	// Put test element on the argument buffer
	PrimaryArgument throttleArg;
	throttleArg.type = PrimaryArgument::is_double;
	throttleArg.var.fval = throttle;
	argumentBuffer.put(throttleArg);

	// Unlock buffers
	bufferAccessMutex.unlock();
	
	printf("Parsed Physics Tick");
}

void SetPitch()
{
	airship.pitch = argumentBuffer.get().var.fval;
	printf("Physics tick was run\n");
}

void SetPitchParser(std::string& arguments)
{
	float pitch = stof(arguments);

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.put(SetPitch);

	// Put test element on the argument buffer
	PrimaryArgument pitchArg;
	pitchArg.type = PrimaryArgument::is_double;
	pitchArg.var.fval = pitch;
	argumentBuffer.put(pitchArg);

	// Unlock buffers
	bufferAccessMutex.unlock();
	
	printf("Parsed Physics Tick");
}

void SetYaw()
{
	airship.pitch = argumentBuffer.get().var.fval;
	printf("Physics tick was run\n");
}

void SetYawParser(std::string& arguments)
{
	float yaw = stof(arguments);

	// Lock the buffers to safely write to them
	bufferAccessMutex.lock();

	// Put function pointer
	functionBuffer.put(SetYaw);

	// Put test element on the argument buffer
	PrimaryArgument yawArg;
	yawArg.type = PrimaryArgument::is_double;
	yawArg.var.fval = yaw;
	argumentBuffer.put(yawArg);

	// Unlock buffers
	bufferAccessMutex.unlock();
	
	printf("Parsed Physics Tick");
}

#endif