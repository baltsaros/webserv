#include "../inc/Response.hpp"

ws::Response::Response() {}

ws::Response::Response(const Request &req, const Configuration &config) :
		_req(req), _config(config) {createResponse();}

ws::Response::Response(const Response &src) {*this = src;}

ws::Response::~Response() {}

ws::Response&	ws::Response::operator=(const Response &rhs) {
	if (this != &rhs) {
		_msg = rhs._msg;
		_response = rhs._response;
		_ret = rhs._ret;
		_config = rhs._config;
		_req = rhs._req;
	}
	return *this;
}

void	ws::Response::createResponse() {
	if (_req.getErrorCode() >= 0)
		checkRequest();
	else {
		if (checkExtension(_req.getTarget(), ".css"))
			_response = "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\nContent-Length: ";
		else if (checkExtension(_req.getTarget(), ".ico"))
			_response = "HTTP/1.1 200 OK\r\nContent-Type: image/x-icon\r\nContent-Length: ";
		else
			_response = "HTTP/1.1 200 OK\r\nContent-Type: text\r\nContent-Length: ";
		_msg = read_file(_req.getPath());
		_response += std::to_string(_msg.size());
		_response += "\r\n\r\n";
		_response += _msg;
	}
}

// check for an error code and create an according response
void	ws::Response::checkRequest() {
	int	errorCode = _req.getErrorCode();
	std::map<int, std::string>	errorPath = _config.getConfigServer()[0]->getErrorPages();
	_msg = read_file(errorPath[errorCode]);
	switch (errorCode) {
		case 400:
			_response = "HTTP/1.1 400 Bad Request\r\n";
			break;
		case 403:
			_response = "HTTP/1.1 403 Forbidden\r\n";
			break;
		case 404:
			_response = "HTTP/1.1 404 Not Found\r\n";
			break;
		case 405:
			_response = "HTTP/1.1 405 Method Not Allowed\r\n";
			break;
		default:
			break;
	}
	// if (errorPath[errorCode].size() == 0) {
	if (_msg.size() == 0) {
		_msg = read_file("website/html/error_pages/default_error.html");
		_response = "HTTP/1.1 400 Bad Request\r\n";
	}
	_response += "Content-Type: text/html\r\n";
	_response += "Content-Length: " + std::to_string(_msg.length());
	_response += "\r\n\r\n";
	_response += _msg;
}


// getters
std::string	ws::Response::getMsg()		const {return _msg;}
std::string	ws::Response::getResponse()	const {return _response;}
int			ws::Response::getRet()		const {return _ret;}
ws::Request	ws::Response::getRequest()	const {return _req;}