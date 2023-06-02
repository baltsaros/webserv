#include "../inc/Request.hpp"

ws::Request::Request() {_errorCode = -1;}
ws::Request::Request(std::string buffer, Configuration config)
		: _buffer(buffer), _config(config) {
	_errorCode = -1;
	readBuffer();
	// std::cout << "header: " << _header << std::endl;
	// std::cout << "body: " << _body << std::endl;
	// std::cout << "method: " << _method << std::endl;
	// std::cout << "target: " << _target << std::endl;
	// std::cout << "protocol: " << _protocolVersion << std::endl;
	// std::cout << "host: " << _host << std::endl;
	// std::cout << "uagent: " << _uAgent << std::endl;
	// std::cout << "accept: " << _accept << std::endl;
	// std::cout << "code: " << _errorCode << std::endl;
}

ws::Request::Request(Request const &src) {
	*this = src;
}

ws::Request::~Request() {}

ws::Request&	ws::Request::operator=(Request const &rhs) {
	if (this != &rhs) {
		_config = rhs._config;
		_buffer = rhs._buffer;
		_header = rhs._header;
		_body = rhs._body;
		_method = rhs._method;
		_target = rhs._target;
		_protocolVersion = rhs._protocolVersion;
		_host = rhs._host;
		_uAgent = rhs._uAgent;
		_accept = rhs._accept;
		_path = rhs._path;
		_response = rhs._response;
		_errorCode = rhs._errorCode;
	}
	return (*this);
}

void	ws::Request::_checkPos(size_t pos) {
	if (pos == std::string::npos)
		std::cerr << "Invalid character position" << std::endl;
}

std::string	ws::Request::_getParam(std::string toGet, size_t offset) {
	size_t		end;
	size_t		start;
	std::string	param;

	start = _buffer.find(toGet);
	end = _buffer.find("\r\n", start);
	param = _buffer.substr(start + offset, end-start-offset);
	return (param);
}

void	ws::Request::readBuffer() {
	size_t	crlf = 0;
	size_t	pos;

	// std::cout << "buffer: \n" << _buffer << std::endl;
	// check until empty line that separates body from header
	crlf = _buffer.find("\r\n\r\n");
	if (crlf == std::string::npos || !_buffer.size()) {
		std::cerr << "Nothing was found" << std::endl;
		_errorCode = 400;
		return ;
	}
	_header = _buffer.substr(0, crlf);
	// check that there are no empty spaces before method
	if (_header[0] != 'G' && _header[0] != 'P' && _header[0] != 'D') {
		std::cerr << "Invalid method" << std::endl;
		_errorCode = 405;
		return ;
	}
	_body = _buffer.substr(crlf + 2);
	// get parameters from the starting line: method, taget and protocol version
	_parseStartingLine();
	
	//check for other parameters
	pos = _buffer.find("Host: ");
	if (pos != std::string::npos)
		_host = _getParam("Host: ", 6);
	pos = _buffer.find("User-Agent: ");
	if (pos != std::string::npos)
		_uAgent = _getParam("User-Agent: ", 12);
	pos = _buffer.find("Accept: ");
	if (pos != std::string::npos)
		_accept = _getParam("Accept: ", 8);
	// std::cout << "header: " << _header << std::endl;
	// std::cout << "body: " << _body << std::endl;
	// std::cout << "method: " << _method << std::endl;
	// std::cout << "target: " << _target << std::endl;
	// std::cout << "protocol: " << _protocolVersion << std::endl;
	// std::cout << "host: " << _host << std::endl;
	// std::cout << "uagent: " << _uAgent << std::endl;
	// std::cout << "accept: " << _accept << std::endl;
}

// need to check for errors
void	ws::Request::_parseStartingLine() {
	size_t		pos;
	size_t		start;
	std::string file;
	bool		cssFlag = false;
	
	pos = _header.find(" ");
	_method = _header.substr(0, pos);
	if ((_method.compare("GET") && _method.compare("POST")
		&& _method.compare("DELETE")) || !_method.size()) {
		_errorCode = 405;
		return ;
	}
	start = pos + 1;
	pos = _header.find(" ", start);
	_target = _header.substr(start, pos - start);
	// need to check every location for the config here
	// std::cout << "target: " << _target << std::endl;
	cssFlag = checkCssExtension(_target);
	if (cssFlag)
		_path = "website" + _target;
	else
		_path = "website/html/index.html";
	// std::cout << "path: " << _path << std::endl;
	if (_target.compare("/") && _target.compare("/index.html") && !cssFlag) {
		_errorCode = 404;
		return ;
	}
	start = pos + 1;
	pos = _header.find("\r\n", start);
	_protocolVersion = _header.substr(start, pos - start);
}

// Getters
std::string	ws::Request::getMethod() const			{return _method;}
std::string	ws::Request::getTarget() const			{return _target;}
std::string	ws::Request::getProtocol() const		{return _protocolVersion;}
std::string	ws::Request::getHost() const			{return _host;}
std::string	ws::Request::getUAgent() const			{return _uAgent;}
std::string	ws::Request::getAccept() const			{return _accept;}
std::string	ws::Request::getPath() const			{return _path;}
std::string	ws::Request::getResponse() const		{return _response;}
int			ws::Request::getErrorCode() const		{return _errorCode;}