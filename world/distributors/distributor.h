// File: distributor.h
// Purpose: A distributor will be in charge of distributing entities around the world

#include "mysticmath/double3.h"
#include "world/world.h"

#include <vector>

class Distributor
{
 public:
	std::vector<Entity*> Entities;
	float Range;

	Distributor(float range) : Range(range){};

	virtual void TryDistribute(Double3 position) = 0;
	virtual void Distribute(Double3 position)    = 0;
};