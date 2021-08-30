
#include <string>

namespace Database
{
void dbConnect();
void createPlayer(std::string userID);
std::string getUserQpCount(std::string userID);
std::string getUserLocation(std::string userID);
void alterUserQpCount(std::string userID, int64_t delta);
void setUserLocation(std::string userID, std::string newLocation);
}