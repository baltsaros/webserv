#include "../inc/Server.hpp"

// Constructors
ws::Server::Server() {}

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

ws::Server::Server(Server const &src) {
	*this = src;
}

ws::Server::~Server() {
	delete _socket;
}

ws::Server&	ws::Server::operator=(Server const &rhs) {
	if (this != &rhs) {
		_timeout = rhs._timeout;
		_socket = rhs._socket;
		_req = rhs._req;
		_sockfd = rhs._sockfd;
		_max_sd = rhs._max_sd;
		_working_set = rhs._working_set;
		_master_set = rhs._master_set;
	}
	return (*this);
}

// Accept a connection on our socket and creates a new socket ft that is linked to the original one
// Receive a message from the socket _sockfd
void	ws::Server::accepter() {
	std::cout << "Accepting" << std::endl;
	struct sockaddr_in	address = _socket->get_address();
	int	addrlen = sizeof(address);

	_sockfd = accept(_socket->get_socket(), (struct sockaddr *)&address,
			(socklen_t *)&addrlen);
	if (_sockfd == -1 && errno == EAGAIN)
		return;
	if (_sockfd == -1)
		test_connection(_sockfd);
}

// Print the received message
void	ws::Server::handler() {
	std::cout << "Reading" << std::endl;
	ssize_t	valread = recv(_sockfd, _buf, sizeof(_buf), MSG_DONTWAIT);
	// With MSG_DONTWAIT recv won't block socket if there is nothing to read
	if (valread == -1 && errno == EAGAIN)
		return ;
	if (valread == -1) {
		test_connection(valread);
		return ;
	}
	if (valread == 0) {
		std::cout << "Server was shut down" << std::endl;
		close(_sockfd);
		return ;
	}
	if (_buf) {
		// std::cout << _buf << std::endl;
		Request req(_buf);
		_req = req;
	}
}

// Send a response back
void	ws::Server::responder() {
	int			ret;
	// std::cout << "code: " << _req.getErrorCode() << std::endl;
	if (_req.getErrorCode() >= 0)
		ret = checkRequest();
	else {
		std::string	header = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: ";
		std::string	msg = "Testing";
		std::string	response;

		response = header + std::to_string(msg.size());
		response += "\r\n\r\n";
		response += msg;
		ret = send(_sockfd, response.c_str(), response.size(), 0);
	}
	// std::cout << to_send << std::endl;
	if (ret == -1 && errno == EAGAIN) {
		return ;
	}
	else if (ret == -1) {
		test_connection(ret);
		return ;
	}
}

// check for error code; had to use absolute path for errors;
int		ws::Server::checkRequest() {
	int	errorCode = _req.getErrorCode();
	int	ret = 0;
	std::string error;
	std::string response;
	switch (errorCode) {
		case 0:
			error = read_file("web/error_pages/400.html");
			response = "HTTP/1.1 400 Bad Request\r\n";
			break;
		case 3:
			error = read_file("web/error_pages/403.html");
			response = "HTTP/1.1 403 Forbidden\r\n";
			break;
		case 4:
			error = read_file("web/error_pages/404.html");
			response = "HTTP/1.1 404 Not Found\r\n";
			break;
		case 5:
			error = read_file("web/error_pages/405.html");
			response = "HTTP/1.1 405 Method Not Allowed\r\n";
			break;
		default:
			break;
	}
	response += "Content-Type: text/html\r\n";
	response += "Content-Length: " + std::to_string(error.length()) + "\r\n";
	response += "\r\n";
	response += error;
	ret = send(_sockfd, response.c_str(), response.size(), 0);
	return (ret);
}

void	ws::Server::launcher() {
	int	ret;

	while (19) {
		// Select() erases used fd from a set; so we need to use a copy of our original set
		_working_set = _master_set;
		ret = select(_max_sd + 1, &_working_set, NULL, NULL, &_timeout);
		if (ret == -1)
			test_connection(ret);
		if (ret == 0) {
			std::cout << "Timeout" << std::endl;
			break ;
		}
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
					handler();
					responder();
				}
				else { // If it is not server socket; just read it and clear it from the set
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
	std::cerr << "Webserver error: " << strerror(to_test) << std::endl;
	// if (to_test < 0) {
		// std::cout << "Webserver error: " << strerror(to_test) << std::endl;
		// exit(EXIT_FAILURE);
	// }
}

std::string	ws::Server::read_file(const std::string& filename) {
	std::ifstream	input_file(filename);
	std::stringstream	buffer;
	buffer << input_file.rdbuf();
	return buffer.str();
}

// Getters
ws::Socket*	ws::Server::get_server_sd() {
	return (_socket);
}