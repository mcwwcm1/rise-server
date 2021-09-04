#include <string>
#include <vector>

namespace Database
{
//connects to the Database. This should only be called a single time at the very beginning to initialize that connection.
void DbConnect();
//this creates a player. If the player already exists, it currently exits without creating them or throwing an error.
void CreatePlayer(const std::string& userID, int qpCount, std::string location);
//returns a user's qp count.
uint64_t GetUserQpCount(const std::string& userID);
//returns a user's location
std::string GetUserLocation(const std::string& userID);
//alters a user's qp count. The passed delta is how many qpies to add/subtract from their current balance.
//Positive values are added, negative ones subtracted.
//NOTE: This does not do bounds checking. If used incorrectly, this can make a user's qp count uder- or overflow.
void AlterUserQpCount(const std::string& userID, int64_t delta);
//sets a user's location to the specified string.
void SetUserLocation(const std::string& userID, const std::string& newLocation);
//Alters how many of a specific item a user has. If the item does not exist yet, an entry will be created for it with the specified amount.
//If the item does exist the amount indicates how many of the item to add or remove from a player's inventory.
//Should this leave the item at an amount of 0 OR LESS, its entry will be removed from the DB.
void AlterInventoryItemCount(const std::string& userID, const std::string& itemLabel, int amount);
//retrieves the amount a player has of an item in their inventory.
uint64_t GetInventoryItemCount(const std::string& userID, const std::string& itemLabel);
//deletes a player and all associated data from the database.
//this is irreversible.
void DeletePlayer(const std::string& userID);
//Appends user IDs and their qp counts, sorted by qp count in descending order to the two passed vectors.
//The 'from' parameter gives the first spot to be included (0-inexed) and the 'to' parameter gives the spot to stop at. (it will not be included)
//This means passing from = 10 and to = 12 will return places 10 and 11.
void GetQpLeaderboard(uint64_t from, uint64_t to, std::vector<std::string>* userIDs, std::vector<uint64_t>* qpCounts);
//returns the total value of qpies of all current users combined
uint64_t GetQpAbsoluteTotal();
}