// File: bugswarmentity.cpp
// Purpose: Implements bugswarmentity.h

#include "bugswarmentity.h"

BugSwarmEntity::BugSwarmEntity() : Entity() {}

std::string BugSwarmEntity::GetCreationCommand()
{
	return "SpawnEntity " + BugID + "|" + Position.ToString() + "|";
}

void BugSwarmEntity::RunTick(float dt) {}

void BugSwarmEntity::SubmitAllParameters()
{
	SubmitChange("BSSpawnRate", std::to_string(SpawnRate));
	SubmitChange("BSUpdateRate", std::to_string(UpdateRate));
	SubmitChange("BSRange", Range.ToString());
	SubmitChange("BSMax", std::to_string(MaxBugs));
	SubmitChange("BSCapacity", std::to_string(Capacity));
	SubmitChange("BSSpeed", Double2(MinSpeed, MaxSpeed).ToString());
	SubmitChange("BSTarget", TargetPosition.ToString());
}