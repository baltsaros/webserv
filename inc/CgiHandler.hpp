#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include "Request.hpp"

# include <unistd.h>
# include <iostream>
# include <map>
# include <sys/socket.h>

# define CONTENT_TYPE "CONTENT_TYPE"
# define CONTENT_LENGTH "CONTENT_LENGTH"
# define REQUEST_METHOD "REQUEST_METHOD"
# define SERVER_PROTOCOL "SERVER_PROTOCOL"
# define HTTP_USER_AGENT "HTTP_USER_AGENT"
# define PATH_INFO "PATH_INFO"
# define SCRIPT_FILENAME "SCRIPT_FILENAME"
# define SCRIPT_NAME "SCRIPT_NAME"

# define BUFFER_LENGTH 1024
class Request;

namespace ws {


class CgiHandler
{
	private:
		Request								_req;
		std::string							_executable;
		std::map<std::string, std::string>	_envVariables;
		int									_socketFd;

		void initializeEnv(void);

	public:
		CgiHandler();
		CgiHandler(Request req, int socketFd);
		~CgiHandler();
		CgiHandler(CgiHandler const & src);

		CgiHandler & operator=(CgiHandler const & rhs);

		Request								getRequest(void) const;
		std::string							getExecutable(void) const;
		std::map<std::string, std::string>	getEnvVariables(void) const;
		int									getSocketFd(void) const;

		void	execute(void);
};
}
#endif