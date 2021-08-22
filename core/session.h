// File: session.h
// Purpose: Implements a single websocket session

#pragma once

// Echoes back all received WebSocket messages
class Session : public std::enable_shared_from_this<Session>
{
	websocket::stream<beast::tcp_stream> _ws;
	beast::flat_buffer _buffer;
	// Declare mutex to protect against overlapping writes
	std::mutex _mutex;
	// Holds current UserID
	std::string _userID = "";

 public:
	// Take ownership of the socket
	explicit Session(tcp::socket&& socket) : _ws(std::move(socket)) {}
	// Destructor to clean up registeredUsers
	~Session() { UnRegisterUser(_userID); }
	// Get on the correct executor
	void Run()
	{
		//For async functionality and thread safety
		net::dispatch(
				_ws.get_executor(),
				beast::bind_front_handler(&Session::OnRun, shared_from_this()));
	}
	// Start the asynchronous operation
	void OnRun()
	{
		// Set the suggested tiemout settings for the websocket
		_ws.set_option(
				websocket::stream_base::timeout::suggested(beast::role_type::server));
		// Set a decorator to change the Server of the handshake
		_ws.set_option(
				websocket::stream_base::decorator([](websocket::response_type& res) {
					res.set(http::field::server,
			            std::string(BOOST_BEAST_VERSION_STRING) + " rise-server");
				}));
		// Accept the websocket handshake
		_ws.async_accept(
				beast::bind_front_handler(&Session::OnAccept, shared_from_this()));
	}
	void OnAccept(beast::error_code ec)
	{
		if (ec) { return fail(ec, "accept"); }
		// Read a message
		DoRead();
	}
	void DoRead()
	{
		// Read a message into our buffer
		_ws.async_read(
				_buffer,
				beast::bind_front_handler(&Session::OnRead, shared_from_this()));
	}
	void OnRead(beast::error_code ec, std::size_t bytes_transferred)
	{
		boost::ignore_unused(bytes_transferred);
		// This indicates that the session was closed
		if (ec == websocket::error::closed) return;
		if (ec) fail(ec, "read");

		//Separate function call and arguments
		std::string message  = boost::beast::buffers_to_string(_buffer.data());
		std::string function = message.substr(0, message.find(" "));
		std::string arguments =
				message.substr(message.find(" ") + 1, message.length());
		//printf(("Function received: " + function + "\n").c_str());
		//printf(("Argument received: " + arguments + "\n").c_str());

		// Check if user is making a registration call
		if (function == "register") {
			if (_userID == "") {
				RegisterUser(arguments, this);
				_userID = arguments;
			}
		} else if (parseMap.find(function) == parseMap.end()) {
			// Send error message
			printf(
					("Function NOT FOUND IDIOT!!@!!!!1!11!: " + function + "\n").c_str());
		} else if (arguments == "") {
			// Check for argument
			printf("NO ARGUMENT STUHPID\n");
		} else {
			// Call the function
			parseMap[function](arguments);
		}
		// Clear the buffer
		_buffer.consume(_buffer.size());

		//Do another read
		DoRead();
	}
	void OnWrite(beast::error_code ec, std::size_t bytes_transferred)
	{
		boost::ignore_unused(bytes_transferred);
		//if(ec)
		//	return fail(ec, "write");
		// Clear the buffer
		_buffer.consume(_buffer.size());
		// Release the lock
		_mutex.unlock();
		//printf("Send mutex unlocked\n");
	}
	void OnSend(boost::shared_ptr<std::string const> const& ss)
	{
		//printf("Send mutex locked\n");
		// Grab a lock
		_mutex.lock();
		// We are not currently writing, so send this immediately
		_ws.async_write(
				net::buffer(*ss),
				beast::bind_front_handler(&Session::OnWrite, shared_from_this()));
		//printf("on_send ran\n");
	}
	void Send(boost::shared_ptr<std::string const> const& ss)
	{
		// Post our work to the strand, this ensures
		// that the members of `this` will not be
		// accessed concurrently.
		net::post(
				_ws.get_executor(),
				beast::bind_front_handler(&Session::OnSend, shared_from_this(), ss));
		//printf("send ran\n");
	}
};
