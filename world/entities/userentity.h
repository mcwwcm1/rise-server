// File: userentity.h
// Purpose: Keeps track of user information and supplies useful methods for changing properties of a user in game

#pragma once

#include "entity.h"
#include "data/items.h"
#include <vector>

struct EquipmentSlot {
 public:
	std::vector<std::string> AllowedTags;
	std::string EquippedItem;

	EquipmentSlot(std::vector<std::string> tags) : AllowedTags(tags){};
};

class UserEntity : public Entity
{
 public:
	UserEntity(std::string userID);

	bool Initialized = false;  // Only send data to the DB when this is true!

	std::string UserID;

	Inventory* UserInventory;
	std::vector<EquipmentSlot> EquipmentSlots;

	size_t Qupies;

	void AddQupies(size_t amount);
	bool EquipItem(std::string itemID, size_t slotIndex);
	void DequipItem(size_t slotIndex);
	bool HasItemEquipped(std::string itemID);

	void InventoryItemAdded(ItemStack item);
	void InventoryItemRemoved(ItemStack item);
	void InventoryItemChanged(ItemStack item);

	void SendInventoryData();

	virtual std::string GetCreationCommand();
};
