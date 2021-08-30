#include "database.h"

#include <pqxx/pqxx>
#include <iostream>

pqxx::connection* dbConn;

namespace Database
{
void dbConnect()
{
	dbConn = new pqxx::connection("dbname=riseserver user=postgres password=ePU&#B%72j2nRhA$RpK!Hfu++8XYbGQv host=funnyanimalfacts.com port=5432");
	std::cout << "DB Connection Successful" << std::endl;
}

void createPlayer(std::string userID)
{
	pqxx::work transaction{*dbConn};
	
	//check if the user already exists in the db
	pqxx::result response = transaction.exec("SELECT * FROM player WHERE userid = '" + transaction.quote(userID) + "';");
	if (!response.empty()) {
		return; //TODO: throw error
	}
	
	//insert user into the DB
	transaction.exec("INSERT INTO player(userid, qp, location) VALUES('" + transaction.quote(userID) + "', 100, 'starterIsland');");
	std::cout << "Created user '" + transaction.quote(userID) + "'";
	
	transaction.commit();
}

uint64_t getUserQpCount(std::string userID)
{
	pqxx::work transaction{*dbConn};
	pqxx::result response = transaction.exec("SELECT * FROM player WHERE userid = '" + transaction.quote(userID) + "';");
	
	return response.begin()["qp"].as(uint64_t);
}

void alterUserQpCount(std::string userID, int64_t delta)
{
	pqxx::work transaction{*dbConn};
	pqxx::result response = transaction.exec("SELECT * FROM player WHERE userid = '" + transaction.quote(userID) + "';");
	
	uint64_t currentQP = response.begin()["qp"].as(uint64);
	currentQP += delta;
	
	transaction.exec("UPDATE player SET qp = " + std::to_string(currentQP) + " WHERE userid = '" + transaction.quote(userID) + "';");
	transaction.commit();
}

std::string getUserLocation(std::string userID)
{
	pqxx::work transaction{*dbConn};
	pqxx::result response = transaction.exec("SELECT * FROM player WHERE userid = '" + transaction.quote(userID) + "';");
	
	return response.begin()["location"].c_str();
}

void setUserLocation(std::string userID, std::string newLocation)
{
	pqxx::work transaction{*dbConn};
	transaction.exec("UPDATE player SET location = " + transaction.quote(newLocation) + " WHERE userid = '" + transaction.quote(userID) + "';");
}

} // namespace Database