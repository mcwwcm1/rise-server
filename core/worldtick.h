// File: worldtick.h
// Purpose: For ticking everything :)

#pragma once

#include "../world/world.h"
#include "core/session.h"

void WorldTick()
{
	// Tick physics
	World::Singleton->Space->RunTick();

	for (auto entity : World::Singleton->Entities) {
		if (entity.second->Dirty) {
			std::string changes = "ChangeTable " + entity.second->ID + " ";
			for (auto change : entity.second->ChangeTable) {
				changes += change.first + "|" + change.second + "|";
			}

			if (!entity.second->Owner.has_value()) {
				printf(
						"Unable to send changetable due to the owner of the entity (%s) not being assigned\n",
						entity.second->ID.c_str());
				continue;
			}
			if (Session::GetUserSession(entity.second->Owner.value()) == nullptr) {
				printf(
						"Unable to send changetable due to the owner of the entity (%s) not being a registered user (%s)\n",
						entity.second->ID.c_str(),
						entity.second->Owner.value().c_str());
				continue;
			}

			Send(entity.second->Owner.value(), changes);
			entity.second->ChangeTable.clear();
			entity.second->Dirty = false;
		}
	}
}
