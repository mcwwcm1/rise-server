// File: items.h
// Purpose: Stores relevant item information that is pulled from the DB

#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <functional>

struct ItemProperty {
 public:
	std::string Name;
	double Value;  // I want this to be any arbitrary type ideally - Coffee :)
	ItemProperty(std::string name, double value) : Name(name), Value(value){};
};

struct ItemInfo {
 public:
	static std::unordered_map<std::string, ItemInfo*> RegisteredItems;

	const std::string ItemID;
	const std::vector<std::string> Tags;
	const bool Stackable;
	const size_t QupyValue;
	const size_t DalfitchDepth;
	std::vector<ItemProperty*> Properties;  // Should eventually be const

	static ItemInfo* GetItemByID(const std::string& id);
	static void RegisterItem(ItemInfo* item);

	ItemInfo(std::string itemID, std::vector<std::string> tags, bool stackable, size_t qupyValue, size_t dalfitchDepth) : ItemID(itemID), Tags(tags), Stackable(stackable), QupyValue(qupyValue), DalfitchDepth(dalfitchDepth){};
};

void AddDummyItemData();

struct ItemStack {
 public:
	ItemInfo* Item;
	size_t StackSize;

	ItemStack() = default;
	ItemStack(std::string itemID, size_t stackSize = 1) : Item(ItemInfo::GetItemByID(itemID)), StackSize(stackSize){};
	ItemStack(ItemInfo* item, size_t stackSize = 1) : Item(item), StackSize(stackSize){};

	size_t HeldCapacity() const;
	bool IsEmpty() const;
};

struct Inventory {
 public:
	size_t capacity;
	std::unordered_map<std::string, ItemStack> Items;

#pragma region Item handling
	// Returns whether this inventory contains the given items
	bool HasItems(const std::string& itemID, size_t amount) const;
	// Returns whether this inventory contains the given items
	bool HasItems(ItemInfo* item, size_t amount) const;
	// Returns whether this inventory contains the given item
	bool HasItem(const std::string& itemID) const;
	// Returns whether this inventory contains the given item
	bool HasItem(ItemInfo* item) const;

	// Returns whether there is enough capacity within this inventory to hold the given inventory
	bool CanHoldInventory(const Inventory& inventory) const;
	// Returns whether there is enough capacity within this inventory to hold the given item stack
	bool CanHoldItems(const ItemStack& items) const;

	// This will take from the given inventory, transferring to this inventory until it reaches capacity. Returns whether it successfully transferred all items.
	bool TransferItems(Inventory& inventory);
	// This will take from the given stack, transferring to this inventory until it reaches capacity. Returns whether it successfully transferred all items.
	bool TransferItems(ItemStack& items);

	// Add the given inventory to this inventory. This will disregard items that exceed the inventories capacity
	void AddItems(const Inventory& inventory);
	// Add the given items to this inventory. This will disregard items that exceed the inventories capacity
	void AddItems(const std::string& itemID, size_t amount);
	// Add the given items to this inventory. This will disregard items that exceed the inventories capacity
	void AddItems(ItemInfo* item, size_t amount);
	// Add the given items to this inventory. This will disregard items that exceed the inventories capacity
	void AddItems(const ItemStack& items);
	// Add the given item to this inventory. This will disregard items that exceed the inventories capacity
	void AddItem(const std::string& itemID);
	// Add the given item to this inventory. This will disregard items that exceed the inventories capacity
	void AddItem(ItemInfo* item);

	// Attempts to take items from this inventory. This may fail, returning an empty stack.
	ItemStack TakeItems(ItemInfo* item, size_t amount);
	// Attempts to take items from this inventory. This may fail, returning an empty stack.
	ItemStack TakeItems(const std::string& itemID, size_t amount);
	// Attempts to take an item from this inventory. This may fail, returning an empty stack.
	ItemStack TakeItem(const std::string& itemID);
	// Attempts to take an item from this inventory. This may fail, returning an empty stack.
	ItemStack TakeItem(ItemInfo* item);
#pragma endregion

	std::function<void(ItemStack)> ItemChangedCallback;
	std::function<void(ItemStack)> ItemAddedCallback;
	std::function<void(ItemStack)> ItemRemovedCallback;

	size_t RemainingCapacity() const;
	size_t HeldCapacity() const;
	bool IsEmpty() const;

	size_t MaxTransferAmount(const ItemInfo* item) const;
	size_t MaxTransferAmount(const ItemStack& items) const;
};