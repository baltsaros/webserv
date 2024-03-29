#include "../inc/Server.hpp"

// Constructors
ws::Server::Server() {}

// constructor takes the following parameters:
// domain - what address family we use (IPv4 or IPv6 for example)
// service - what socket type we use (SOCK_STREAM means two way connections, TCP socket)
// protocol - what protocol to use with the socket; 0 is default value
// interface - what IP addresses the socket should listen to; INADDR_ANY listens to all
// IPv4 addresses, value is 0.0.0.0
// backlog - specifies the maximum number of clients
// that can be waiting to be served while the server is handling other connections
// config - config parameters
ws::Server::Server(int domain, int service, int protocol,
					u_long interface, int backlog,
					Configuration config) : _config(config) {
	// get all servers from the confing
	int	generation = 1;
	_servers = _config.getConfigServer();
	_max_sd = 0;
	// iterate every server block; create a socket for every port and then add the socket
	// to the vector of sockets; find max socket value
	for (std::vector<ConfigServer*>::iterator it = _servers.begin(); it != _servers.end(); ++it) {
		std::vector<int>	port = (*it)->getPorts();
		Socket	*socket = new Socket(domain, service, protocol, port, interface, backlog, generation);
		std::vector<int>	tmp = socket->getSockets();
		for (std::vector<int>::iterator it2 = tmp.begin(); it2 != tmp.end(); ++it2) {
			_socketServer[*it2] = *it;
			_socket[*it2] = socket;
			_sockfds.push_back(*it2);
			if (*it2 > _max_sd)
				_max_sd = *it2;
		}
		++generation;
	}
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
	std::map<int, Socket*>::iterator	it = _socket.begin();
	int									prev_gen = 0;

	for (; it != _socket.end(); ++it) {
		if (prev_gen != it->second->getGeneration()) {
			prev_gen = it->second->getGeneration();
			delete it->second;
		}
	}
	std::vector<ConfigServer*>				vectServers = _config.getConfigServer();
	std::vector<ConfigServer*>::iterator	itServ = vectServers.begin();
		for (; itServ != vectServers.end(); ++itServ) {
		delete *itServ;
	}
}

ws::Server&	ws::Server::operator=(Server const &rhs) {
	if (this != &rhs) {
		_config = rhs._config;
		_timeout = rhs._timeout;
		_req = rhs._req;
		_socket = rhs._socket;
		_sockfds = rhs._sockfds;
		_max_sd = rhs._max_sd;
		_buf = rhs._buf;
		_working_set = rhs._working_set;
		_master_set = rhs._master_set;
		_socketServer = rhs._socketServer;
		_servers = rhs._servers;
	}
	return (*this);
}

// Accept an incoming connection on the listening socket
// and create a new socket for that connection
int		ws::Server::_accepter(int sockfd) {
	// std::cout << "Accepting" << std::endl;
	struct sockaddr_in	address = _socket[sockfd]->getAddress();
	int	new_sd;
	int	addrlen = sizeof(address);

	new_sd = accept(sockfd, (struct sockaddr *)&address,
			(socklen_t *)&addrlen);
	_socketServer[new_sd] = _socketServer[sockfd];
	return new_sd;
}

// Read an incoming request on socket sockfd
// Keep calling recv() until all content is read or there is an error/connection closed
// Then parse the received request
// Return error value or how many bytes were read in the last recv()
int		ws::Server::_handler(int sockfd) {
	// std::cout << "Reading" << std::endl;
	_buf.clear();
	char	cbuf[BUFFER_SIZE];
	ssize_t	bytesRead;
	// keep reading incoming data until there is nothing left to read
	do {
		// With MSG_DONTWAIT recv won't block socket if there is nothing to read
		bytesRead = recv(sockfd, cbuf, BUFFER_SIZE - 1, MSG_DONTWAIT);
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
		Request req(_buf, _socketServer[sockfd]);
		_req = req;
	}
	return bytesRead;
}

// Depending on the request's method and error status, send a proper response back
// Return error status
int		ws::Server::_responder(int sockfd) {
	int			ret;
	Response	response(_req);
	std::string	toSend;

	if (_req.getReturnStatus() >= 0)
	{
		toSend = response.getResponse();
		ret = send(sockfd, toSend.c_str(), toSend.size(), 0);
	}
	else if (this->_req.getMethod() == "DELETE")
	{
		ret = this->_deleteFile(this->_req, sockfd);
	}
	else if (_checkCgi(this->_req))
	{
		CgiHandler cgi = CgiHandler(_req, sockfd);
		ret = cgi.execute();
	}
	else {
		toSend = response.getResponse();
		ret = send(sockfd, toSend.c_str(), toSend.size(), 0);
	}
	return ret;
}

// Check if socket sockfd is in the array of listening sockets
bool	ws::Server::_checkSocket(int sockfd) {
	std::vector<int>::iterator	it;

	for (it = _sockfds.begin(); it != _sockfds.end(); ++it) {
		if (sockfd == *it)
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
		// std::cout << "==== WAITING ====" << std::endl;
		for (int i = 0; i <= _max_sd && sds_ready > 0; ++i) {
			// Check what descriptor (socket) is ready for connection
			if (FD_ISSET(i, &_working_set)) {
				sds_ready -= 1;
				// check if i is the listening socket
				if (_checkSocket(i)) {
					// if so, we need to accept all incoming connections
					while (19) {
						new_sd = _accepter(i);
						if (new_sd < 0) {
							// EWOULDBLOCK in this case means that there are no more
							// pending connections on the queue
							if (errno != EWOULDBLOCK) {
								std::cerr << "Accept() error" << std::endl;
								end_server = true;
							}
							break ;
						}
						// std::cout << "max socket: " << _max_sd << std::endl;
						// std::cout << "server socket: " << new_sd << std::endl;
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
						ret = _handler(i);
						if (ret <= 0) {
							close_conn = true;
							break ;
						}
						// send response back to the client
						ret = _responder(i);
						if (ret == -1) {
							std::cerr << "Send() error" << std::endl;
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
		// std::cout << "==== DONE ====" << std::endl;
	} // main loop begins
	// std::cout << "Closing fds" << std::endl;
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

/*
** Check if the Cgi can handle the request with script we've written
** @param Request req : The request we've received
** @return boolean : True if the cgi can handle the script, false otherwise
*/
bool	ws::Server::_checkCgi(Request & req)
{
	if (req.getPath() == PATH_CGI_SCRIPT) return (true);
	if (req.getPath() == PATH_UPLOAD_SCRIPT && req.getMethod() == "POST") return (true);
	if (checkExtension(req.getPath(), ".py")) return (true);
	return (false);
}

/*
** Delete a file and send back a response with no content.
**  the user doesn't need to know if the file was deleted or not
**  so we don't need to check the return of remove function.
** @param : Request &req : the request sent to the server.
			int socketFd : the socket where we send the response.
** @return : int : The return of send to know if there was a error or not.
*/
int		ws::Server::_deleteFile(Request & req, int socketFd)
{
	std::string	response = 	"HTTP/1.1 204 No Content\n\n";
	remove((this->_req.getPath()).c_str());
	return send(socketFd, response.c_str(), response.size(), 0);
}

// Getters
std::map<int, ws::Socket*>		ws::Server::getServerSd() {return _socket;}
std::map<int, ConfigServer*>	ws::Server::getSocketServer() const {return _socketServer;}
std::vector<ConfigServer*>		ws::Server::getServers() const {return _servers;}