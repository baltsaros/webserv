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
		this->_envVariables = rhs.getEnvVariables();
		this->_executable = rhs.getExecutable();
		this->_req = rhs.getRequest();
		this->_socketFd = rhs.getSocketFd();
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

int	ws::CgiHandler::getSocketFd(void) const
{
	return (this->_socketFd);
}

void	ws::CgiHandler::initializeEnv(void)
{
	this->_envVariables[CONTENT_TYPE] = CONTENT_TYPE + (std::string)"=application/x-www-form-urlencoded";
	this->_envVariables[CONTENT_LENGTH] = CONTENT_LENGTH + (std::string)"=" + std::to_string(this->_req.getBody().size());
	this->_envVariables[HTTP_USER_AGENT] = HTTP_USER_AGENT + (std::string)"=" + this->_req.getUAgent();
	this->_envVariables[PATH_INFO] = PATH_INFO + (std::string)"=" + this->_req.getTarget();
	this->_envVariables[REQUEST_METHOD] = REQUEST_METHOD + (std::string)"=" + this->_req.getMethod();
	this->_envVariables[SCRIPT_FILENAME] = SCRIPT_FILENAME + (std::string)"=" + this->_req.getTarget();
	this->_envVariables[SCRIPT_NAME] = SCRIPT_NAME + (std::string)"=" + this->_req.getTarget(); 
	this->_envVariables[SERVER_PROTOCOL] = SERVER_PROTOCOL + (std::string)"="+ this->_req.getProtocol();

}

void	ws::CgiHandler::execute(void)
{
	//this->_envVariables[CONTENT_TYPE] = "application/x-www-form-urlencoded";
	char *env[] = {&this->_envVariables[CONTENT_TYPE][0],
					&this->_envVariables[CONTENT_LENGTH][0],
					&this->_envVariables[HTTP_USER_AGENT][0],
					&this->_envVariables[PATH_INFO][0],
					&this->_envVariables[REQUEST_METHOD][0],
					&this->_envVariables[SCRIPT_FILENAME][0],
					&this->_envVariables[SCRIPT_NAME][0],
					NULL};
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

		while((ret = read(pipe_out[0], buf, BUFFER_LENGTH)))
		{
			toRet = buf;
			send(this->_socketFd, toRet.c_str(), toRet.size(), 0);
		}
		close(pipe_out[0]);
		waitpid(pid, NULL, 0);
	}
}