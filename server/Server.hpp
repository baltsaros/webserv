#pragma once

# include <unistd.h>
# include <string>
# include "Socket.hpp"

namespace	ws {
class Server {
	private:
		Socket	*_socket;
		int		_sockfd;
		char	_buf[30000] = {0};
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