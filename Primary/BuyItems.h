// File: buyitems.h
// Purpose: For buying items from various vending machines / stores in the world

#include "Core/Commands.h"
#include "Core/Send.h"
#include "Utilities.h"

// buyitems <storeID> <userID> <itemID> <amount>
void BuyItems()
{
	std::string storeID = Commands::GetArgument<std::string>();
	std::string userID  = Commands::GetArgument<std::string>();
	std::string itemID  = Commands::GetArgument<std::string>();
	int amount          = Commands::GetArgument<int>();

	//get UserEntity and ItemInfo objects for the requested user and item.
	UserEntity* user = World::Singleton->Users[userID];
	ItemInfo* item   = ItemInfo::GetItemByID(itemID);

	if (item == nullptr) { printf("Attempted to buy non-existent item %s\n", itemID.c_str()); }

	//prepare response to fill in the if branches
	std::string response = "";

	// Check if the user can buy the item and has enough inventory space for it
	if (user->Qupies >= item->QupyValue * amount && user->UserInventory->CanHoldItems(itemID, amount)) {
		user->UserInventory->AddItems(item, amount);
		user->AddQupies(amount * -item->QupyValue);
		response = "BuySuccess " + storeID;
	} else {
		response = "BuyFail " + storeID;
	}

	Send(response);
}

void BuyItemsParser(const std::string& arguments)
{
	auto parts = Split(arguments, '|');
	Commands::ValidateArgumentCount(parts, 4);

	int amount = std::stoi(parts[3]);

	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(BuyItems);

	Commands::argumentBuffer.Put(parts[0]);  // StoreID
	Commands::argumentBuffer.Put(parts[1]);  // UserID
	Commands::argumentBuffer.Put(parts[2]);  // ItemID
	Commands::argumentBuffer.Put(amount);
}