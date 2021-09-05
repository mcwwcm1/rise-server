// File: userdata.h
// Purpose: For getting data about a users actions in game

#include <string>
#include "Utilities.h"
#include "Core/Commands.h"
#include "World/World.h"

UserEntity* EnsureUserRegistered(std::string userID)
{
	UserEntity* user;
	auto pair = World::Singleton->Users.find(userID);
	if (pair == World::Singleton->Users.end()) {
		user = new UserEntity(userID);
		World::Singleton->RegisterEntity(user);
		Send("RegisterUser " + userID + "|" + user->ID + "|");
	} else
		user = pair->second;

	return user;
}

// setuserposition <userID> <position>
void SetUserPosition()
{
	std::string userID = Commands::GetArgument<std::string>();
	Double3 position   = Commands::GetArgument<Double3>();

	UserEntity* user = EnsureUserRegistered(userID);

	user->Position = position;
}

void SetUserPositionParser(const std::string& arguments)
{
	auto parts = Split(arguments, '|');

	Commands::ValidateArgumentCount(parts, 2);

	Double3 position = Double3FromString(parts[1]);

	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(SetUserPosition);

	// Put arguments
	Commands::argumentBuffer.Put(parts[0]);  // UserID
	Commands::argumentBuffer.Put(position);
}

// sellitems <userID> <itemID> <amount>
void SellItems()
{
	std::string userID = Commands::GetArgument<std::string>();
	std::string itemID = Commands::GetArgument<std::string>();
	size_t amount      = Commands::GetArgument<size_t>();

	auto userPair = World::Singleton->Users.find(userID);

	if (userPair == World::Singleton->Users.end())
		return;

	UserEntity* user = userPair->second;

	if (user->UserInventory->HasItems(itemID, amount)) {
		ItemStack items = user->UserInventory->TakeItems(itemID, amount);
		user->AddQupies(items.Item->QupyValue * items.StackSize);
	}
}

void SellItemsParser(const std::string& arguments)
{
	auto parts = Split(arguments, '|');

	Commands::ValidateArgumentCount(parts, 3);

	size_t amount = stoull(parts[2]);

	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(SellItems);

	// Put arguments
	Commands::argumentBuffer.Put(parts[0]);  // UserID
	Commands::argumentBuffer.Put(parts[1]);  // ItemID
	Commands::argumentBuffer.Put(amount);
}

// userspawned <userID>
void UserSpawned()
{
	std::string userID = Commands::GetArgument<std::string>();
	UserEntity* user   = EnsureUserRegistered(userID);

	Send("RegisterUser " + userID + "|" + user->ID + "|");
}

void UserSpawnedParser(const std::string& arguments)
{
	// Put function pointer
	Commands::functionBuffer.Put(UserSpawned);

	// Put arguments
	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);
	Commands::argumentBuffer.Put(arguments);
}

// equipitem <userID> <itemID> <slotIndex>
void EquipItem()
{
	std::string userID = Commands::GetArgument<std::string>();
	UserEntity* user   = EnsureUserRegistered(userID);

	std::string itemID = Commands::GetArgument<std::string>();
	size_t slotIndex   = Commands::GetArgument<size_t>();

	if (!user->EquipItem(itemID, slotIndex)) {
		Send("DequipItem " + userID + "|" + std::to_string(slotIndex) + "|");
		printf("%s failed to equip %s\n", userID.c_str(), itemID.c_str());
	} else
		printf("%s successfully equipped %s\n", userID.c_str(), itemID.c_str());
}

void EquipItemParser(const std::string& arguments)
{
	auto parts = Split(arguments, '|');

	Commands::ValidateArgumentCount(parts, 3);

	size_t slotIndex = stoull(parts[2]);

	// Put function pointer
	Commands::functionBuffer.Put(EquipItem);

	// Put arguments
	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);
	Commands::argumentBuffer.Put(parts[0]);  // UserID
	Commands::argumentBuffer.Put(parts[1]);  // ItemID
	Commands::argumentBuffer.Put(slotIndex);
}

// dequipitem <userID> <slotIndex>
void DequipItem()
{
	std::string userID = Commands::GetArgument<std::string>();
	UserEntity* user   = EnsureUserRegistered(userID);

	size_t slotIndex = Commands::GetArgument<size_t>();

	user->DequipItem(slotIndex);
}

void DequipItemParser(const std::string& arguments)
{
	auto parts = Split(arguments, '|');

	Commands::ValidateArgumentCount(parts, 2);

	size_t slotIndex = stoull(parts[1]);

	// Put function pointer
	Commands::functionBuffer.Put(DequipItem);

	// Put arguments
	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);
	Commands::argumentBuffer.Put(parts[0]);  // UserID
	Commands::argumentBuffer.Put(slotIndex);
}

// requestinventory <userID>
void RequestInventory()
{
	std::string userID = Commands::GetArgument<std::string>();
	UserEntity* user   = EnsureUserRegistered(userID);

	user->ResubmitFullInventory();
}

void RequestInventoryParser(const std::string& arguments)
{
	// Put function pointer
	Commands::functionBuffer.Put(RequestInventory);

	// Put arguments
	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);
	Commands::argumentBuffer.Put(arguments);  // UserID
}