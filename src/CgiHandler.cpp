#include "../inc/CgiHandler.hpp"

CgiHandler::CgiHandler()
{
	this->_req = NULL;
}

CgiHandler::~CgiHandler()
{
}

CgiHandler::CgiHandler(Request * req)
{
	this->_req = req;
	CgiHandler::initializeEnv();
}

CgiHandler::CgiHandler(CgiHandler const & src)
{
	
}

CgiHandler & CgiHandler::operator=(CgiHandler const & rhs)
{
	if (this != &rhs)
	{

	}
}

Request *	CgiHandler::getRequest(void) const
{
	return (this->_req);
}

std::string	CgiHandler::getExecutable(void) const
{
	return (this->_executable);
}

std::map<std::string, std::string>	CgiHandler::getEnvVariables(void) const
{
	return (this->_envVariables);
}

void	CgiHandler::initializeEnv(void)
{

}

void	CgiHandler::execute(void)
{
	int	pid;

	pid = fork();
	if (pid == -1) //Error fork
	{
		std::cout << "Fork error\n";
		return ;
	}
	if (pid == 0) //Child process
	{
		//Need to execve with program, argv and env

	} //IF---CHILD
	else //Parent process
	{}
}