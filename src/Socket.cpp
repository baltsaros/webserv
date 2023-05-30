#include "../inc/Socket.hpp"

// Constructors
ws::Socket::Socket() {}

ws::Socket::Socket(int domain, int service, int protocol, int port,
					u_long interface, int backlog) {
	int	ret;
	int	on = 1;

	// Setting server parameters
	_address.sin_family = domain;
	_address.sin_port = htons(port);
	_address.sin_addr.s_addr = htonl(interface);
	// _address.sin_addr.s_addr = INADDR_ANY;

	// Creating endpoint for communication
	_sockfd = socket(domain, service, protocol);
	test_connection(_sockfd);
	_backlog = backlog;
	// Allow _sockfd to be reusable
	ret = setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	test_connection(ret);
	// Make the socket non blocking
	ret = fcntl(_sockfd, F_SETFL, O_NONBLOCK);
	test_connection(ret);
	connect_to_network();
	return ;
}

ws::Socket::Socket(Socket const &src) {
	*this = src;
}

ws::Socket::~Socket() {}

ws::Socket&	ws::Socket::operator=(Socket const &rhs) {
	if (this != &rhs) {
		_sockfd = rhs._sockfd;
		_max_sd = rhs._max_sd;
		_backlog = rhs._backlog;
		_address = rhs._address;
	}
	return (*this);
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
	_max_sd = _sockfd;
	// // Initialize set
	// FD_ZERO(&_master_set);
	// // Add a new socket to the set
	// FD_SET(_sockfd, &_master_set);
}

// Check for errors; socket, bind and listen return -1 on error and set errno to the appropriate value
void	ws::Socket::test_connection(int to_test) {
	if (to_test < 0) {
		std::cerr << "Webserver error: " << strerror(to_test) << std::endl;
		exit(EXIT_FAILURE);
	}
}

// Getters
struct sockaddr_in	ws::Socket::get_address() const {
	return (_address);
}

int	ws::Socket::get_socket() const {
	return (_sockfd);
}

int	ws::Socket::get_maxsd() const {
	return (_max_sd);
}

// fd_set	ws::Socket::get_masterset() const {
// 	return (_master_set);
// }
