#pragma once

# include <sys/socket.h> // for functions like socket, bind, listen, accept and others
# include <netinet/in.h> // some macros for socket
# include <netdb.h> // getaddrinfo
# include <string.h> // strerror
# include <fcntl.h>
# include <iostream>

namespace	ws {

class Socket {
	private:
		int					_sockfd, _max_sd;
		int					_backlog;
		struct sockaddr_in	_address;
		// fd_set				_master_set;

	public:
		Socket();
		Socket(int domain, int service, int protocol, int port,
				u_long interface, int backlog);
		Socket(const Socket &src);
		~Socket();

		Socket&				operator=(const Socket &rhs);
		void				connect_to_network();
		void				test_connection(int to_test);
		struct sockaddr_in	get_address() const;
		int					get_socket() const;
		int					get_maxsd() const;
		// fd_set				get_masterset() const;
};

}