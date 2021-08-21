// File: worldtick.h
// Purpose: For ticking everything :)

#include "globals.h"
#include "../world/world.h"

void WorldTick()
{
	// Tick physics
	World::singleton->space->RunTick();

	for (auto entity : World::singleton->entities)
	{
		if(entity.second->dirty)
		{
			std::string* changes = new std::string("changeTable ");
			for (auto change : entity.second->changeTable)
			{
				*changes += change.first + "|" + change.second + "|";
			}

			Send(entity.second->owner, changes);
			entity.second->changeTable.clear();
			entity.second->dirty = false;
		}
	}
}
