// File: session.h
// Purpose: Defines a single websocket session

#ifndef SESSION_H
#define SESSION_H

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>

#include "fail.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session>
{
private:
	websocket::stream<beast::tcp_stream> ws_;
	beast::flat_buffer buffer_;
	/*
	//Declare the map to be used for command parsing (populated in main)
	std::unordered_map<std::string, parsing_function> parseMap;
	*/
public:
	explicit session(tcp::socket&& socket) : ws_(std::move(socket)) {};
	void run();
	void on_run();
	void on_accept(beast::error_code ec);
	void do_read();
	void on_read(beast::error_code ec, std::size_t bytes_transferred);
	void on_write(beast::error_code ec, std::size_t bytes_transferred);
};

#endif
