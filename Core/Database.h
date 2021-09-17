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
//returns the total value of qpies of all current users and crews combined
uint64_t GetQpAbsoluteTotal();
//returns a user's active crew. If the user is not in any crews (or has no active crew for some reason), empty string is returned.
std::string GetUserActiveCrew(const std::string& userID, const std::string& crewID);
//sets a user's active crew while also setting all other crews they are in as inactive.
void SetUserActiveCrew(const std::string& userID, const std::string& crewID);
//adds a user to a crew.
//This DOES NOT make the new crew the user's active crew.
void AddUserToCrew(const std::string& userID, const std::string& crewID);
//Simply removes a user from a crew.
void RemoveUserFromCrew(const std::string& userID, const std::string& crewID);
//returns an std::vector of strings. These strings are the crew IDs that the passed user is in.
std::vector<std::string> GetUserCrews(const std::string& userID);

//crew versions of user functions above

//this creates a crew. If the crew already exists, it currently exits without creating it or throwing an error.
//directly aliased to CreatePlayer().
void CreateCrew(const std::string& userID, int qpCount, std::string location);
//returns a crew's qp count.
//directly aliased to GetUserQpCount().
uint64_t GetCrewQpCount(const std::string& userID);
//alters a crew's qp count. The passed delta is how many qpies to add/subtract from its current balance.
//Positive values are added, negative ones subtracted.
//NOTE: This does not do bounds checking. If used incorrectly, this can make a crews's qp count uder- or overflow.
//directly aliased to AlterUserQpCount().
void AlterCrewQpCount(const std::string& userID, int64_t delta);
//Alters how many of a specific item a crew has. If the item does not exist yet, an entry will be created for it with the specified amount.
//If the item does exist the amount indicates how many of the item to add or remove from a player's inventory.
//Should this leave the item at an amount of 0 OR LESS, its entry will be removed from the DB.
//directly aliased to AlterInventoryItemCount().
void AlterCrewInventoryItemCount(const std::string& userID, const std::string& itemLabel, int amount);
//retrieves the amount a crew has of an item in their inventory.
//directly aliased to GetInventoryItemCount().
uint64_t GetCrewInventoryItemCount(const std::string& userID, const std::string& itemLabel);
//deletes a crew and all associated data from the database.
//this is irreversible.
//directly aliased to DeletePlayer().
void DeleteCrew(const std::string& userID);
}