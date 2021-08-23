// File: bugswarmdistributor.cpp
// Purpose: Implements bugswarmdistributor.h

#include "bugswarmdistributor.h"
#include "utilities.h"

void BugSwarmDistributor::Distribute(std::vector<Double3> positions)
{
	for (size_t p = 0; p < (positions.size() * MaxPerPosition) - Entities.size(); p++) {
		for (uint i = 0; i < MaxSpawnAttempts; i++) {
			Double3 direction = Random::RandomInUnitSphere();
			Double3 targetPos = positions[p] + (direction.Normalized() * MinDistance) + (direction * (Range - MinDistance));  // The random position within the allowed region

			// Check if this location is too close to other positions
			bool allowed = true;
			for (Double3 checkPos : positions) {
				if ((checkPos - targetPos).MagnitudeSquared() < MinDistance * MinDistance) {
					allowed = false;
					break;
				}
			}

			// Check if this location is too close to other swarms
			for (auto entity : Entities) {
				if ((entity->Position - targetPos).MagnitudeSquared() < Spacing * Spacing) {
					allowed = false;
					break;
				}
			}

			if (allowed) {
				// Spawn the swarm here and don't attempt again
				BugSwarmEntity* e = new BugSwarmEntity();
				e->BugID          = BugID;
				e->SetLocalPosition(targetPos);
				e->SpawnRate  = 1;
				e->UpdateRate = 2;
				e->Capacity   = 25;
				e->MaxBugs    = 10;
				e->Range      = Double3(5, 5, 5);
				e->MinSpeed   = .5;
				e->MaxSpeed   = 3;
				e->SubmitAllParameters();
				AddEntity(e);
				break;
			}
		}
	}
}