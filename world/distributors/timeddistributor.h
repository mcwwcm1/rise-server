// File: timeddistributor.h
// Purpose: A distributor that attempts to distribute entities at fixed time intervals

#include "distributor.h"
#include "mysticmath/double3.h"

#include <time.h>

class TimedDistributor : public Distributor
{
 private:
	clock_t _clock;

 public:
	TimedDistributor(float range, float delay) : Distributor(range), Delay(delay){};

	float Delay;

	virtual void TryDistribute(Double3 position);
	virtual void Distribute(Double3 position) = 0;
};