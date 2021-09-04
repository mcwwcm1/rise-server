// File: bugswarmentity.h
// Purpose: For spawning bug swarms in game around users

#pragma once

#include "World/Entities/Entity.h"
#include "MysticMath/Double3.h"
#include "MysticMath/Double2.h"
#include "World/Entities/UserEntity.h"

class BugSwarmEntity : public Entity
{
 public:
	std::string BugID;
	float SpawnRate, UpdateRate = 0;
	Double3 Range;
	int MaxBugs  = 0;  // The maximum bugs allowed out at any given time
	int Capacity = 0;  // The total number of bugs to spawn from this swarm
	float MinSpeed, MaxSpeed = 0;
	Double3 TargetPosition;          // (Local, relative to entity) Bugs will trend towards this location
	Entity* TargetEntity = nullptr;  // Will override the TargetPosition

	BugSwarmEntity();

	virtual ~BugSwarmEntity();

	virtual std::string GetCreationCommand() override;

	virtual void RunTick(float dt) override;
	virtual void CatchBug(UserEntity* user);
	virtual void SetTargetPosition(const Double3& position);
	virtual void SetTargetEntity(Entity* entity);
	virtual void UntargetEntity();
	virtual void SetRange(const Double3& range);
	virtual void SubmitAllParameters();
};