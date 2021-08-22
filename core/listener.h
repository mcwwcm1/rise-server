// File: listener.h
// Purpose: Implements the listener class

#ifndef LISTENER_H
#define LISTENER_H

// Accepts incoming connections and launches the sessions
class Listener : public std::enable_shared_from_this<Listener>
{
	net::io_context& _ioc;
	tcp::acceptor _acceptor;

 public:
	Listener(net::io_context& ioc, tcp::endpoint endpoint)
			: _ioc(ioc), _acceptor(ioc)
	{
		beast::error_code ec;

		// Opens the acceptor
		_acceptor.open(endpoint.protocol(), ec);
		if (ec) {
			fail(ec, "open");
			return;
		}

		// Allow address reuse
		_acceptor.set_option(net::socket_base::reuse_address(true), ec);
		if (ec) {
			fail(ec, "set_option");
			return;
		}

		// Bind to the server address
		_acceptor.bind(endpoint, ec);
		if (ec) {
			fail(ec, "bind");
			return;
		}

		// Start listening for connections
		_acceptor.listen(net::socket_base::max_listen_connections, ec);
		if (ec) {
			fail(ec, "listen");
			return;
		}
	}

	// Start accepting incoming connections
	void Run() { DoAccept(); }

 private:
	void DoAccept()
	{
		// The new connection gets its own strand
		_acceptor.async_accept(
				net::make_strand(_ioc),
				beast::bind_front_handler(&Listener::OnAccept, shared_from_this()));
	}

	void OnAccept(beast::error_code ec, tcp::socket socket)
	{
		if (ec) {
			fail(ec, "accept");
		} else {
			// Create the session and run it
			/*
			if(!headlessSession) {
				headlessSession = newSession.get();

			}
			*/
			std::shared_ptr<session> newSession =
					std::make_shared<session>(std::move(socket));
			if (!headlessSession) {
				std::lock_guard<std::mutex> lck(headlessPointerMutex);
				headlessSession = newSession.get();
				printf("Headless pointer set!\n");
			}
			newSession->run();
			/*
			//Check if headless has been selected and initialize self as headless if not
			std::shared_ptr<session> sp(headlessSession);
			if(!sp) {
				headlessSession = newSession;
				printf("Pointer has been set.");
			}
			*/
		}

		// Accept another connection
		DoAccept();
	}
};

#endif
