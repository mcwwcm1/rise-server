// File: worldtick.h
// Purpose: For ticking everything :)

#include "../world/world.h"
#include "globals.h"

void WorldTick()
{
	// Tick physics
	World::Singleton->Space->RunTick();

	for (auto entity : World::Singleton->Entities) {
		if (entity.second->Dirty) {
			std::string* changes =
					new std::string("ChangeTable " + entity.second->ID + " ");
			for (auto change : entity.second->ChangeTable) {
				*changes += change.first + "|" + change.second + "|";
			}

			if (entity.second->Owner == nullptr) {
				printf(
						"Unable to send changetable due to the owner of the entity (%s) not being assigned\n",
						entity.second->ID.c_str());
				continue;
			}
			if (registeredUsers.find(*entity.second->Owner) ==
			    registeredUsers.end()) {
				printf(
						"Unable to send changetable due to the owner of the entity (%s) not being a registered user (%s)\n",
						entity.second->ID.c_str(),
						entity.second->Owner->c_str());
				continue;
			}

			Send(entity.second->Owner, changes);
			entity.second->ChangeTable.clear();
			entity.second->Dirty = false;
		}
	}
}
