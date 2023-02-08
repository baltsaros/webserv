#pragma once

# include <unistd.h>
# include <string>
# include <sys/time.h> // struct timeval
# include "Socket.hpp"

namespace	ws {
class Server {
	private:
		struct timeval	_timeout;
		Socket	*_socket;
		int		_sockfd, _max_sd;
		char	_buf[30000];
		fd_set	_working_set, _master_set;
		void	accepter();
		void	handler();
		void	responder();

	public:
		Server(int domain, int service, int protocol,
				int port, u_long interface, int backlog);
		~Server();

		void	launcher();
		void	test_connection(int to_test);
		Socket*	get_server_sd();
};
}