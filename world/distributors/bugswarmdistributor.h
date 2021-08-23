// File: bugswarmddistributor.h
// Purpose: A distributor that attempts to distribute bugs around the players

#include "timeddistributor.h"

class BugSwarmDistributor : public TimedDistributor
{
 public:
	BugSwarmDistributor(float range, float delay, float spacing, std::string bugID) : TimedDistributor(range, delay), Spacing(spacing), BugID(bugID){};

	float Spacing;
	std::string BugID;

	void virtual Distribute(Double3 position);
};