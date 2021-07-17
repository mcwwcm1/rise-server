// File: session.h
// Purpose: Implements a single websocket session

#ifndef SESSION_H
#define SESSION_H

// Echoes back all received WebSocket messages
class session : public std::enable_shared_from_this<session>
{
	websocket::stream<beast::tcp_stream> ws_;
	beast::flat_buffer buffer_;
	// Declare mutex to protect against overlapping writes
	std::mutex mutex_;
	// Holds current UserID
	std::string userID = "";
public:
	// Take ownership of the socket
	explicit
	session(tcp::socket&& socket)
		: ws_(std::move(socket))
	{
	}
	// Destructor to clean up registeredUsers
	~session()
	{
		UnRegisterUser(userID);
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
						" rise-server");
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
        if(ec) {
            return fail(ec, "accept");
		}
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

		//Separate function call and arguments
		std::string message = boost::beast::buffers_to_string(buffer_.data());
		std::string function = message.substr(0, message.find(" "));
		std::string arguments = message.substr(message.find(" ") + 1, message.length());

		// Check if user is making a registration call
		if(function=="register") {
			if(userID=="") {
				RegisterUser(arguments, this);
				userID = arguments;
			}
		} else if(parseMap.find(function)==parseMap.end()) {
			// Send error message
			printf(("Function NOT FOUND IDIOT!!@!!!!1!11!: " + function + "\n").c_str());
		} else if (arguments!="") {
			// Check for argument
			printf("NO ARGUMENT STUHPID\n");
		} else {
			// Call the function
			parseMap[function](arguments);
		}
		// Clear the buffer
		buffer_.consume(buffer_.size());

		//Do another read
		do_read();
	}
	void on_write(
		beast::error_code ec,
		std::size_t bytes_transferred)
	{
		boost::ignore_unused(bytes_transferred);
		//if(ec)
		//	return fail(ec, "write");
		// Clear the buffer
		buffer_.consume(buffer_.size());
		// Release the lock
		mutex_.unlock();
	}
	void on_send(boost::shared_ptr<std::string const> const& ss)
	{
		// We are not currently writing, so send this immediately
		ws_.async_write(
			net::buffer(*ss),
			beast::bind_front_handler(
				&session::on_write,
				shared_from_this()));
		printf("on_send ran\n");
	}
	void send(boost::shared_ptr<std::string const> const& ss)
	{
		// Grab a lock
		mutex_.lock();

		// Post our work to the strand, this ensures
    	// that the members of `this` will not be
    	// accessed concurrently.
    	net::post(
        	ws_.get_executor(),
        	beast::bind_front_handler(
            	&session::on_send,
            	shared_from_this(),
            	ss));
		printf("send ran\n");
	}

};

#endif
