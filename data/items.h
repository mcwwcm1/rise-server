// File: items.h
// Purpose: Stores relevant item information that is pulled from the DB

#include <unordered_map>
#include <string>
#include <vector>
#include <iterator>

struct ItemInfo {
 public:
	std::string ItemID;
	std::vector<std::string> Tags;
	bool Stackable;
	size_t QupyValue;
	size_t DalfitchDepth;
};

std::unordered_map<std::string, ItemInfo*> RegisteredItems;

ItemInfo* GetItemByID(std::string id)
{
	auto i = RegisteredItems.find(id);
	if (i != RegisteredItems.end())
		return i->second;

	return nullptr;
};

struct ItemStack {
 public:
	ItemInfo* Item;
	size_t StackSize;

	ItemStack(std::string itemID, size_t stackSize = 1) : Item(GetItemByID(itemID)), StackSize(stackSize){};
	ItemStack(ItemInfo* item, size_t stackSize = 1) : Item(item), StackSize(stackSize){};

	size_t HeldCapacity() const;
	bool IsEmpty() const;
};

struct Inventory {
 public:
	size_t capacity;
	std::unordered_map<std::string, ItemStack*> Items;

#pragma region Item handling
	// Returns weather this inventory contains the given items
	bool HasItems(std::string itemID, size_t amount) const;
	// Returns weather this inventory contains the given items
	bool HasItems(ItemInfo* item, size_t amount) const;
	// Returns weather this inventory contains the given item
	bool HasItem(std::string itemID) const;
	// Returns weather this inventory contains the given item
	bool HasItem(ItemInfo* item) const;

	ItemStack* GetItemStack(ItemInfo* item) const;
	ItemStack* GetItemStack(std::string itemID) const;

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
	void AddItems(std::string itemID, size_t amount);
	// Add the given items to this inventory. This will disregard items that exceed the inventories capacity
	void AddItems(ItemInfo* item, size_t amount);
	// Add the given items to this inventory. This will disregard items that exceed the inventories capacity
	void AddItems(const ItemStack& items);
	// Add the given item to this inventory. This will disregard items that exceed the inventories capacity
	void AddItem(std::string itemID);
	// Add the given item to this inventory. This will disregard items that exceed the inventories capacity
	void AddItem(ItemInfo* item);

	// Attempts to take items from this inventory. This may fail, returning an empty stack.
	ItemStack TakeItems(ItemInfo* item, size_t amount);
	// Attempts to take items from this inventory. This may fail, returning an empty stack.
	ItemStack TakeItems(std::string itemID, size_t amount);
	// Attempts to take an item from this inventory. This may fail, returning an empty stack.
	ItemStack TakeItem(std::string itemID);
	// Attempts to take an item from this inventory. This may fail, returning an empty stack.
	ItemStack TakeItem(ItemInfo* item);
#pragma endregion

	size_t RemainingCapacity() const;
	size_t HeldCapacity() const;
	bool IsEmpty() const;

	size_t MaxTransferAmount(const ItemInfo* item) const;
	size_t MaxTransferAmount(const ItemStack& items) const;
};
