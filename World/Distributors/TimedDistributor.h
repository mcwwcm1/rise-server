// File: timeddistributor.h
// Purpose: A distributor that attempts to distribute entities at fixed time intervals

#pragma once

#include "Distributor.h"
#include "MysticMath/Double3.h"

#include <time.h>

class TimedDistributor : public Distributor
{
 private:
	clock_t _clock = 0;

 public:
	TimedDistributor(float range, float delay) : Distributor(range), Delay(delay){};

	float Delay = 0;

	virtual void TryDistribute(std::vector<Double3> positions);
	virtual void Distribute(std::vector<Double3> positions) = 0;
};