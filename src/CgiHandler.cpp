#include "../inc/CgiHandler.hpp"

ws::CgiHandler::CgiHandler()
{
}

ws::CgiHandler::~CgiHandler()
{
}

ws::CgiHandler::CgiHandler(ws::Request req, int socketFd)
{
	this->_req = req;
	this->_socketFd = socketFd;
	CgiHandler::initializeEnv();
}

ws::CgiHandler::CgiHandler(CgiHandler const & src)
{
	
}

ws::CgiHandler & ws::CgiHandler::operator=(CgiHandler const & rhs)
{
	if (this != &rhs)
	{

	}
	return (*this);
}

ws::Request	 ws::CgiHandler::getRequest(void) const
{
	return (this->_req);
}

std::string	ws::CgiHandler::getExecutable(void) const
{
	return (this->_executable);
}

std::map<std::string, std::string>	ws::CgiHandler::getEnvVariables(void) const
{
	return (this->_envVariables);
}

void	ws::CgiHandler::initializeEnv(void)
{
}

void	ws::CgiHandler::execute(void)
{
	this->_envVariables[CONTENT_TYPE] = "application/x-www-form-urlencoded";
	std::string plop = "CONTENT_TYPE=application/x-www-form-urlencoded";
	std::string method = "REQUEST_METHOD=POST";
	//std::string length = "CONTENT_LENGTH=27";
	char *env[] = {&plop[0], &method[0], NULL};
	int pipe_in[2];
	int pipe_out[2];

	if (pipe(pipe_in) < 0)
		return ;
	if (pipe(pipe_out) < 0)
		return ;
	int pid = fork();
	if (pid == 0) //child
	{
		close(pipe_in[1]);
		close(pipe_out[0]);
		dup2(pipe_in[0], STDIN_FILENO);
		close(pipe_in[0]);

		dup2(pipe_out[1], STDOUT_FILENO);
		close(pipe_out[1]);
		execve("src/cgi-bin/calculator.py", NULL, env);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
	{
		int ret;
		close(pipe_in[0]);
		close(pipe_out[1]);
		std::string body = this->_req.getBody();
		write(pipe_in[1], body.c_str(), body.size());
		close(pipe_in[1]);

		char buf[BUFFER_LENGTH];
		std::string toRet;

		while((ret = read(pipe_out[0], buf, BUFFER_LENGTH - 1)))
		{
			toRet = buf;
			send(this->_socketFd, toRet.c_str(), toRet.size(), 0);
		}
		close(pipe_out[0]);
		waitpid(pid, NULL, 0);
	}
}