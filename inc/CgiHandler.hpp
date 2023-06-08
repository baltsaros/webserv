#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include "Request.hpp"

# include <unistd.h>
# include <iostream>
# include <map>

# define CONTENT_TYPE "CONTENT_TYPE"
# define CONTENT_LENGTH "CONTENT_LENGTH"
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
		int									_pid;
		
		void initializeEnv(void);

	public:
		int									_pipe_in[2];
		int									_pipe_out[2];
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