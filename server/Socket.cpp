#include "Socket.hpp"

// Default constructor
ws::Socket::Socket(int domain, int service, int protocol, int port,
					u_long interface, int backlog) {
	// Setting server parameters
	_address.sin_family = domain;
	_address.sin_port = htons(port);
	_address.sin_addr.s_addr = htonl(interface);
	_address.sin_addr.s_addr = INADDR_ANY;

	// Creating endpoint for communication
	_sockfd = socket(domain, service, protocol);
	test_connection(_sockfd);
	_backlog = backlog;
	connect_to_network();
	return ;
}

// Assigning _address to _sockfd and listen for new connections
void	ws::Socket::connect_to_network(void) {
	int	ret = 0;

	std::cout << "Binding" << std::endl;
	ret = bind(_sockfd, (struct sockaddr *)&_address, sizeof(_address));
	test_connection(ret);
	std::cout << "Listening" << std::endl;
	ret = listen(_sockfd, _backlog);
	test_connection(ret);
}

// Check for errors; socket, bind and listen return -1 on error and set errno to the appropriate value
void	ws::Socket::test_connection(int to_test) {
	if (to_test < 0) {
		std::cout << "Webserver error: " << strerror(to_test) << std::endl;
		exit(EXIT_FAILURE);
	}
}

// Getters
struct sockaddr_in	ws::Socket::get_address() {
	return (_address);
}

int	ws::Socket::get_socket() {
	return (_sockfd);
}
