// File: listener.h
// Purpose: Implements the listener class

#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <memory>

// Accepts incoming connections and launches the sessions
class Listener : public std::enable_shared_from_this<Listener>
{
 public:
	Listener(boost::asio::io_context& ioc,
	         boost::asio::ip::tcp::endpoint endpoint);

	// Start accepting incoming connections
	void Run();

 private:
	void DoAccept();
	void OnAccept(boost::beast::error_code ec,
	              boost::asio::ip::tcp::socket socket);

	boost::asio::io_context& _ioc;
	boost::asio::ip::tcp::acceptor _acceptor;
};
