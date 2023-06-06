#include "../inc/Server.hpp"

// Constructors
ws::Server::Server() {}

ws::Server::Server(int domain, int service, int protocol,
					std::vector<int> ports, u_long interface, int backlog,
					Configuration config) : _config(config) {
	_socket = new Socket(domain, service, protocol, ports, interface, backlog);
	_max_sd = _socket->get_maxsd();
	_sockfds = _socket->get_sockets();
	// Initialize set
	FD_ZERO(&_master_set);
	// Add a new socket/sockets to the set
	for (std::vector<int>::iterator it = _sockfds.begin(); it != _sockfds.end(); ++it)
		FD_SET((*it), &_master_set);
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
		_config = rhs._config;
		_timeout = rhs._timeout;
		_socket = rhs._socket;
		_req = rhs._req;
		_sockfds = rhs._sockfds;
		_max_sd = rhs._max_sd;
		_working_set = rhs._working_set;
		_master_set = rhs._master_set;
	}
	return (*this);
}

// Accept a connection on our socket and creates a new socket ft that is linked to the original one
// Receive a message from the socket _sockfd
int		ws::Server::accepter(int sockfd) {
	std::cout << "Accepting" << std::endl;
	struct sockaddr_in	address = _socket->get_address();
	int	new_sd;
	int	addrlen = sizeof(address);

	new_sd = accept(sockfd, (struct sockaddr *)&address,
			(socklen_t *)&addrlen);
	return new_sd;
}

// Print the received message
int		ws::Server::handler(int i) {
	std::cout << "Reading" << std::endl;
	_buf.clear();
	char	cbuf[BUFFER_SIZE];
	ssize_t	bytesRead;
	// keep reading incoming data until there is nothing left to read
	do {
		// With MSG_DONTWAIT recv won't block socket if there is nothing to read
		bytesRead = recv(i, cbuf, BUFFER_SIZE - 1, MSG_DONTWAIT);
		if (bytesRead > 0) {
			cbuf[bytesRead] = '\0';
			_buf += cbuf;
		}
		// -1 means error; 0 - connection was closed by the client
		else if (bytesRead == 0) {
			std::cout << "Connection was closed" << std::endl;
			return 0;
		}
		else
			return -1;
	} while (bytesRead == BUFFER_SIZE - 1);
	if (_buf.size() > 0) {
		// std::cout << _buf << std::endl;
		Request req(_buf, _config);
		_req = req;
	}
	return 1;
}

// Send a response back
int		ws::Server::responder(int i) {
	int			ret;
	Response	response(_req, _config);
	std::string	toSend;

	toSend = response.getResponse();
	ret = send(i, toSend.c_str(), toSend.size(), 0);
	if (ret == -1) {
		std::cerr << "Send() error" << std::endl;
		return -1;
	}
	return ret;
}

bool	ws::Server::checkSocket(int i) {
	std::vector<int>::iterator	it;

	for (it = _sockfds.begin(); it != _sockfds.end(); ++it) {
		if (i == *it)
			return true;
	}
	return false;
}

void	ws::Server::launcher() {
	int		ret = 0, sds_ready = 0, new_sd = 0;
	bool	end_server = false, close_conn = false;

	while (!end_server) {
		// Select() erases used fds from a set, so we need to use a copy of our original set
		memcpy(&_working_set, &_master_set, sizeof(_master_set));
		ret = select(_max_sd + 1, &_working_set, NULL, NULL, &_timeout);
		// select() in this case returns the number of readable descriptors
		// ret == -1 means an error; ret == 0 means timeout
		if (ret == -1) {
			std::cerr << "Select() Error" << std::endl;
			break ;
		}
		else if (ret == 0) {
			std::cout << "Timeout" << std::endl;
			break ;
		}
		sds_ready = ret;
		std::cout << "==== WAITING ====" << std::endl;
		for (int i = 0; i <= _max_sd && sds_ready > 0; ++i) {
			// Check what descriptor (socket) is ready for connection
			if (FD_ISSET(i, &_working_set)) {
				sds_ready -= 1;
				// check if i is the listening socket
				if (checkSocket(i)) {
					// if so, we need to accept all incoming connections
					while (19) {
						new_sd = accepter(i);
						if (new_sd < 0) {
							// EWOULDBLOCK in this case means that there are no more
							// pending connections on the queue
							if (errno != EWOULDBLOCK) {
								std::cerr << "Accept() error" << std::endl;
								end_server = true;
							}
							break ;
						}
						std::cout << "max socket: " << _max_sd << std::endl;
						std::cout << "server socket: " << new_sd << std::endl;
						// adding the new incoming connection to the master set
						FD_SET(new_sd, &_master_set);
						if (new_sd > _max_sd)
							_max_sd = new_sd;
						if (new_sd == -1)
							break ;
					}
				}
				else {
					// if it is not a listnening socket, we don't need to accept
					// an incoming connection. The existing connections is readable
					close_conn = false;
					while (19) {
						// receive all incoming data
						ret = handler(i);
						if (!ret) {
							close_conn = true;
							break ;
						}
						else if (ret == -1)
							break ;
						// send response back to the client
						ret = responder(i);
						if (ret == -1) {
							close_conn = true;
							break ;
						}
					}
					// if close_conn flag was turned on (recv/send errors)
					// we need to close the active connection,
					// remove it from the master_set and decrease the value of
					// max socket in the master set
					if (close_conn) {
						close(i);
						FD_CLR(i, &_master_set);
						if (i == _max_sd) {
							while (!FD_ISSET(_max_sd, &_master_set))
								_max_sd -= 1;
						}
					}
				} // else
			} // FD_ISSET
		} // for loop to check every descriptor
		std::cout << "==== DONE ====" << std::endl;
	} // main loop begins
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

// Getters
ws::Socket*	ws::Server::get_server_sd() {
	return (_socket);
}