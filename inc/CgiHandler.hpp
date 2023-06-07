#ifndef CGIHANDLER_HPP
# define CHIHANDLER_HPP

# include "Request.hpp"

# include <unistd.h>
# include <iostream>
# include <map>

class Request;

class CgiHandler
{
	private:
		Request *							_req;
		std::string							_executable;
		std::map<std::string, std::string>	_envVariables;
		
		void initializeEnv(void);

	public:
		CgiHandler();
		CgiHandler(Request * req);
		~CgiHandler();
		CgiHandler(CgiHandler const & src);

		CgiHandler & operator=(CgiHandler const & rhs);
		Request *	getRequest(void) const;
		std::string	getExecutable(void) const;
		std::map<std::string, std::string> getEnvVariables(void) const;

		void	execute(void);
};

#endif