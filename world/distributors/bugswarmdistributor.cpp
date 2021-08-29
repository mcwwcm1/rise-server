// File: bugswarmdistributor.cpp
// Purpose: Implements bugswarmdistributor.h

#include "bugswarmdistributor.h"
#include "utilities.h"
#include "world/world.h"

void BugSwarmDistributor::Distribute(std::vector<Double3> positions)
{
	// Spawn entities
	for (size_t p = 0; p < (positions.size() * MaxPerPosition) - Entities.size(); p++) {
		for (int i = 0; i < MaxSpawnAttempts; i++) {
			Double3 direction = Random::RandomInUnitSphere();

			Double3 targetPos = positions[p % positions.size()] + (direction.Normalized() * MinDistance) + (direction * (Range - MinDistance));  // The random position within the allowed region

			// Check if this location is too close to positions
			bool allowed = true;
			for (Double3 checkPos : positions) {
				if ((checkPos - targetPos).MagnitudeSquared() < MinDistance * MinDistance) {
					allowed = false;
					break;
				}
			}

			if (!allowed)
				continue;

			// Check if the position has reached it's allowed limit
			int count = 0;
			for (auto entity : Entities) {
				if ((positions[p % positions.size()] - entity->Position).MagnitudeSquared() < Range * Range) {
					count++;
					if (count >= MaxPerPosition) {
						allowed = false;
						break;
					}
				}
			}

			if (!allowed)
				continue;

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
				e->SpawnRate  = 5;
				e->UpdateRate = .5;
				e->Capacity   = 25;
				e->MaxBugs    = 5;
				e->Range      = Double3(10, 5, 10);
				e->MinSpeed   = .2;
				e->MaxSpeed   = .8;
				e->SubmitAllParameters();
				AddEntity(e);
				break;
			}
		}
	}
}