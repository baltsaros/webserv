#include "../inc/Socket.hpp"

// Constructors
ws::Socket::Socket() {}

ws::Socket::Socket(int domain, int service, int protocol, std::vector<int> ports,
					u_long interface, int backlog, int generation) {
	int	ret;
	int	on = 1;
	int	sockfd;
	std::vector<int>::iterator	it;

	_generation = generation;
	for (it = ports.begin(); it != ports.end(); ++it) {
		// Setting server parameters
		_address.sin_family = domain;
		_address.sin_port = htons(*it);
		_address.sin_addr.s_addr = htonl(interface);
		// _address.sin_addr.s_addr = INADDR_ANY;

		// Creating endpoint for communication
		sockfd = socket(domain, service, protocol);
		test_connection(sockfd, sockfd);
		_backlog = backlog;
		// Allow _sockfd to be reusable
		ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
		test_connection(ret, sockfd);
		// Make the socket non blocking
		ret = fcntl(sockfd, F_SETFL, O_NONBLOCK);
		test_connection(ret, sockfd);
		connect_to_network(sockfd);
		_sockfds.push_back(sockfd);
	}
	return ;
}

ws::Socket::Socket(Socket const &src) {
	*this = src;
}

ws::Socket::~Socket() {}

ws::Socket&	ws::Socket::operator=(Socket const &rhs) {
	if (this != &rhs) {
		_sockfds = rhs._sockfds;
		_max_sd = rhs._max_sd;
		_backlog = rhs._backlog;
		_address = rhs._address;
		_generation = rhs._generation;
	}
	return (*this);
}

// Assigning _address to _sockfd and listen for new connections
void	ws::Socket::connect_to_network(int sockfd) {
	int	ret = 0;

	std::cout << "Binding" << std::endl;
	ret = bind(sockfd, (struct sockaddr *)&_address, sizeof(_address));
	test_connection(ret, sockfd);
	std::cout << "Listening" << std::endl;
	ret = listen(sockfd, _backlog);
	test_connection(ret, sockfd);
	_max_sd = sockfd;
	// // Initialize set
	// FD_ZERO(&_master_set);
	// // Add a new socket to the set
	// FD_SET(_sockfd, &_master_set);
}

void	ws::Socket::setGeneration(int generation) {_generation = generation;}

// Check for errors; socket, bind and listen return -1 on error and set errno to the appropriate value
void	ws::Socket::test_connection(int to_test, int socket) {
	if (to_test < 0) {
		std::cerr << "Webserver error: " << strerror(to_test) << std::endl;
		close(socket);
		exit(EXIT_FAILURE);
	}
}

// Getters
struct sockaddr_in	ws::Socket::getAddress() const {return _address;}

std::vector<int>	ws::Socket::getSockets() const {return _sockfds;}

int	ws::Socket::getMaxsd() const {return _max_sd;}

int	ws::Socket::getGeneration() const {return _generation;}
