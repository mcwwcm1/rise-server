// File: timeddistributor.cpp
// Purpose: Implements timeddistributor.h

#include "timeddistributor.h"

void TimedDistributor::TryDistribute(std::vector<Double3> positions)
{
	if (clock() - _clock > Delay * 1000) {
		_clock = clock();
		Distribute(positions);
	}
}