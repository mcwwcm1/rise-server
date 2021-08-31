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

void createPlayer(const std::string& userID, int qpCount, std::string location)
{
	pqxx::work transaction{*dbConn};
	
	try {
		//check if the user already exists in the db
		pqxx::result response = transaction.exec("SELECT * FROM player WHERE userid = " + transaction.quote(userID) + ";");
		if (!response.empty()) {
			return; //user already exists so we just don't create another entry.
		}
		
		//insert user into the DB
		transaction.exec("INSERT INTO player(userid, qp, location) VALUES(" + transaction.quote(userID) + ", " + std::to_string(qpCount) + ", " + transaction.quote(location) + ");");
		std::cout << "Created user '" + transaction.quote(userID) + "'";
		
		transaction.commit();
	} catch (...) {
		std::cout << "Error while inserting player into the DB User ID: '" << userID << "' Aborting transaction." << std::endl;
		transaction.abort();
	}
}

uint64_t getUserQpCount(const std::string& userID)
{
	pqxx::work transaction{*dbConn};
	pqxx::result response = transaction.exec("SELECT * FROM player WHERE userid = " + transaction.quote(userID) + ";");
	
	return response.begin()["qp"].as<uint64_t>();
}

void alterUserQpCount(const std::string& userID, int64_t delta)
{
	pqxx::work transaction{*dbConn};
	try {
		transaction.exec("UPDATE player SET qp = qp + " + std::to_string(delta) + " WHERE userid = " + transaction.quote(userID) + ";");
		transaction.commit();
	} catch (...) {
		std::cout << "Error while altering user QP count. User ID: '" << userID << "' Aborting transaction." << std::endl;
		transaction.abort();
	}
}

std::string getUserLocation(const std::string& userID)
{
	pqxx::work transaction{*dbConn};
	pqxx::result response = transaction.exec("SELECT * FROM player WHERE userid = " + transaction.quote(userID) + ";");
	
	return response.empty()? "" : response.begin()["location"].c_str();
}

void setUserLocation(const std::string& userID, const std::string& newLocation)
{
	pqxx::work transaction{*dbConn};
	try {
		transaction.exec("UPDATE player SET location = " + transaction.quote(newLocation) + " WHERE userid = " + transaction.quote(userID) + ";");
		transaction.commit();
	} catch (...) {
		std::cout << "Error while setting user location. User ID: '" << userID << "' Location: '" << newLocation << "' Aborting transaction." << std::endl;
		transaction.abort();
	}
}

void alterInventoryItemCount(const std::string& userID, const std::string& itemLabel, int delta)
{
	pqxx::work transaction{*dbConn};
	
	try {
		pqxx::result foundItems = transaction.exec("SELECT * FROM inventory inv JOIN player p ON(inv.userid, p.id) JOIN item i ON(inv.itemid, i.id) WHERE p.userid = " + transaction.quote(userID) + " AND itemid = " + transaction.quote(itemLabel) + ";");
		
		if (foundItems.empty()) {
			//add the item
			transaction.exec("INSERT INTO inventory(userid, itemid, amount) VALUES((SELECT id FROM player WHERE userid = " + transaction.quote(userID) + "), (SELECT id FROM item WHERE label = " + transaction.quote(itemLabel) + "), " + std::to_string(delta) + ");");
		} else {
			//item already there, update it's count
			uint64_t newAmount = foundItems.begin()["amount"].as<uint64_t>() + delta;
			
			if (newAmount > 0) {
				transaction.exec("UPDATE inventory SET amount = " + std::to_string(newAmount) + " FROM player p, item i WHERE p.userid = " + transaction.quote(userID) + " AND i.label = " + transaction.quote(itemLabel) + ";");
			} else {
				transaction.exec("DELETE FROM inventory inv USING player p, item i WHERE inv.userid = p.id AND inv.itemid = i.id AND p.userid = " + transaction.quote(userID) + " AND i.label = " + transaction.quote(itemLabel) + ";");
			}
		}
		
		transaction.commit();
	} catch (...) {
		std::cout << "Error while altering inventory item count. User ID: '" << userID << "', Item ID: '" << itemLabel << "' Aborting transaction." << std::endl;
		transaction.abort();
	}
}

uint64_t getInventoryItemCount(const std::string& userID, const std::string& itemLabel)
{
	pqxx::work transaction{*dbConn};
	pqxx::result itemAmounts = transaction.exec("SELECT inv.amount FROM inventory inv JOIN player p ON(inv.userid = p.id) JOIN item i ON(inv.itemid = i.id) WHERE p.userid = " + transaction.quote(userID) + " AND i.label = " + transaction.quote(itemLabel) + ";");
	
	return itemAmounts.empty()? 0 : itemAmounts.begin()["amount"].as<uint64_t>();
}

} // namespace Database