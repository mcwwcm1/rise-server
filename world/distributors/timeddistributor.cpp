// File: distributor.cpp
// Purpose: Implements timeddistributor.h

#include "timeddistributor.h"

void TimedDistributor::TryDistribute(Double3 position)
{
	if (clock() - _clock > Delay) {
		_clock = clock();

		Distribute(position);
	}
}