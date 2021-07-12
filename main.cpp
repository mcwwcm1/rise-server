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


#include "circularbuffer/circularbuffer.h"
#include "primary/indexes.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

//Declare the map to be used for command parsing (populated in main)
std::unordered_map<std::string, parsing_function> parseMap;

//Failure Reporting
void fail(beast::error_code ec, char const* what)
{
	std::cerr << what << ": " << ec.message() << "\n";
}

// Echoes back all received WebSocket messages
class session : public std::enable_shared_from_this<session>
{
	websocket::stream<beast::tcp_stream> ws_;
	beast::flat_buffer buffer_;

public:
	// Take ownership of the socket
	explicit
	session(tcp::socket&& socket)
		: ws_(std::move(socket))
	{
	}

	// Get on the correct executor
	void run()
	{
		//For async functionality and thread safety
		net::dispatch(ws_.get_executor(),
			beast::bind_front_handler(
				&session::on_run,
				shared_from_this()));
	}

	// Start the asynchronous operation
	void on_run()
	{
		// Set the suggested tiemout settings for the websocket
		ws_.set_option(
			websocket::stream_base::timeout::suggested(
				beast::role_type::server));

		// Set a decorator to change the Server of the handshake
		ws_.set_option(websocket::stream_base::decorator(
			[](websocket::response_type& res)
			{
				res.set(http::field::server,
					std::string(BOOST_BEAST_VERSION_STRING) +
						" my dumb websocket server");
			}));

		// Accept the websocket handshake
		ws_.async_accept(
			beast::bind_front_handler(
				&session::on_accept,
				shared_from_this()));
	}

	void
    on_accept(beast::error_code ec)
    {
        if(ec)
            return fail(ec, "accept");

        // Read a message
        do_read();
    }

	void
	do_read()
	{
		// Read a message into our buffer
		ws_.async_read(
			buffer_,
			beast::bind_front_handler(
				&session::on_read,
				shared_from_this()));
	}

	void on_read(
		beast::error_code ec,
		std::size_t bytes_transferred)
	{
		boost::ignore_unused(bytes_transferred);

		// This indicates that the session was closed
		if(ec == websocket::error::closed)
			return;

		if(ec)
			fail(ec, "read");

		// Echo the message
		ws_.text(ws_.got_text());
		ws_.async_write(
			buffer_.data(),
			beast::bind_front_handler(
				&session::on_write,
				shared_from_this()));

		//Separate function call and arguments
		std::string message = boost::beast::buffers_to_string(buffer_.data());
		std::string function = message.substr(0, message.find(" "));
		std::string arguments = message.substr(message.find(" ") + 1, message.length());

		//Call function
		parseMap[function](arguments);
	}

	void on_write(
		beast::error_code ec,
		std::size_t bytes_transferred)
	{
		boost::ignore_unused(bytes_transferred);

		if(ec)
			return fail(ec, "write");

		// Clear the buffer
		buffer_.consume(buffer_.size());

		// Do another read
		do_read();
	}
};

//-----------------End of session descriptions------------------------

// Accepts incoming connections and launches the sessions
class listener : public std::enable_shared_from_this<listener>
{
	net::io_context& ioc_;
	tcp::acceptor acceptor_;

public:
	listener(
		net::io_context& ioc,
		tcp::endpoint endpoint)
		: ioc_(ioc)
		, acceptor_(ioc)
	{
		beast::error_code ec;

		// Opens the acceptor
		acceptor_.open(endpoint.protocol(), ec);
		if(ec)
		{
			fail(ec, "open");
			return;
		}

		// Allow address reuse
		acceptor_.set_option(net::socket_base::reuse_address(true), ec);
		if(ec)
		{
			fail(ec, "set_option");
			return;
		}

		// Bind to the server address
		acceptor_.bind(endpoint, ec);
		if(ec)
		{
			fail(ec, "bind");
			return;
		}

		// Start listening for connections
		acceptor_.listen(
			net::socket_base::max_listen_connections, ec);
		if(ec)
		{
			fail(ec, "listen");
			return;
		}
	}

	// Start accepting incoming connections
	void
	run()
	{
		do_accept();
	}

private:
	void
	do_accept()
	{
		// The new connection gets its own strand
		acceptor_.async_accept(
			net::make_strand(ioc_),
			beast::bind_front_handler(
				&listener::on_accept,
				shared_from_this()));
	}

	void
	on_accept(beast::error_code ec, tcp::socket socket)
	{
		if(ec)
		{
			fail(ec, "accept");
		}
		else
		{
			// Create the session and run it
			std::make_shared<session>(std::move(socket))->run();
		}

		// Accept another connection
		do_accept();
	}
};
//-------------------End of listener description-------------------------

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
