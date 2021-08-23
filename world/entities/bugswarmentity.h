// File: bugswarmentity.h
// Purpose: For spawning bug swarms in game around users

#include "entity.h"
#include "mysticmath/double3.h"
#include "mysticmath/double2.h"

class BugSwarmEntity : public Entity
{
 public:
	std::string BugID;
	float SpawnRate, UpdateRate;
	Double3 Range;
	int MaxBugs;   // The maximum bugs allowed out at any given time
	int Capacity;  // The total number of bugs to spawn from this swarm
	float MinSpeed, MaxSpeed;
	Double3 TargetPosition;  // (Local, relative to entity) Bugs will trend towards this location

	BugSwarmEntity();

	virtual std::string GetCreationCommand() override;

	virtual void RunTick(float dt);

	virtual void SubmitAllParameters();
};