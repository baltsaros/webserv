#include "../inc/CgiHandler.hpp"

//Destructor
ws::CgiHandler::~CgiHandler()
{
	this->_envVariables.clear();
}

//Constructor
ws::CgiHandler::CgiHandler(ws::Request req, int socketFd)
{
	this->_req = req;
	this->_socketFd = socketFd;
	this->_executable = this->_req.getPath();
	CgiHandler::initializeEnv();
}

//Copy constructor
ws::CgiHandler::CgiHandler(CgiHandler const & src)
{
	*this = src;
}

//Override = operator
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

/*
** Getter fot the request field.
** @return : request : request 
*/
ws::Request	 ws::CgiHandler::getRequest(void) const
{
	return (this->_req);
}

/*
** Getter for the executable fields.
** @return : executable : std::string 
*/
std::string	ws::CgiHandler::getExecutable(void) const
{
	return (this->_executable);
}

/*
** Getter for the environement variables fields.
** @return : envVariables : std::map<std::string, std::string> 
*/
std::map<std::string, std::string>	ws::CgiHandler::getEnvVariables(void) const
{
	return (this->_envVariables);
}

/*
** Getter for the socketFd fields.
** @return : socketFd : int 
*/
int	ws::CgiHandler::getSocketFd(void) const
{
	return (this->_socketFd);
}

/*
** Create the association between a field and it's value for the CGI environment
**	variables.
** @param :	std::string leftString the name of the variable
**			std::string 
*/
std::string	ws::CgiHandler::createEnvString(std::string leftString, std::string righString)
{
	return (leftString + "=" + righString);
}
/*
** Initialise all the environment variables for the cgi.
*/
void	ws::CgiHandler::initializeEnv(void)
{
	this->_envVariables[CONTENT_TYPE] = createEnvString(CONTENT_TYPE, this->_req.getHeaderFields()[CONTENT_TYPE_FIELD]);
	this->_envVariables[CONTENT_LENGTH] = createEnvString(CONTENT_LENGTH, std::to_string(this->_req.getBody().size()));
	this->_envVariables[HTTP_USER_AGENT] = createEnvString(HTTP_USER_AGENT, this->_req.getHeaderFields()[USER_AGENT_FIELD]);
	this->_envVariables[PATH_INFO] = createEnvString(PATH_INFO, this->_req.getTarget());
	this->_envVariables[REQUEST_METHOD] = createEnvString(REQUEST_METHOD, this->_req.getMethod());
	this->_envVariables[SCRIPT_FILENAME] = createEnvString(SCRIPT_FILENAME, this->_req.getTarget());
	this->_envVariables[SCRIPT_NAME] = createEnvString(SCRIPT_NAME, this->_req.getTarget());
	this->_envVariables[SERVER_PROTOCOL] = createEnvString(SERVER_PROTOCOL, this->_req.getProtocol());
	this->_envVariables[QUERY_STRING] = createEnvString(QUERY_STRING, this->_req.getQueryString());
}

/*
** Execute the cgi script.
** The function is divide in two processes with a fork.
** The child process is used to execute the cgi script with execve and send
**	back the response to the parent process through pipes.
** The parent function is used to send the data to the child process and get 
**	the response back to write it through the socket fd.
*/
int	ws::CgiHandler::execute(void)
{
	char *env[] = {&this->_envVariables[CONTENT_TYPE][0],
					&this->_envVariables[CONTENT_LENGTH][0],
					&this->_envVariables[HTTP_USER_AGENT][0],
					&this->_envVariables[PATH_INFO][0],
					&this->_envVariables[REQUEST_METHOD][0],
					&this->_envVariables[SCRIPT_FILENAME][0],
					&this->_envVariables[SCRIPT_NAME][0],
					&this->_envVariables[SERVER_PROTOCOL][0],
					&this->_envVariables[QUERY_STRING][0],
					NULL};
	int pipe_in[2];
	int pipe_out[2];
	int	status = 0;

	//Erase the first charactrer which is '/'. If not, execve will failed.
	//std::string newTarget = this->_req.getTarget().erase(0, 1);

	if (pipe(pipe_in) < 0)
		return -1;
	if (pipe(pipe_out) < 0)
		return -1;
	int pid = fork();
	if (pid == 0) //child
	{
		close(pipe_in[1]);
		close(pipe_out[0]);
		dup2(pipe_in[0], STDIN_FILENO);
		close(pipe_in[0]);

		dup2(pipe_out[1], STDOUT_FILENO);
		close(pipe_out[1]);
		execve(this->_executable.c_str(), NULL, env);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
	{
		int retPid;
		int ret;
		close(pipe_in[0]);
		close(pipe_out[1]);
		std::string body = this->_req.getBody();
		status = write(pipe_in[1], body.c_str(), body.size());
		if (status < 0)
			return -1;
		close(pipe_in[1]);

		char buf[BUFFER_LENGTH];
		std::string toRet;

		while((ret = read(pipe_out[0], buf, BUFFER_LENGTH)))
		{
			if (ret < 0)
				return -1;
			toRet = buf;
			status = send(this->_socketFd, toRet.c_str(), toRet.size(), 0);
		}
		close(pipe_out[0]);
		waitpid(pid, &retPid, 0);
		if (WIFEXITED(retPid) ) {
			int es = WEXITSTATUS(retPid);
			this->_req.setReturnStatus(500);
			Response rep = Response(_req);
			std::string toSend = rep.getResponse();
			status = send(this->_socketFd, toSend.c_str(), toSend.size(), 0);
		}
	}
	return status;
}