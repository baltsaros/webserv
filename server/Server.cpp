#include "Server.hpp"

ws::Server::Server(int domain, int service, int protocol,
					int port, u_long interface, int backlog) {
	_socket = new Socket(domain, service, protocol, port, interface, backlog);
	_max_sd = _socket->get_maxsd();
	memset(_buf, '\0', sizeof(_buf));
	// Initialize set
	FD_ZERO(&_master_set);
	// Add a new socket to the set
	FD_SET(_socket->get_socket(), &_master_set);
	// Set timeout time for select; 3 mins in out case;
	_timeout.tv_sec = 3 * 60;
	_timeout.tv_usec = 0;
}

ws::Server::~Server() {
	delete _socket;
}

// Accept a connection on our socket and creates a new socket ft that is linked to the original one
// Receive a message from the socket _sockfd
void	ws::Server::accepter() {
	std::cout << "Accepting" << std::endl;
	struct sockaddr_in	address = _socket->get_address();
	int	addrlen = sizeof(address);

	_sockfd = accept(_socket->get_socket(), (struct sockaddr *)&address,
			(socklen_t *)&addrlen);
	test_connection(_sockfd);
}

// Print the received message
void	ws::Server::handler() {
	std::cout << "Reading" << std::endl;
	ssize_t	valread = recv(_sockfd, _buf, sizeof(_buf), 0);
	// test_connection(valread); // exits because of fcntl
	if (!valread) {
		std::cout << "Server was shut down" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << _buf << std::endl;
}

// Send a response back
void	ws::Server::responder() {
	std::string	header = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: ";
	// std::string	msg = "Hello from the Server!";
	std::string	msg = "Testing";
	std::string	to_send;
	int			ret;

	to_send = header + std::to_string(msg.size());
	to_send += "\r\n\r\n";
	to_send += msg;
	ret = send(_sockfd, to_send.c_str(), to_send.size(), 0);
	// std::cout << to_send << std::endl;
	test_connection(ret);
}

void	ws::Server::launcher() {
	int	ret;

	while (19) {
		// select() erases used fd from a set; so we need to use a copy of our original set
		_working_set = _master_set;
		ret = select(_max_sd + 1, &_working_set, NULL, NULL, &_timeout);
		test_connection(ret);
		// Check what fd is ready for connection
		std::cout << "==== WAITING ====" << std::endl;
		for (int i = 0; i <= _max_sd; ++i) {
			if (FD_ISSET(i, &_working_set)) {
				if (i == _socket->get_socket()) {
					accepter();
					FD_SET(_sockfd, &_working_set);
					std::cout << "max socket: " << _max_sd << std::endl;
					std::cout << "server socket: " << _sockfd << std::endl;
					if (_sockfd > _max_sd)
						_max_sd = _sockfd;
					responder();
				}
				else { // if it is not server socket; just read it and clear it from the set
					handler();
					FD_CLR(i, &_working_set);
				}
			}
		}
		std::cout << "==== DONE ====" << std::endl;
	}
	std::cout << "Closing fds" << std::endl;
	for (int i = 0; i <= _max_sd; ++i) {
		if (FD_ISSET(i, &_master_set))
			close(i);
	}
}

// Check for errors; accept, send, recv return -1 on error and set errno to the appropriate value
void	ws::Server::test_connection(int to_test) {
	if (to_test < 0) {
		std::cout << "Webserver error: " << strerror(to_test) << std::endl;
		exit(EXIT_FAILURE);
	}
}

// Getters
ws::Socket*	ws::Server::get_server_sd() {
	return (_socket);
}