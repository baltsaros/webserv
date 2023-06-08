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

int			ws::CgiHandler::getSocketFd(void) const
{
	return (this->_socketFd);
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
	char *env[] = {&plop[0], NULL};

	if (pipe(this->_pipe_in) < 0)
		return ;
	if (pipe(this->_pipe_out) < 0)
		return ;
		std::cout << this->_req.getBody() << "\n";
	this->_pid = fork();
	if (this->_pid == 0) //child
	{
		close(this->_pipe_in[1]);
		close(this->_pipe_out[0]);
		//std::cout << this->_req.getTarget() << "\n";
		dup2(this->_pipe_in[0], STDIN_FILENO);
		close(this->_pipe_in[0]);
		dup2(this->_pipe_out[1], STDOUT_FILENO);
		close(this->_pipe_out[1]);
		execve("src/cgi-bin/calculator.py", NULL, env);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
	{
		close(this->_pipe_in[0]);
		close(this->_pipe_out[1]);
		std::string body = this->_req.getBody();
		write(this->_pipe_in[1], body.c_str(), body.size());
		close(this->_pipe_in[1]);
		char buf[BUFFER_LENGTH];
		int	ret;
		//std::cout << "HAHAHAH\n";
		while((ret = read(this->_pipe_out[0], buf, BUFFER_LENGTH)))
		{
			write(this->_socketFd, buf, ret);
			//std::cout << "plop======" << buf << "\n";
		}
		close(this->_pipe_out[0]);
		//std::cout << "fourt\n";
		waitpid(this->_pid, NULL, 0);
		//write(this->_pipe_in[1], body.c_str(), body.size());
	}
} 