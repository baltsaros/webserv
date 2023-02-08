#include "Server.hpp"

ws::Server::Server(int domain, int service, int protocol,
					int port, u_long interface, int backlog) {
	_socket = new Socket(domain, service, protocol, port, interface, backlog);
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

	std::cout << "Reading" << std::endl;
	ssize_t	valread = recv(_sockfd, _buf, 30000, 0);
	test_connection(valread);
}

// Print the received message
void	ws::Server::handler() {
	std::cout << _buf << std::endl;
}

// Send a responce back
void	ws::Server::responder() {
	std::string	header = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: ";
	std::string	msg = "Testing";
	std::string	to_send;
	int			ret;

	to_send = header + std::to_string(msg.size());
	to_send += "\r\n\r\n";
	to_send += msg;
	ret = send(_sockfd, to_send.c_str(), to_send.size(), 0);
	std::cout << to_send << std::endl;
	test_connection(ret);
	close(_sockfd);
}

void	ws::Server::launcher() {
	while (19) {
		std::cout << "==== WAITING ====" << std::endl;
		accepter();
		handler();
		responder();
		std::cout << "==== DONE ====" << std::endl;
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