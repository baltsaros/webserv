#pragma once

# include <sys/socket.h> // for functions like socket, bind, listen, accept and others
# include <netinet/in.h> // some macros for socket
# include <netdb.h> // getaddrinfo
# include <string.h> // strerror
# include <vector>
# include <fcntl.h>
# include <iostream>

namespace	ws {

class Socket {
	private:
		std::vector<int>	_sockfds;
		int					_max_sd;
		int					_backlog;
		struct sockaddr_in	_address;
		// fd_set				_master_set;

	public:
		Socket();
		Socket(int domain, int service, int protocol, std::vector<int> port,
				u_long interface, int backlog);
		Socket(const Socket &src);
		~Socket();

		Socket&				operator=(const Socket &rhs);
		void				connect_to_network(int sockfd);
		void				test_connection(int to_test);
		struct sockaddr_in	get_address() const;
		std::vector<int>	get_sockets() const;
		int					get_maxsd() const;
		// fd_set				get_masterset() const;
};

}