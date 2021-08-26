// File: items.cpp
// Purpose: Implements items.h

#include "items.h"

size_t ItemStack::HeldCapacity() const { return Item->DalfitchDepth * StackSize; }
bool ItemStack::IsEmpty() const { return StackSize == 0; }

#pragma region inventory methods
bool Inventory::HasItems(std::string itemID, size_t amount) const
{
	auto pair = Items.find(itemID);
	if (pair != Items.end())
		if (pair->second->StackSize >= amount)
			return true;

	return false;
}
bool Inventory::HasItems(ItemInfo* item, size_t amount) const { return HasItems(item->ItemID, amount); }
bool Inventory::HasItem(std::string itemID) const { return HasItems(itemID, 1); }
bool Inventory::HasItem(ItemInfo* item) const { return HasItems(item->ItemID, 1); }

ItemStack* Inventory::GetItemStack(std::string itemID) const
{
	auto i = Items.find(itemID);
	return i == Items.end() ? nullptr : i->second;
}
ItemStack* Inventory::GetItemStack(ItemInfo* item) const { return GetItemStack(item->ItemID); }

bool Inventory::CanHoldInventory(const Inventory& inventory) const { return RemainingCapacity() >= inventory.HeldCapacity(); }
bool Inventory::CanHoldItems(const ItemStack& items) const { return RemainingCapacity() >= items.HeldCapacity(); }

bool Inventory::TransferItems(Inventory& inventory)
{
	for (auto stack : inventory.Items) {
		size_t maxTransferAmount = MaxTransferAmount(*stack.second);
		auto items               = inventory.TakeItems(stack.second->Item, maxTransferAmount);
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
	for (auto item : inventory.Items)
		AddItems(*item.second);
}
void Inventory::AddItems(std::string itemID, size_t amount) { AddItems(ItemStack(GetItemByID(itemID), amount)); }
void Inventory::AddItems(ItemInfo* item, size_t amount) { AddItems(ItemStack(item, amount)); }
void Inventory::AddItems(const ItemStack& items)
{
	size_t maxTransferAmount = MaxTransferAmount(items);
	if (maxTransferAmount > 0) {
		auto item = Items.find(items.Item->ItemID);
		if (item != Items.end())
			item->second->StackSize += maxTransferAmount;
		else
			Items[items.Item->ItemID] = new ItemStack(items.Item, maxTransferAmount);
	}
}
void Inventory::AddItem(std::string itemID) { AddItems(ItemStack(GetItemByID(itemID))); }
void Inventory::AddItem(ItemInfo* item) { AddItems(ItemStack(item)); }

ItemStack Inventory::TakeItem(std::string itemID) { return TakeItems(GetItemByID(itemID), 1); }
ItemStack Inventory::TakeItems(std::string itemID, size_t amount) { return TakeItems(GetItemByID(itemID), amount); }
ItemStack Inventory::TakeItem(ItemInfo* item) { return TakeItems(item, 1); }
ItemStack Inventory::TakeItems(ItemInfo* item, size_t amount)
{
	if (!HasItem(item))
		return ItemStack(item, 0);

	ItemStack* itemStack = GetItemStack(item);
	if (itemStack->StackSize > amount) {
		itemStack->StackSize -= amount;
		return ItemStack(item, amount);
	} else {
		Items.erase(item->ItemID);
		return *itemStack;
	}
}

size_t Inventory::RemainingCapacity() const { return capacity - HeldCapacity(); }
size_t Inventory::HeldCapacity() const
{
	size_t currentCount;
	for (auto i : Items)
		currentCount += i.second->HeldCapacity();

	return currentCount;
}
bool Inventory::IsEmpty() const { return Items.size() == 0; }

size_t Inventory::MaxTransferAmount(const ItemInfo* item) const { return RemainingCapacity() / item->DalfitchDepth; }
size_t Inventory::MaxTransferAmount(const ItemStack& items) const { return std::min(MaxTransferAmount(items.Item), items.StackSize); }
#pragma endregion