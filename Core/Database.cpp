#include "Database.h"

#include <pqxx/pqxx>
#include <iostream>

pqxx::connection* dbConn;

namespace Database
{
void DbConnect()
{
	dbConn = new pqxx::connection("dbname=riseserver user=postgres password=ePU&#B%72j2nRhA$RpK!Hfu++8XYbGQv host=funnyanimalfacts.com port=5432");
	std::cout << "DB Connection Successful" << std::endl;
}

void CreatePlayer(const std::string& userID, int qpCount, std::string location)
{
	pqxx::work transaction{*dbConn};

	try {
		//check if the user already exists in the db
		pqxx::result response = transaction.exec("SELECT * FROM player WHERE userid = " + transaction.quote(userID) + ";");
		if (!response.empty()) {
			return;  //user already exists so we just don't create another entry.
		}

		//insert user into the DB
		transaction.exec("INSERT INTO player(userid, qp, location) VALUES(" + transaction.quote(userID) + ", " + std::to_string(qpCount) + ", " + transaction.quote(location) + ");");
		std::cout << "Created user '" + transaction.quote(userID) + "'";

		transaction.commit();
	} catch (const std::exception& exception) {
		std::cout << "Error while inserting player into the DB User ID: '" << userID << "' Aborting transaction." << std::endl
							<< "Exception: " << std::endl
							<< exception.what() << std::endl;
		transaction.abort();
	}
}

uint64_t GetUserQpCount(const std::string& userID)
{
	pqxx::work transaction{*dbConn};
	pqxx::result response = transaction.exec("SELECT * FROM player WHERE userid = " + transaction.quote(userID) + ";");

	return response.begin()["qp"].as<uint64_t>();
}

void AlterUserQpCount(const std::string& userID, int64_t delta)
{
	pqxx::work transaction{*dbConn};
	try {
		transaction.exec("UPDATE player SET qp = qp + " + std::to_string(delta) + " WHERE userid = " + transaction.quote(userID) + ";");
		transaction.commit();
	} catch (const std::exception& exception) {
		std::cout << "Error while altering user QP count. User ID: '" << userID << "' Aborting transaction." << std::endl
							<< "Exception: " << std::endl
							<< exception.what() << std::endl;
		transaction.abort();
	}
}

std::string GetUserLocation(const std::string& userID)
{
	pqxx::work transaction{*dbConn};
	pqxx::result response = transaction.exec("SELECT * FROM player WHERE userid = " + transaction.quote(userID) + ";");

	return response.empty() ? "" : response.begin()["location"].c_str();
}

void SetUserLocation(const std::string& userID, const std::string& newLocation)
{
	pqxx::work transaction{*dbConn};
	try {
		transaction.exec("UPDATE player SET location = " + transaction.quote(newLocation) + " WHERE userid = " + transaction.quote(userID) + ";");
		transaction.commit();
	} catch (const std::exception& exception) {
		std::cout << "Error while setting user location. User ID: '" << userID << "' Location: '" << newLocation << "' Aborting transaction." << std::endl
							<< "Exception: " << std::endl
							<< exception.what() << std::endl;
		transaction.abort();
	}
}

void AlterInventoryItemCount(const std::string& userID, const std::string& itemLabel, int delta)
{
	pqxx::work transaction{*dbConn};

	try {
		pqxx::result foundItems = transaction.exec("SELECT * FROM inventory inv JOIN player p ON(inv.userid = p.id) JOIN item i ON(inv.itemid = i.id) WHERE p.userid = " + transaction.quote(userID) + " AND i.label = " + transaction.quote(itemLabel) + ";");

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
	} catch (const std::exception& exception) {
		std::cout << "Error while altering inventory item count. User ID: '" << userID << "', Item ID: '" << itemLabel << "' Aborting transaction." << std::endl
							<< "Exception: " << std::endl
							<< exception.what() << std::endl;
		transaction.abort();
	}
}

uint64_t GetInventoryItemCount(const std::string& userID, const std::string& itemLabel)
{
	pqxx::work transaction{*dbConn};
	pqxx::result itemAmounts = transaction.exec("SELECT inv.amount FROM inventory inv JOIN player p ON(inv.userid = p.id) JOIN item i ON(inv.itemid = i.id) WHERE p.userid = " + transaction.quote(userID) + " AND i.label = " + transaction.quote(itemLabel) + ";");

	return itemAmounts.empty() ? 0 : itemAmounts.begin()["amount"].as<uint64_t>();
}

void DeletePlayer(const std::string& userID)
{
	pqxx::work transaction{*dbConn};
	try {
		//TODO: Properly clean up tables that need it once they exist, as default behavior is to delete every column that the deleted entry gets referenced in.
		transaction.exec("DELETE FROM player WHERE id = " + transaction.quote(userID) + ";");

		transaction.commit();
	} catch (const std::exception& exception) {
		std::cout << "Error while deleting player. User ID: '" << userID << "' Aborting transaction." << std::endl
							<< "Exception: " << std::endl
							<< exception.what() << std::endl;
		transaction.abort();
	}
}

void GetQpLeaderboard(uint64_t from, uint64_t to, std::vector<std::string>* userIDs, std::vector<uint64_t>* qpCounts)
{
	pqxx::work transaction{*dbConn};
	pqxx::result leaderboardResults = transaction.exec("SELECT userid, qp FROM player WHERE userid LIKE 'U-%' ORDER BY qp DESC OFFSET " + std::to_string(from) + " LIMIT " + std::to_string(to - from) + ";");
	//fill userIDs
	if (userIDs) {
		for (auto row : leaderboardResults) {
			userIDs->push_back(row["userid"].c_str());
		}
	}
	//fill qpCounts
	if (qpCounts) {
		for (auto row : leaderboardResults) {
			qpCounts->push_back(row["qp"].as<uint64_t>());
		}
	}
}

uint64_t GetQpAbsoluteTotal()
{
	pqxx::work transaction{*dbConn};
	pqxx::result totalsResult = transaction.exec("SELECT SUM(qp) FROM player;");

	return totalsResult.begin()["sum"].as<uint64_t>();
}

std::string GetUserActiveCrew(const std::string& userID )
{
	pqxx::work transaction{*dbConn};
	pqxx::result response = transaction.exec("SELECT * FROM crewmember c JOIN player pu ON(c.playerid = pu.id) JOIN player pc ON(c.crewid = pc.id) WHERE pu.userid = " + transaction.quote(userID) + " AND c.active = true;");

	return response.empty() ? "" : response.begin()["pc.userid"].c_str();
}

void SetUserActiveCrew(const std::string& userID, const std::string& crewID)
{
	pqxx::work transaction{*dbConn};
	try {
		transaction.exec("UPDATE crewmember c SET c.active = false FROM player p WHERE p.id = c.playerid AND p.userid = " + transaction.quote(userID) + ";");
		transaction.exec("UPDATE crewmember c SET c.active = true FROM player p WHERE p.id = c.playerid AND p.userid = " + transaction.quote(userID) + ";");
		transaction.commit();
	} catch (const std::exception& exception) {
		std::cout << "Error while setting active crew for player. User ID: '" << userID << "', Crew ID: '" + crewID + "' Aborting transaction." << std::endl
							<< "Exception: " << std::endl
							<< exception.what() << std::endl;
		transaction.abort();
	}
}

void AddUserToCrew(const std::string& userID, const std::string& crewID)
{
	pqxx::work transaction{*dbConn};
	try {
		transaction.exec("INSERT INTO crewmember(playerid, crewid, active) VALUES((SELECT id FROM player WHERE userid = " + transaction.quote(userID) + "), (SELECT id FROM player WHERE userid = " + transaction.quote(crewID) + "), false);");
		transaction.commit();
	} catch (const std::exception& exception) {
		std::cout << "Error while adding player to crew. User ID: '" << userID << "', Crew ID: '" + crewID + "' Aborting transaction." << std::endl
							<< "Exception: " << std::endl
							<< exception.what() << std::endl;
		transaction.abort();
	}
}

void RemoveUserFromCrew(const std::string& userID, const std::string& crewID)
{
	pqxx::work transaction{*dbConn};
	try {
		transaction.exec("DELETE FROM crewmember c USING player pu, player pc WHERE c.playerid = pu.id AND c.crewID = pc.id AND pu.userid = " + transaction.quote(userID) + " AND pc.crewID = " + transaction.quote(crewID) + ";");
		transaction.commit();
	} catch (const std::exception& exception) {
		std::cout << "Error while removing player from crew. User ID: '" << userID << "', Crew ID: '" + crewID + "' Aborting transaction." << std::endl
							<< "Exception: " << std::endl
							<< exception.what() << std::endl;
		transaction.abort();
	}
}

}  // namespace Database