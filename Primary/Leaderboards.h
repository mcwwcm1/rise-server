// File: entityinstructions.h
// Purpose: For registering entities and assigning various properties about them

#include "Core/Commands.h"
#include "Core/Database.h"
#include "Core/Send.h"
#include "Utilities.h"

// updateleaderboard <entityID> <fromPlacement> <toPlacement>
void UpdateLeaderboard()
{
	std::string entityID = Commands::GetArgument<std::string>();
	int fromPlacement    = Commands::GetArgument<int>();
	int toPlacement      = Commands::GetArgument<int>();

	std::vector<std::string> userIDs;
	std::vector<uint64_t> qupies;
	Database::GetQpLeaderboard((unsigned long) fromPlacement, (unsigned long) toPlacement, &userIDs, &qupies);

	// Format the leaderboard
	std::string response = "LBUpdate " + entityID + "|" + std::to_string(userIDs.size()) + "|";
	for (int i = fromPlacement; i < toPlacement; i++) {
		if (i - fromPlacement >= (int) userIDs.size())
			break;
		response += std::to_string((i - fromPlacement) + 1) + "|" + userIDs[i - fromPlacement] + "|" + std::to_string(qupies[i - fromPlacement]) + "|";
	}

	Send(response);
}

void UpdateLeaderboardParser(const std::string& arguments)
{
	auto parts = Split(arguments, '|');
	Commands::ValidateArgumentCount(parts, 3);

	int fromPlacement = std::stoi(parts[1]);
	int toPlacement   = std::stoi(parts[2]);

	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Put function pointer
	Commands::functionBuffer.Put(UpdateLeaderboard);

	Commands::argumentBuffer.Put(parts[0]);  // EntityID
	Commands::argumentBuffer.Put(fromPlacement);
	Commands::argumentBuffer.Put(toPlacement);
}