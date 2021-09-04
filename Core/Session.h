// File: session.h
// Purpose: Implements a single websocket session

#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <memory>
#include <mutex>
#include <unordered_map>

// Echoes back all received WebSocket messages
class Session : public std::enable_shared_from_this<Session>
{
 public:
	// Take ownership of the socket
	explicit Session(boost::asio::ip::tcp::socket&& socket);
	// Destructor to clean up registeredUsers
	~Session();

	// Get on the correct executor
	void Run();

	void Send(const std::string& str);

	static Session* GetHeadless();
	static Session* GetUserSession(const std::string& userID);

 private:
	// Start the asynchronous operation
	void OnRun();
	void OnAccept(boost::beast::error_code ec);
	void DoRead();
	void OnRead(boost::beast::error_code ec, std::size_t bytes_transferred);
	void OnWrite(boost::beast::error_code ec, std::size_t bytes_transferred);
	void OnSend(boost::shared_ptr<std::string const> const& ss);

	static std::mutex _headlessMutex;
	static Session* _headlessSession;
	static std::unordered_map<std::string, Session*> _registeredUsers;

	boost::beast::websocket::stream<boost::beast::tcp_stream> _ws;
	boost::beast::flat_buffer _buffer;
	// Declare mutex to protect against overlapping writes
	std::mutex _mutex;
	// Holds current UserID
	std::string _userID = "";
};
