#include "../inc/Request.hpp"

ws::Request::Request() {_returnStatus = -1;}
ws::Request::Request(const std::string &buffer, ConfigServer *config)
		: _buffer(buffer), _config(config) {
	_returnStatus = -1;
	_locations = _config->getLocation();
	_autoIndexFlag = false;
	readBuffer();
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
	size_t		crlf = 0;
	size_t		pos;
	std::string	host;

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
	if (this->_headerFields.count("Transfer-Encoding") && 
		this->_headerFields["Transfer-Encoding"] == "chunked\r")
	{
		int contentLength = chunkRequest();
		this->_headerFields[CONTENT_LENGTH_FIELD] = contentLength;
	}
	host = ws::substrUntil(_headerFields["Host"], ':');
	if (!host.size() || host.compare(_config->getHost())) {
		_returnStatus = 400;
		return ;
	}
	if (this->_returnStatus < 0 && this->_method == "POST"
		&& this->_headerFields.count(CONTENT_LENGTH_FIELD) == 0
		&& this->_headerFields.count("Transfer-Encoding") == 0)
	{
		std::cerr << "Content length is missing with request POST\n";
		this->_returnStatus = 411;
		return ;
	}
	if (this->_body.size() > this->_config->getClientMaxBodySize() &&
		this->_returnStatus < 0)
	{
		std::cerr << "Body size is higher than client max body size field\n";
		this->_returnStatus = 413;
		return ;
	}
	if (this->_returnStatus < 0 && this->_method == "POST"
		&& this->_headerFields.count("Transfer-Encoding") == 0
		&& this->_headerFields.count(CONTENT_LENGTH_FIELD)
		&& this->_body.size() != atof(this->_headerFields[CONTENT_LENGTH_FIELD].c_str()))
		{
			std::cerr << "Body size doesn't match content length\n";
			this->_returnStatus = 501;
			return ;
		}
	// get parameters from the starting line: method, taget and protocol version
}

void	ws::Request::_checkPath() {

	std::map<std::string, ConfigLocation *>::iterator	tmp;
	std::string	target;

	tmp = _findLocation();
	target = _target;
	_autoIndexFlag = false;
	_returnStatus = -1;
	if (!_checkMethod(tmp->second->getMethods())) {
		_returnStatus = 405;
		return ;
	}
	if (tmp->first.compare(SLASH)) {
		target.erase(0, tmp->first.size());
	}
	_path = tmp->second->getRoot() + target;
	ws::trimTrailingChar(_path, '/');
	if (!isDirectory(_path)) {
		if (ws::checkNoExtension(_path))
			_path += ".html";
		if (!ws::fileExists(_path))
			_returnStatus = 404;
	}
	else {
		if (tmp->second->getAutoIndex())
			_autoIndexFlag = true;
		else
			_path += SLASH + tmp->second->getIndex();
	}
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
	if (checkExtension(_target, ".css") || checkExtension(_target, ".ico")) {
		_path = "website" + _target;
		_returnStatus = -1;
	}
	else
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
		pos2 = content.find(SPACE, pos1);
		if (pos2 == std::string::npos) {
			_returnStatus = 400;
			return ;
		}
		pos = content.find(NEWLINE, pos2);
		pair.second = content.substr(pos2 + 1, pos - pos2 - 1);
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

std::map<std::string, ConfigLocation *>::iterator	ws::Request::_findLocation() {

	std::string	target;
	int			pos = 42;
	
	target = _target;
	while (pos != 0 && _locations[target] == NULL) {
		pos = target.find_last_of('/');
		target = target.substr(0, pos);
	}
	if (pos == 0)
		target = "/";
	return _locations.find(target);
}

int	ws::Request::chunkRequest(void)
{
	std::string	newBody = this->_body;
	std::string	bodyToRet;
	int			contentLength = 0;
	int			tempBody;
	int			tempLength;
	std::stringstream stream;


	while ((tempBody = newBody.find("\r\n")))
	{
		if (atoi(newBody.substr(0, tempBody).c_str()) == 0)
			break;
		stream << newBody.substr(0, tempBody).c_str();
		stream >> std::hex >> tempLength;
		contentLength += tempLength;
		newBody.erase(0, tempBody + 2);
		tempBody = newBody.find("\r\n");
		bodyToRet.append(newBody.substr(0, tempBody));
		bodyToRet.append("\n");
		contentLength += 1;
		newBody.erase(0, tempBody + 2);
	}
	//delete the last \n of the body we don't need
	bodyToRet.erase(bodyToRet.size() - 1, bodyToRet.size());
	this->_body = bodyToRet;
	return (contentLength);
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