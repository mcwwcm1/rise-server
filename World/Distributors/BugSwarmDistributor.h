// File: bugswarmddistributor.h
// Purpose: A distributor that attempts to distribute bugs around the players

#pragma once

#include "TimedDistributor.h"
#include "World/Entities/BugSwarmEntity.h"

class BugSwarmDistributor : public TimedDistributor
{
 public:
	BugSwarmDistributor(float range, float delay, float spacing, std::string bugID) : TimedDistributor(range, delay), Spacing(spacing), BugID(bugID){};

	float Spacing            = 15;  // The minimum spacing between swarms
	float MinDistance        = 15;  // The minimum distance from any given position
	int MaxPerPosition       = 5;   // The maximum number of entities within the range of a position
	int MaxSpawnPerIteration = 5;   // The maximum number of entities to spawn per "Distribute" method
	int MaxSpawnAttempts     = 10;
	std::string BugID;

	void virtual Distribute(std::vector<Double3> positions);
};