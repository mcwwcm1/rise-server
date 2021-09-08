// File: userentity.cpp
// Purpose: Implements userentity.h

#include "UserEntity.h"

#include <algorithm>
#include "Core/Database.h"

UserEntity::UserEntity(std::string userID) : Entity()
{
	printf("Creating user %s\n", userID.c_str());
	UserID = userID;

	UserInventory           = new Inventory();
	UserInventory->capacity = 1000000000;  // Practically infinite space for now.

	UserInventory->ItemAddedCallback   = [&](ItemStack is) { InventoryItemAdded(is); };
	UserInventory->ItemRemovedCallback = [&](ItemStack is) { InventoryItemRemoved(is); };
	UserInventory->ItemChangedCallback = [&](ItemStack is) { InventoryItemChanged(is); };

	SendClearInventory();

	UserInventory->AddItem("StarterNet");
	UserInventory->AddItem("StarterGrapple");
	UserInventory->AddItem("StarterZap");
	UserInventory->AddItem("FlareLauncher");

	// Create equipment slots that accept "Equippable" items
	EquipmentSlots.reserve(2);
	EquipmentSlots.emplace_back(std::vector<std::string>({"equippable"}));
	EquipmentSlots.emplace_back(std::vector<std::string>({"equippable"}));

	Database::CreatePlayer(UserID, 0, "starterIsland");
	Qupies = Database::GetUserQpCount(UserID);
	SubmitQupies();
}

void UserEntity::AddQupies(size_t amount)
{
	Qupies += amount;
	Database::AlterUserQpCount(UserID, amount);
	SubmitChange("QupyCount", std::to_string(Qupies));
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
	printf("Adding %s to %s\n", item.Item->ItemID.c_str(), UserID.c_str());
	SubmitChange("InvAdd", item.Item->ItemID + ":" + std::to_string(item.StackSize), false);
}

void UserEntity::InventoryItemRemoved(ItemStack item)
{
	printf("Removing %s from %s\n", item.Item->ItemID.c_str(), UserID.c_str());
	SubmitChange("InvRemove", item.Item->ItemID, false);
}

void UserEntity::InventoryItemChanged(ItemStack item)
{
	SubmitChange("InvChange", item.Item->ItemID + ":" + std::to_string(item.StackSize), false);
}

void UserEntity::ResubmitFullInventory()
{
	SendClearInventory();
	for (auto i : UserInventory->Items) {
		SubmitChange("InvAdd", i.second.Item->ItemID + ":" + std::to_string(i.second.StackSize), false);
	}
}

void UserEntity::SubmitQupies() { SubmitChange("QupyCount", std::to_string(Qupies)); }

void UserEntity::SendClearInventory()
{
	SubmitChange("InvClear", "");
}

std::string UserEntity::GetCreationCommand()
{
	return "RegisterUser " + ID + "|";
}