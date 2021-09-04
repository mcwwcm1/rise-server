#include "Listener.h"

#include "Core/Session.h"

#include "Utilities.h"

Listener::Listener(boost::asio::io_context& ioc,
                   boost::asio::ip::tcp::endpoint endpoint)
		: _ioc(ioc), _acceptor(ioc)
{
	boost::beast::error_code ec;

	// Opens the acceptor
	_acceptor.open(endpoint.protocol(), ec);
	if (ec) {
		BoostFail(ec, "open");
		return;
	}

	// Allow address reuse
	_acceptor.set_option(boost::asio::socket_base::reuse_address(true), ec);
	if (ec) {
		BoostFail(ec, "set_option");
		return;
	}

	// Bind to the server address
	_acceptor.bind(endpoint, ec);
	if (ec) {
		BoostFail(ec, "bind");
		return;
	}

	// Start listening for connections
	_acceptor.listen(boost::asio::socket_base::max_listen_connections, ec);
	if (ec) {
		BoostFail(ec, "listen");
		return;
	}
}

void Listener::Run() { DoAccept(); }

void Listener::DoAccept()
{
	// The new connection gets its own strand
	_acceptor.async_accept(boost::asio::make_strand(_ioc),
	                       boost::beast::bind_front_handler(&Listener::OnAccept,
	                                                        shared_from_this()));
}

void Listener::OnAccept(boost::beast::error_code ec,
                        boost::asio::ip::tcp::socket socket)
{
	if (ec) {
		BoostFail(ec, "accept");
	} else {
		std::shared_ptr<Session> newSession =
				std::make_shared<Session>(std::move(socket));
		newSession->Run();
	}

	// Accept another connection
	DoAccept();
}