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
#include <pqxx/pqxx>

namespace beast     = boost::beast;
namespace http      = beast::http;
namespace websocket = beast::websocket;
namespace net       = boost::asio;
using tcp           = boost::asio::ip::tcp;

// Include core headers
#include "core/commands.h"
#include "core/circularbuffer.h"
#include "core/session.h"
#include "core/listener.h"
#include "core/send.h"
#include "core/worldtick.h"

// Include primary functions
#include "primary/echo.h"
#include "primary/physicstick.h"
#include "primary/echoto.h"

int main(int argc, char* argv[])
{
	//connection to the db. Just for testing, don't uncomment.
	//pqxx::connection dbConn("dbname=riseserver user=riseserver password=HfUK6+{Cb!LS=~52 host=funnyanimalfacts.com port=5432");

	//-------------------------Intialize function parsing map, array and buffers---------------------------
	//Populate parseMap
	Commands::Register("echoto", EchoToParser);
	Commands::Register("setthrottle", SetThrottleParser);
	Commands::Register("setpitch", SetPitchParser);
	Commands::Register("setyaw", SetYawParser);
	Commands::Register("registerstaticcollider", RegisterStaticColliderParser);
	Commands::Register("addforce", AddForceParser);
	Commands::Register("registerentity", RegisterEntityParser);
	Commands::Register("unregisterentity", UnregisterEntityParser);
	Commands::Register("setowner", SetOwnerParser);
	Commands::Register("adddistanceconstraint", AddDistanceConstraintParser);
	Commands::Register("requestairship", RequestAirshipParser);
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

	std::chrono::milliseconds timespan(1000 / 20);  //defines sleep timespan in ms
	while (true) {
		{
			std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

			//Iterate over all elements in function buffer until empty
			while (!Commands::functionBuffer.Empty()) {
				Commands::functionBuffer.Get()();
			}

			WorldTick();
		}

		std::this_thread::sleep_for(timespan);
	}
	return EXIT_SUCCESS;
}