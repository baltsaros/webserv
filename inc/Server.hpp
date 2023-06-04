#pragma once

# include <unistd.h>
# include <string>
# include <sys/time.h> // struct timeval
# include <fstream>
# include <sstream>
# include "Socket.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Configuration.hpp"
# include "ServerUtils.hpp"

# define BUFFER_SIZE 4096 // should be more than 4

namespace	ws {
class Server {
	private:
		struct timeval	_timeout;
		Socket			*_socket;
		Request			_req;
		Configuration	_config;
		int				_sockfd, _max_sd;
		std::string		_buf;
		fd_set			_working_set, _master_set;
		int				accepter();
		int				handler(int i);
		int				responder(int i);

	public:
		Server();
		Server(int domain, int service, int protocol,
				int port, u_long interface, int backlog,
				Configuration config);
		Server(const Server &src);
		~Server();

		Server&	operator=(const Server &rhs);
		void	launcher();
		void	test_connection(int to_test);
		Socket*	get_server_sd();
};
}