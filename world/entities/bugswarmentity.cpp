// File: bugswarmentity.cpp
// Purpose: Implements bugswarmentity.h

#include "bugswarmentity.h"
#include "world/world.h"

BugSwarmEntity::BugSwarmEntity() : Entity() {}

BugSwarmEntity::~BugSwarmEntity()
{
	World::Singleton->UnregisterEntity(this);
}

std::string BugSwarmEntity::GetCreationCommand()
{
	return "SpawnEntity " + BugID + "|" + ID + "|";
}

void BugSwarmEntity::RunTick(float dt)
{
	if (TargetEntity == nullptr) {
		// Attract entities
		for (auto u : World::Singleton->Users) {
			if (u.second->HasItemEquipped("starterZap"))  // BAD! Use property system to find how much an item attracts bugs
			{
				if ((Position - u.second->Position).Magnitude() < 30) {
					SetTargetEntity(u.second);
					break;
				}
			}
		}
	} else {
		UserEntity* user = dynamic_cast<UserEntity*>(TargetEntity);
		if (user != nullptr) {
			if (!user->HasItemEquipped("starterZap")) {
				UntargetEntity();
			}
		}
	}

	if (TargetEntity != nullptr) {
		SetTargetPosition(TargetEntity->Position);
	}
}

void BugSwarmEntity::CatchBug(UserEntity* user)
{
	Capacity--;
	SubmitChange("BSCapacity", std::to_string(Capacity));

	user->UserInventory->AddItem(BugID);

	if (Capacity <= 0)
		delete this;
}

void BugSwarmEntity::SetTargetPosition(const Double3& position)
{
	TargetPosition = GlobalPointToLocal(position);
	SubmitChange("BSTarget", TargetPosition.ToString());
}

void BugSwarmEntity::SetTargetEntity(Entity* entity)
{
	SetRange(Double3(1, .5, 1));
	TargetEntity = entity;
}

void BugSwarmEntity::UntargetEntity()
{
	SetRange(Double3(5, 5, 5));
	TargetEntity = nullptr;
}

void BugSwarmEntity::SetRange(const Double3& range)
{
	Range = range;
	SubmitChange("BSRange", Range.ToString());
}

void BugSwarmEntity::SubmitAllParameters()
{
	SubmitChange("BSSpawnRate", std::to_string(SpawnRate));
	SubmitChange("BSUpdateRate", std::to_string(UpdateRate));
	SubmitChange("BSRange", Range.ToString());
	SubmitChange("BSMax", std::to_string(MaxBugs));
	SubmitChange("BSCapacity", std::to_string(Capacity));
	SubmitChange("BSSpeed", Double2(MinSpeed, MaxSpeed).ToString());
	SubmitChange("BSTarget", TargetPosition.ToString());
}