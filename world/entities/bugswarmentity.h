// File: bugswarmentity.h
// Purpose: For spawning bug swarms in game around users

#include "entity.h"
#include "mysticmath/double3.h"
#include "mysticmath/double2.h"

class BugSwarmEntity : public Entity
{
 public:
	float SpawnRate, UpdateRate;
	Double3 Range;
	int MaxBugs;
	int Capacity;
	float MinSpeed, MaxSpeed;
	Double3 TargetPosition;

	BugSwarmEntity(std::string id);

	virtual void RunTick(float dt);

	virtual void SubmitAllParameters();
};