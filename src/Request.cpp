#include "../inc/Request.hpp"

ws::Request::Request() {_returnStatus = -1;}
ws::Request::Request(const std::string &buffer, ConfigServer *config)
		: _buffer(buffer), _config(config) {
	_returnStatus = -1;
	_locations = _config->getLocation();
	_autoIndexFlag = false;
	readBuffer();
	// std::cout << "header: " << _header << std::endl;
	// std::cout << "body: " << _body << std::endl;
	// std::cout << "method: " << _method << std::endl;
	// std::cout << "target: " << _target << std::endl;
	// std::cout << "protocol: " << _protocolVersion << std::endl;
	// std::cout << "Headerfields:\n";
	// printMapStrings(_headerFields);
	// std::cout << std::endl;
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
		_headerFields = rhs._headerFields;
		_path = rhs._path;
		_response = rhs._response;
		_returnStatus = rhs._returnStatus;
		_locations = rhs._locations;
		_autoIndexFlag = rhs._autoIndexFlag;
		_queryString = rhs._queryString;
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
		_returnStatus = 400;
		return ;
	}
	_header = _buffer.substr(0, crlf);
	// check that there are no empty spaces before method
	if (_header[0] != 'G' && _header[0] != 'P' && _header[0] != 'D') {
		std::cerr << "Invalid method" << std::endl;
		_returnStatus = 405;
		return ;
	}
	_parseStartingLine();
	_parseHeaderFields();
	_body = _buffer.substr(crlf + 4);
	if (this->_body.size() > this->_config->getClientMaxBodySize())
	{
		std::cerr << "Body size is higher than client max body size field\n";
		this->_returnStatus = 413;
		return ;
	}
	// std::cout << "body: " << _body << "|\n";
	// get parameters from the starting line: method, taget and protocol version
}

void	ws::Request::_checkPath() {
	std::map<std::string, ConfigLocation *>::iterator	tmp;
	std::map<std::string, ConfigLocation *>::iterator	it = _locations.begin();
	std::map<std::string, ConfigLocation *>::iterator	itEnd = _locations.end();
	bool	aiFlag = false;
	bool	findLocation = false;

	tmp = _locations.find(_target);
	do {
		_autoIndexFlag = false;
		_returnStatus = -1;
		// searching for a proper location to create path;
		// if target and location name are the same, then go to condition 2
		// in order to have proper autoindex and methods
		// std::cout << "name: " << it->first << std::endl;
		// std::cout << "target: " << _target << std::endl;
		if (!_target.compare("/"))
			_path = it->second->getRoot() + "/" + it->second->getIndex();
		else if (tmp != itEnd && tmp->second->getRoot().size()) {
			it = tmp;
			trimTrailingChar(_target, '/');
			_path = it->second->getRoot();
		}
		else {
			trimTrailingChar(_target, '/');
			_path = it->second->getRoot() + _target;
		}
		aiFlag = it->second->getAutoIndex();
		// std::cout << "flag: " << aiFlag << std::endl;
		// check if directory exists or not; otherwise append ".html"
		if (aiFlag && isDirectory(_path))
			_autoIndexFlag = true;
		else if (ws::checkNoExtension(_path))
			_path += ".html";
		// std::cout << "path: " << _path << std::endl;
		// std::cout << "exist: " << fileExists(_path) << std::endl;
		// if file at _path does not exist, return error 404
		if (ws::fileExists(_path)) {
			findLocation = true;
			if (!_checkMethod(it->second->getMethods())) {
				_returnStatus = 405;
				break ;
			}
		}
		++it;
		if (it == itEnd && !findLocation) {
			findLocation = true;
			_returnStatus = 404;
		}
	} while (findLocation != true);
}

bool	ws::Request::_checkMethod(std::vector<std::string> methods) {
	std::vector<std::string>::iterator	it = methods.begin();
	std::vector<std::string>::iterator	itEnd = methods.end();

	// allow GET methods if in a locations allowed methods are not specified
	if (methods.empty() && !_method.compare("GET"))
		return true;
	for (; it != itEnd; ++it) {
		if (!_method.compare(*it))
			return true;
	}
	return false;
}

// need to check for errors
void	ws::Request::_parseStartingLine() {
	size_t		pos;
	size_t		start;
	std::string file;

	pos = _header.find(" ");
	_method = _header.substr(0, pos);
	if ((_method.compare("GET") && _method.compare("POST")
		&& _method.compare("DELETE")) || !_method.size()) {
		_returnStatus = 405;
		return ;
	}
	start = pos + 1;
	pos = _header.find(" ", start);
	_target = _header.substr(start, pos - start);
	_queryString = _target;
	_parseGetTarget();
	_checkPath();
	start = pos + 1;
	pos = _header.find("\r\n", start);
	_protocolVersion = _header.substr(start, pos - start);
	if ((_protocolVersion.compare("HTTP/1.0") && _protocolVersion.compare("HTTP/1.1"))
		|| !_protocolVersion.size())
		_returnStatus = 400;
}

void	ws::Request::_parseHeaderFields() {
	size_t								pos, pos1, pos2;
	std::pair<std::string, std::string>	pair;

	pos = _header.find("\r\n");
	pos = pos + 2;
	// content is a trim of _header without the first line
	std::string	content = _header.substr(pos);
	// std::cout << "content:\n" << content;
	// std::cout << "\n=============\n";
	pos = 0;
	// puts the lefthand side of ":" into the key of the map and the righthand side into the value of the map
	while (pos != std::string::npos) {
		pos1 = content.find(COLON, pos);
		if (pos1 == std::string::npos) {
			_returnStatus = 400;
			return ;
		}
		if (pos == 0)
			pair.first = content.substr(pos, pos1);
		else
			pair.first = content.substr(pos + 1, pos1 - pos - 1);
		// std::cout << "first: " << pair.first << std::endl;
		pos2 = content.find(SPACE, pos1);
		if (pos2 == std::string::npos) {
			_returnStatus = 400;
			return ;
		}
		pos = content.find(NEWLINE, pos2);
		pair.second = content.substr(pos2 + 1, pos - pos2 - 1);
		// std::cout << "second: " << pair.second << std::endl;
		this->_headerFields.insert(pair);
	}
}

void	ws::Request::_parseGetTarget(void)
{
	int pos2 = _target.find("?");
	if (pos2 != std::string::npos)
	{
		_target.erase(pos2, std::string::npos);
	}
	_queryString.erase(0, pos2 + 1);
}

// Setters
void	ws::Request::setReturnStatus(int code) {_returnStatus = code;}

// Getters
std::string		ws::Request::getBuffer() const			{return _buffer;}
std::string		ws::Request::getHeader() const			{return _header;}
std::string		ws::Request::getBody() const			{return _body;}
std::string		ws::Request::getMethod() const			{return _method;}
std::string		ws::Request::getTarget() const			{return _target;}
std::string		ws::Request::getProtocol() const		{return _protocolVersion;}
std::map<std::string, std::string>	ws::Request::getHeaderFields() const	{return _headerFields;}
ConfigServer*	ws::Request::getConfig() const			{return _config;}
std::string		ws::Request::getPath() const			{return _path;}
std::string		ws::Request::getResponse() const		{return _response;}
int				ws::Request::getReturnStatus() const	{return _returnStatus;}
bool			ws::Request::getAutoIndexFlag() const	{return _autoIndexFlag;}
std::string		ws::Request::getQueryString() const		{return _queryString;}
void			ws::Request::setPath(std::string & path) {this->_path = path;}