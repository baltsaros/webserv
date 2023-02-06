#pragma once

# include <sys/socket.h> // for functions like socket, bind, listen, accept and others
# include <netinet/in.h> // some macros for socket
# include <netdb.h> // getaddrinfo
# include <string.h> // strerror
# include <iostream>

namespace	ws {

class Socket {
	private:
		int					_sockfd;
		int					_backlog;
		struct sockaddr_in	_address;

	public:
		Socket() {}

		Socket(int domain, int service, int protocol, int port,
				u_long interface, int backlog);

		void				connect_to_network();
		void				test_connection(int to_test);
		struct sockaddr_in	get_address();
		int					get_socket();
};

}