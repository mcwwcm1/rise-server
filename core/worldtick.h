// File: worldtick.h
// Purpose: For ticking everything :)

#include "../world/world.h"
#include "globals.h"

void WorldTick()
{
	// Tick physics
	World::singleton->space->RunTick();

	for (auto entity : World::singleton->entities) {
		if (entity.second->dirty) {
			std::string* changes =
					new std::string("ChangeTable " + entity.second->id + " ");
			for (auto change : entity.second->changeTable) {
				*changes += change.first + "|" + change.second + "|";
			}

			if (entity.second->owner == nullptr) {
				printf(
						"Unable to send changetable due to the owner of the entity (%s) not being assigned\n",
						entity.second->id.c_str());
				continue;
			}
			if (registeredUsers.find(*entity.second->owner) ==
			    registeredUsers.end()) {
				printf(
						"Unable to send changetable due to the owner of the entity (%s) not being a registered user (%s)\n",
						entity.second->id.c_str(),
						entity.second->owner->c_str());
				continue;
			}

			Send(entity.second->owner, changes);
			entity.second->changeTable.clear();
			entity.second->dirty = false;
		}
	}
}
