
#include <string>

namespace Database
{
//connects to the Database. This should only be called a single time at the very beginning to initialize that connection.
void dbConnect();
//this creates a player. If the player already exists, it currently exits without creating them or throwing an error.
void createPlayer(const std::string& userID, int qpCount, std::string location);
//returns a user's qp count.
uint64_t getUserQpCount(const std::string& userID);
//returns a user's location
std::string getUserLocation(const std::string& userID);
//alters a user's qp count. The passed delta is how many qpies to add/subtract from their current balance.
//Positive values are added, negative ones subtracted.
//NOTE: This does not do bounds checking. If used incorrectly, this can make a user's qp count uder- or overflow.
void alterUserQpCount(const std::string& userID, int64_t delta);
//sets a user's location to the specified string.
void setUserLocation(const std::string& userID, const std::string& newLocation);
//Alters how many of a specific item a user has. If the item does not exist yet, an entry will be created for it with the specified amount.
//If the item does exist the amount indicates how many of the item to add or remove from a player's inventory.
//Should this leave the item at an amount of 0 OR LESS, its entry will be removed from the DB.
void alterInventoryItemCount(const std::string& userID, const std::string& itemLabel, int amount);
//retrieves the amount a player has of an item in their inventory.
uint64_t getInventoryItemCount(const std::string& userID, const std::string& itemLabel);
}