// File: listener.h
// Purpose: Implements the listener class

#ifndef LISTENER_H
#define LISTENER_H

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
			std::shared_ptr<session> newSession = std::make_shared<session>(std::move(socket));
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
		do_accept();
	}
};

#endif
