// File: session.cpp
// Purpose: Implements session.h

#include "session.h"

// Take ownership of the socket
/*
session::session(tcp::socket&& socket) : ws_(std::move(socket))
{

}
*/
// Get on the correct executor
void session::run()
{
	//For async functionality and thread safety
	net::dispatch(ws_.get_executor(),
		beast::bind_front_handler(
			&session::on_run,
			shared_from_this()));
}

// Start the asynchronous operation
void session::on_run()
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
					" rise server");
		}));

	// Accept the websocket handshake
	ws_.async_accept(
		beast::bind_front_handler(
			&session::on_accept,
			shared_from_this()));
}

void session::on_accept(beast::error_code ec)
{
	if(ec)
		return fail(ec, "accept");

	// Read a message
	do_read();
}

void session::do_read()
{
	// Read a message into our buffer
	ws_.async_read(
		buffer_,
		beast::bind_front_handler(
			&session::on_read,
			shared_from_this()));
}

void session::on_read(
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

void session::on_write(
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
