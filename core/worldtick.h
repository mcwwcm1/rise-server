// File: worldtick.h
// Purpose: For ticking everything :)

#include "globals.h"

void WorldTick()
{
	// Tick airships
	for(auto& as : airships)
		as.second->RunTick();

	// Tick physics
	space->RunTick();

	// Send airship data to Neos
	for(auto& as : airships){
			std::string response = "settransform P" + as.second->GetPosition().str() + "R" + as.second->GetRotation().str();
			Send(&as.first, &response);
	}
}
