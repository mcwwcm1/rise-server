// File: userentity.cpp
// Purpose: Implements userentity.h

#include "userentity.h"
#include <algorithm>

UserEntity::UserEntity(std::string userID) : Entity()
{
	UserID = userID;

	UserInventory = new Inventory();

	UserInventory->ItemAddedCallback   = [&](ItemStack is) { InventoryItemAdded(is); };
	UserInventory->ItemRemovedCallback = [&](ItemStack is) { InventoryItemRemoved(is); };
	UserInventory->ItemChangedCallback = [&](ItemStack is) { InventoryItemChanged(is); };

	UserInventory->AddItem("starterNet");
	UserInventory->AddItem("starterGrapple");
	UserInventory->AddItem("starterZap");
	UserInventory->AddItem("flareLauncher");

	// Create equipment slots that accept "Equippable" items
	EquipmentSlots.reserve(2);
	EquipmentSlots.emplace_back(std::vector<std::string>({"equippable"}));
	EquipmentSlots.emplace_back(std::vector<std::string>({"equippable"}));
}

void UserEntity::AddQupies(size_t amount)
{
	Qupies += amount;
	SubmitChange("QupyCount", std::to_string(amount));
}

bool UserEntity::EquipItem(std::string itemID, size_t slotIndex)
{
	if (slotIndex + 1 > EquipmentSlots.size()) {
		printf("Unable to equip item to slot with index %lu\n", slotIndex);
		return false;
	}

	if (!UserInventory->HasItem(itemID))
		return false;

	ItemInfo* item = ItemInfo::GetItemByID(itemID);

	if (item == nullptr)
		return false;

	EquipmentSlot& slot = EquipmentSlots[slotIndex];

	bool allowed = false;
	for (std::string slotTag : slot.AllowedTags) {
		for (std::string itemTag : item->Tags) {
			if (slotTag == itemTag) {
				allowed = true;
				break;
			}
		}
		if (allowed == true)
			break;
	}

	if (!allowed)
		return false;

	slot.EquippedItem = itemID;
	return true;
}

void UserEntity::DequipItem(size_t slotIndex)
{
	if (slotIndex + 1 > EquipmentSlots.size())
		return;

	EquipmentSlots[slotIndex].EquippedItem = "";
}

bool UserEntity::HasItemEquipped(std::string itemID)
{
	for (auto equipment : EquipmentSlots)
		if (itemID == equipment.EquippedItem)
			return true;

	return false;
}

void UserEntity::InventoryItemAdded(ItemStack item)
{
	SubmitChange("InvAdd", item.Item->ItemID + ":" + std::to_string(item.StackSize), false);
}

void UserEntity::InventoryItemRemoved(ItemStack item)
{
	SubmitChange("InvRemove", item.Item->ItemID, false);
}

void UserEntity::InventoryItemChanged(ItemStack item)
{
	SubmitChange("InvChange", item.Item->ItemID + ":" + std::to_string(item.StackSize), false);
}

std::string UserEntity::GetCreationCommand()
{
	return "RegisterUser " + ID + "|";
}