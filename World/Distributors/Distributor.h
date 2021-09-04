// File: distributor.h
// Purpose: A distributor will be in charge of distributing entities around the world

#pragma once

#include "MysticMath/Double3.h"
#include <vector>
#include "World/Entities/Entity.h"

class Distributor
{
 public:
	std::vector<Entity*> Entities;
	float Range = 200;

	Distributor(float range) : Range(range){};

	virtual void TryDistribute(std::vector<Double3> positions) = 0;
	virtual void Distribute(std::vector<Double3> positions)    = 0;
	virtual void CleanupDistant(std::vector<Double3> positions);
	virtual void AddEntity(Entity* entity);
	virtual void DestroyEntity(Entity* entity);
};