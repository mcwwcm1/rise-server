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

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

//Declare a test type for function pointer
typedef void (*primary_function) ();
typedef void (*parsing_function) (std::string&);

//Failure Reporting
void fail(beast::error_code ec, char const* what)
{
	std::cerr << what << ": " << ec.message() << "\n";
}

// Include core headers
#include "core/circularbuffer.h"
#include "core/globals.h"
#include "core/session.h"
#include "core/listener.h"

// Include primary functions
#include "primary/echotest.h"

int main(int argc, char* argv[])
{
	//connection to the db. Just for testing, don't uncomment.
	//pqxx::connection dbConn("dbname=rise-server user=rise-server password=HfUK6+{Cb!LS=~52 host=funnyanimalfacts.com port=5432");

	//-------------------------Intialize function parsing map, array and buffers---------------------------
	//Populate parseMap
	parseMap["echo"] = EchoTestParser;

	//-----------------------End of function initialization step------------------------------------------
	// Check command line arguments
	if (argc != 4)
	{
		std::cerr <<
			"Usage: <address> <port> <threads>\n" <<
            "Example:\n" <<
            "    executablename 0.0.0.0 8080 1\n";
		return EXIT_FAILURE;
	}
	auto const address = net::ip::make_address(argv[1]);
	auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
	auto const threads = std::max<int>(1, std::atoi(argv[3]));

	// The io_context is required for all I/O
	net::io_context ioc{threads};

	// Create and launch the listening port
	std::make_shared<listener>(ioc, tcp::endpoint{address, port})->run();

	// Run the I/O service on the requested number of threads
	std::vector<std::thread> v;
	v.reserve(threads - 1);
	for(auto i = threads - 1; i > 0; --i)
		v.emplace_back(
		[&ioc]
		{
			ioc.run();
		});

	std::chrono::milliseconds timespan(1000); //defines sleep timespan in ms
	while(true)
	{
		bufferAccessMutex.lock();
		//Iterate over all elements in function buffer until empty
		while(!functionBuffer.empty())
		{
			functionBuffer.get()();
		}
		bufferAccessMutex.unlock();
		printf("Loop iteration completed\n");
		std::this_thread::sleep_for(timespan);
	}
	return EXIT_SUCCESS;
}
