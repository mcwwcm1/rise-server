// File: items.cpp
// Purpose: Implements items.h

#include "Items.h"
#include "Utilities.h"
#include "Core/Send.h"

std::unordered_map<std::string, ItemInfo*> ItemInfo::RegisteredItems;

ItemInfo* ItemInfo::GetItemByID(const std::string& id)
{
	auto i = RegisteredItems.find(id);
	return i != RegisteredItems.end() ? i->second : nullptr;
}

// Not staying, DB FIX :D This comment means nothing. I'm numb.
void AddDummyItemData()
{
	ItemInfo::RegisterItem(new ItemInfo("StarterNet", std::vector<std::string>({"equippable", "bugNet"}), false, 25, 5));
	ItemInfo::RegisterItem(new ItemInfo("StarterGrapple", std::vector<std::string>({"equippable", "grapple"}), false, 25, 8));
	auto zapzap = new ItemInfo("StarterZap", std::vector<std::string>({"equippable", "bugZapper"}), false, 25, 5);
	zapzap->Properties.push_back(new ItemProperty("AttractsBugs", 1));
	ItemInfo::RegisterItem(zapzap);
	ItemInfo::RegisterItem(new ItemInfo("FlareLauncher", std::vector<std::string>({"equippable", "flareLauncher"}), false, 30, 10));
	ItemInfo::RegisterItem(new ItemInfo("WobbleFly", std::vector<std::string>({"bug"}), true, 5, 1));
	ItemInfo::RegisterItem(new ItemInfo("Crystallis", std::vector<std::string>({"bug"}), true, 8, 1));
}

void ItemInfo::RegisterItem(ItemInfo* item)
{
	RegisteredItems[item->ItemID] = item;
	printf("registered item %s\n", item->ItemID.c_str());
	// std::string tags              = VectorToString(item->Tags, '|');
	// Send("SetItemTags " + item->ItemID + "|" + tags + "|");
	// Send("SetItemStackable " + item->ItemID + "|" + std::to_string(item->Stackable) + "|");
}

size_t ItemStack::HeldCapacity() const { return Item->DalfitchDepth * StackSize; }
bool ItemStack::IsEmpty() const { return StackSize == 0; }

#pragma region inventory methods
bool Inventory::HasItems(const std::string& itemID, size_t amount) const
{
	auto pair = Items.find(itemID);
	if (pair != Items.end())
		if (pair->second.StackSize >= amount)
			return true;

	return false;
}
bool Inventory::HasItems(ItemInfo* item, size_t amount) const { return HasItems(item->ItemID, amount); }
bool Inventory::HasItem(const std::string& itemID) const { return HasItems(itemID, 1); }
bool Inventory::HasItem(ItemInfo* item) const { return HasItems(item->ItemID, 1); }

bool Inventory::CanHoldInventory(const Inventory& inventory) const { return RemainingCapacity() >= inventory.HeldCapacity(); }
bool Inventory::CanHoldItems(const ItemStack& items) const { return RemainingCapacity() >= items.HeldCapacity(); }
bool Inventory::CanHoldItems(const std::string& itemID, size_t amount) const { return RemainingCapacity() >= ItemInfo::GetItemByID(itemID)->DalfitchDepth * amount; }

bool Inventory::TransferItems(Inventory& inventory)
{
	for (auto& stack : inventory.Items) {
		size_t maxTransferAmount = MaxTransferAmount(stack.second);
		auto items               = inventory.TakeItems(stack.second.Item, maxTransferAmount);
		AddItems(items);
	}

	return !inventory.IsEmpty();
}
bool Inventory::TransferItems(ItemStack& items)
{
	size_t maxTransferAmount = MaxTransferAmount(items);
	items.StackSize -= maxTransferAmount;
	AddItems(items.Item, maxTransferAmount);
	return !items.IsEmpty();
}

void Inventory::AddItems(const Inventory& inventory)
{
	for (auto& item : inventory.Items)
		AddItems(item.second);
}
void Inventory::AddItems(const std::string& itemID, size_t amount) { AddItems(ItemStack(ItemInfo::GetItemByID(itemID), amount)); }
void Inventory::AddItems(ItemInfo* item, size_t amount) { AddItems(ItemStack(item, amount)); }
void Inventory::AddItems(const ItemStack& items)
{
	size_t maxTransferAmount = MaxTransferAmount(items);
	if (maxTransferAmount > 0) {
		auto item = Items.find(items.Item->ItemID);
		if (item != Items.end()) {
			item->second.StackSize += maxTransferAmount;
			if (ItemChangedCallback) ItemChangedCallback(item->second);
		} else {
			Items[items.Item->ItemID] = ItemStack(items.Item, maxTransferAmount);
			if (ItemAddedCallback) ItemAddedCallback(Items[items.Item->ItemID]);
		}
	}
}
void Inventory::AddItem(const std::string& itemID)
{
	ItemInfo* item = ItemInfo::GetItemByID(itemID);
	if (item != nullptr)
		AddItems(ItemStack(item));
}
void Inventory::AddItem(ItemInfo* item) { AddItems(ItemStack(item)); }

ItemStack Inventory::TakeItem(const std::string& itemID) { return TakeItems(ItemInfo::GetItemByID(itemID), 1); }
ItemStack Inventory::TakeItems(const std::string& itemID, size_t amount) { return TakeItems(ItemInfo::GetItemByID(itemID), amount); }
ItemStack Inventory::TakeItem(ItemInfo* item) { return TakeItems(item, 1); }
ItemStack Inventory::TakeItems(ItemInfo* item, size_t amount)
{
	if (!HasItem(item))
		return ItemStack(item, 0);

	ItemStack& itemStack = Items[item->ItemID];
	if (itemStack.StackSize > amount) {
		itemStack.StackSize -= amount;
		return ItemStack(item, amount);
	} else {
		size_t amount = itemStack.StackSize;
		if (ItemRemovedCallback) ItemRemovedCallback(itemStack);
		Items.erase(item->ItemID);
		return ItemStack(item, amount);
	}
}

size_t Inventory::RemainingCapacity() const { return capacity - HeldCapacity(); }
size_t Inventory::HeldCapacity() const
{
	size_t currentCount = 0;
	for (auto i : Items)
		currentCount += i.second.HeldCapacity();

	return currentCount;
}
bool Inventory::IsEmpty() const { return Items.size() == 0; }

size_t Inventory::MaxTransferAmount(const ItemInfo* item) const { return RemainingCapacity() / item->DalfitchDepth; }
size_t Inventory::MaxTransferAmount(const ItemStack& items) const { return std::min(MaxTransferAmount(items.Item), items.StackSize); }
#pragma endregion