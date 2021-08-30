
#include <string>

namespace Database
{
//connects to the Database. This should only be called a single time at the very beginning to initialize that connection.
void dbConnect();
//this creates a player. If the player already exists, it currently exits without creating them or throwing an error.
void createPlayer(std::string userID);
//returns a user's qp count.
uint64_t getUserQpCount(std::string userID);
//returns a user's location
std::string getUserLocation(std::string userID);
//alters a user's qp count. The passed delta is how many qpies to add/subtract from their current balance.
//Positive values are added, negative ones subtracted.
//NOTE: This does not do bounds checking. If used incorrectly, this can make a user's qp count uder- or overflow.
void alterUserQpCount(std::string userID, int64_t delta);
//sets a user's location to the specified string.
void setUserLocation(std::string userID, std::string newLocation);
}