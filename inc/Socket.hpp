#pragma once

# include <sys/socket.h> // for functions like socket, bind, listen, accept and others
# include <netinet/in.h> // some macros for socket
# include <netdb.h> // getaddrinfo
# include <string.h> // strerror
# include <vector>
# include <fcntl.h>
# include <iostream>
# include <unistd.h>

namespace	ws {

class Socket {
	private:
		std::vector<int>	_sockfds;
		int					_max_sd;
		int					_backlog;
		int					_generation;
		struct sockaddr_in	_address;
		// fd_set				_master_set;

	public:
		Socket();
		Socket(int domain, int service, int protocol, std::vector<int> port,
				u_long interface, int backlog, int generation);
		Socket(const Socket &src);
		~Socket();

		Socket&				operator=(const Socket &rhs);
		void				connect_to_network(int sockfd);
		void				test_connection(int to_test, int socket);

		// setters
		void				setGeneration(int generation);

		// getters
		struct sockaddr_in	getAddress() const;
		std::vector<int>	getSockets() const;
		int					getMaxsd() const;
		int					getGeneration() const;
		// fd_set				get_masterset() const;
};

}