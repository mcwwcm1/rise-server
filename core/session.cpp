#include "session.h"

#include "core/commands.h"
#include "utilities.h"

std::mutex Session::_headlessMutex;
Session* Session::_headlessSession = nullptr;
std::unordered_map<std::string, Session*> Session::_registeredUsers;

Session* Session::GetHeadless() { return _headlessSession; }

Session* Session::GetUserSession(const std::string& userID)
{
	const auto it = _registeredUsers.find(userID);
	if (it != _registeredUsers.end()) { return it->second; }

	return nullptr;
}

Session::Session(boost::asio::ip::tcp::socket&& socket) : _ws(std::move(socket))
{
	if (_headlessSession == nullptr) {
		std::lock_guard<std::mutex> lock(_headlessMutex);
		_headlessSession = this;
	}
}

Session::~Session()
{
	if (!_userID.empty()) {
		const auto it = _registeredUsers.find(_userID);
		if (it != _registeredUsers.end()) {
			_registeredUsers.erase(it);
			printf("Unregistered session for user \"%s\".\n", _userID.c_str());
		}
	}
}

void Session::Run()
{
	//For async functionality and thread safety
	boost::asio::dispatch(
			_ws.get_executor(),
			boost::beast::bind_front_handler(&Session::OnRun, shared_from_this()));
}

void Session::OnRun()
{
	// Set the suggested tiemout settings for the websocket
	_ws.set_option(boost::beast::websocket::stream_base::timeout::suggested(
			boost::beast::role_type::server));
	// Set a decorator to change the Server of the handshake
	_ws.set_option(boost::beast::websocket::stream_base::decorator(
			[](boost::beast::websocket::response_type& res) {
				res.set(boost::beast::http::field::server,
		            std::string(BOOST_BEAST_VERSION_STRING) + " rise-server");
			}));
	// Accept the websocket handshake
	_ws.async_accept(
			boost::beast::bind_front_handler(&Session::OnAccept, shared_from_this()));
}

void Session::OnAccept(boost::beast::error_code ec)
{
	if (ec) { return BoostFail(ec, "accept"); }
	// Read a message
	DoRead();
}

void Session::DoRead()
{
	// Read a message into our buffer
	_ws.async_read(
			_buffer,
			boost::beast::bind_front_handler(&Session::OnRead, shared_from_this()));
}

void Session::OnRead(boost::beast::error_code ec, std::size_t bytes_transferred)
{
	boost::ignore_unused(bytes_transferred);
	// This indicates that the session was closed
	if (ec == boost::beast::websocket::error::closed) return;
	if (ec) { BoostFail(ec, "read"); }

	//Separate function call and arguments
	std::string message   = boost::beast::buffers_to_string(_buffer.data());
	std::string function  = message.substr(0, message.find(" "));
	std::string arguments = message.substr(message.find(" ") + 1, message.length());

	auto parsingFn = Commands::Get(function);

	// Check if user is making a registration call
	if (function == "register") {
		if (_userID == "") {
			_userID                   = arguments;
			_registeredUsers[_userID] = this;
			printf("Registered session for user \"%s\".\n", _userID.c_str());
		}
	} else if (parsingFn == nullptr) {
		// Send error message
		printf("Function NOT FOUND IDIOT!!@!!!!1!11!: %s\n", function.c_str());
	} else if (arguments == "") {
		// Check for argument
		printf("NO ARGUMENT STUHPID\n");
	} else {
		// Call the function
		try {
			parsingFn(arguments);
		} catch (const std::invalid_argument& e) {
			printf("Failed to parse command \"%s\": %s\n", function.c_str(), e.what());
		}
	}
	// Clear the buffer
	_buffer.consume(_buffer.size());

	//Do another read
	DoRead();
}

void Session::OnWrite(boost::beast::error_code ec,
                      std::size_t bytes_transferred)
{
	boost::ignore_unused(bytes_transferred);
	// Clear the buffer
	_buffer.consume(_buffer.size());
	// Release the lock
	_mutex.unlock();
}

void Session::OnSend(boost::shared_ptr<std::string const> const& ss)
{
	//printf("Send mutex locked\n");
	// Grab a lock
	_mutex.lock();
	// We are not currently writing, so send this immediately
	_ws.async_write(
			boost::asio::buffer(*ss),
			boost::beast::bind_front_handler(&Session::OnWrite, shared_from_this()));
}

void Session::Send(const std::string& str)
{
	const auto boostStr = boost::make_shared<std::string const>(std::move(str));
	// Post our work to the strand, this ensures
	// that the members of `this` will not be
	// accessed concurrently.
	boost::asio::post(_ws.get_executor(),
	                  boost::beast::bind_front_handler(
												&Session::OnSend, shared_from_this(), boostStr));
}