#include "../inc/Socket.hpp"

// Constructors
ws::Socket::Socket() {}

// constructor takes the following parameters:
// domain - what address family we use (IPv4 or IPv6 for example)
// service - what socket type we use (SOCK_STREAM means two way connections, TCP socket)
// protocol - what protocol to use with the socket; 0 is default value
// interface - what IP addresses the socket should listen to; INADDR_ANY listens to all
// IPv4 addresses, value is 0.0.0.0
// backlog - specifies the maximum number of clients
// that can be waiting to be served while the server is handling other connections
// generation - tracks to what server block a socket belongs; required for proper
// memory clean up
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

// Bind a socket to the netwok and turn it into a listening socket
void	ws::Socket::connect_to_network(int sockfd) {
	int	ret = 0;

	// std::cout << "Binding" << std::endl;
	ret = bind(sockfd, (struct sockaddr *)&_address, sizeof(_address));
	test_connection(ret, sockfd);
	// std::cout << "Listening" << std::endl;
	ret = listen(sockfd, _backlog);
	test_connection(ret, sockfd);
	_max_sd = sockfd;
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
