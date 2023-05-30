#pragma once

# include <unistd.h>
# include <string>
# include <sys/time.h> // struct timeval
# include <fstream>
# include <sstream>
# include "Socket.hpp"
# include "Request.hpp"

namespace	ws {
class Server {
	private:
		struct timeval	_timeout;
		Socket		*_socket;
		Request		_req;
		int			_sockfd, _max_sd;
		char		_buf[30000];
		fd_set		_working_set, _master_set;
		int			accepter();
		int			handler(int i);
		int			responder(int i);
		std::string	read_file(const std::string& filename);
		int			checkRequest(int i);

	public:
		Server();
		Server(int domain, int service, int protocol,
				int port, u_long interface, int backlog);
		Server(Server const &src);
		~Server();

		Server&	operator=(Server const &rhs);
		void	launcher();
		void	test_connection(int to_test);
		Socket*	get_server_sd();
};
}