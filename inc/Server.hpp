#pragma once

# include <unistd.h>
# include <string>
# include <sys/time.h> // struct timeval
# include <fstream>
# include <sstream>
# include <map>
# include "Socket.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Configuration.hpp"
# include "ServerUtils.hpp"

# define BUFFER_SIZE 4096 // should be more than 4

namespace	ws {
class Server {
	private:
		struct timeval		_timeout;
		std::map<int, Socket*>	_socket;
		std::vector<int>	_sockfds;
		Request				_req;
		Configuration		_config;
		int					_max_sd;
		std::string			_buf;
		fd_set				_working_set, _master_set;
		std::map<int, ConfigServer*>	_socketServer;
		std::vector<ConfigServer*>	_servers;
		int					accepter(int sockfd);
		int					handler(int i);
		int					responder(int i);
		bool				checkSocket(int i);

	public:
		Server();
		Server(int domain, int service, int protocol,
				u_long interface, int backlog,
				Configuration config);
		Server(const Server &src);
		~Server();

		Server&	operator=(const Server &rhs);
		void	launcher();
		void	test_connection(int to_test);
		std::map<int, ws::Socket*>			get_server_sd();
		std::map<int, ConfigServer*>	getSocketServer() const;
		std::vector<ConfigServer*>		getServers() const;
};
}