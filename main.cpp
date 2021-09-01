#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <unordered_map>
#include <chrono>

namespace beast     = boost::beast;
namespace http      = beast::http;
namespace websocket = beast::websocket;
namespace net       = boost::asio;
using tcp           = boost::asio::ip::tcp;

// Include core headers
#include "core/database.h"
#include "core/commands.h"
#include "core/circularbuffer.h"
#include "core/session.h"
#include "core/listener.h"
#include "core/send.h"

// Include primary functions
#include "primary/echo.h"
#include "primary/physicsinstructions.h"
#include "primary/airshipcontrol.h"
#include "primary/echoto.h"
#include "primary/entityinstructions.h"
#include "primary/userdata.h"
#include "primary/leaderboards.h"

// Include dumb shit
#include "data/items.h"

void testDB()
{
	Database::DbConnect();
	std::cout << "Blobfish has " << std::to_string(Database::GetUserQpCount("U-Blobfish")) << " qpies" << std::endl;
	std::cout << "Blobfish's location is " << Database::GetUserLocation("U-Blobfish") << std::endl;

	std::cout << "Stealing Blobfish his qpies..." << std::endl;
	Database::AlterUserQpCount("U-Blobfish", 500000);  //actually gives qpies, cause we're nice. :>
	std::cout << "Blobfish now has " << std::to_string(Database::GetUserQpCount("U-Blobfish")) << " qpies" << std::endl;

	std::cout << "Sending Blobfish to the Shadow Realm..." << std::endl;
	Database::SetUserLocation("U-Blobfish", "shadowRealm");
	std::cout << "Blobfish's location is now " << Database::GetUserLocation("U-Blobfish") << std::endl;

	std::cout << "Giving Blobfish a BlobfishItem..." << std::endl;
	Database::AlterInventoryItemCount("U-Blobfish", "BlobfishItem", 1);
	std::cout << "Blobfish now has " << std::to_string(Database::GetInventoryItemCount("U-Blobfish", "BlobfishItem")) << " of BlobfishItem." << std::endl;

	std::cout << "Fetching Leaderboards..." << std::endl;
	std::vector<uint64_t> moneys;
	std::vector<std::string> people;
	Database::GetQpLeaderboard(0, 100, &people, &moneys);
	for (auto name : people) {
		std::cout << name << std::endl;
	}
	for (auto qp : moneys) {
		std::cout << std::to_string(qp) << std::endl;
	}
}

int main(int argc, char* argv[])
{
	//debug db stuff
	//testDB();

	Database::DbConnect();

	//-------------------------Intialize function parsing map, array and buffers---------------------------
	//Populate parseMap
	Commands::Register("echo", EchoParser);
	Commands::Register("echoto", EchoToParser);
	Commands::Register("setthrottle", SetThrottleParser);
	Commands::Register("setpitch", SetPitchParser);
	Commands::Register("setyaw", SetYawParser);
	Commands::Register("registerstaticcollider", RegisterStaticColliderParser);
	Commands::Register("addforce", AddForceParser);
	Commands::Register("adddistanceconstraint", AddDistanceConstraintParser);
	Commands::Register("removeconstraint", RemoveConstraintParser);
	Commands::Register("requestairship", RequestAirshipParser);
	Commands::Register("setuserposition", SetUserPositionParser);
	Commands::Register("sellitems", SellItemsParser);
	Commands::Register("catchbug", CatchBugParser);
	Commands::Register("userspawned", UserSpawnedParser);
	Commands::Register("equipitem", EquipItemParser);
	Commands::Register("dequipitem", DequipItemParser);
	Commands::Register("updateleaderboard", UpdateLeaderboardParser);
	//-----------------------End of function initialization step------------------------------------------

	// Check command line arguments
	if (argc != 4) {
		std::cerr << "Usage: <address> <port> <threads>\n"
							<< "Example:\n"
							<< "    executablename 0.0.0.0 8080 1\n";
		return EXIT_FAILURE;
	}
	auto const address = net::ip::make_address(argv[1]);
	auto const port    = static_cast<unsigned short>(std::atoi(argv[2]));
	auto const threads = std::max<int>(1, std::atoi(argv[3]));

	// The io_context is required for all I/O
	net::io_context ioc{threads};

	// Create and launch the listening port
	std::make_shared<Listener>(ioc, tcp::endpoint{address, port})->Run();

	// Run the I/O service on the requested number of threads
	std::vector<std::thread> v;
	v.reserve(threads - 1);
	for (auto i = threads - 1; i > 0; --i) v.emplace_back([&ioc] { ioc.run(); });

	AddDummyItemData();  // REMOVE THIS WHEN WE HAVE DB STUFF

	std::chrono::milliseconds timespan(1000 / 20);  //defines sleep timespan in ms
	while (true) {
		std::this_thread::sleep_for(timespan);

		std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

		// Iterate over all elements in function buffer until empty
		while (!Commands::functionBuffer.Empty()) {
			Commands::functionBuffer.Get()();
		}

		if (Session::GetHeadless() == nullptr)
			continue;

		World::Singleton->RunTick();
	}
	return EXIT_SUCCESS;
}
