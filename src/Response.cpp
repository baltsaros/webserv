#include "../inc/Response.hpp"

ws::Response::Response() {}

// Constructor takes a request as its parameter
ws::Response::Response(const Request &req) :
		_req(req) {createResponse();}

ws::Response::Response(const Response &src) {*this = src;}

ws::Response::~Response() {}

ws::Response&	ws::Response::operator=(const Response &rhs) {
	if (this != &rhs) {
		_msg = rhs._msg;
		_response = rhs._response;
		_ret = rhs._ret;
		_req = rhs._req;
	}
	return *this;
}

// Check error status and create a proper response message, including header and body
void	ws::Response::createResponse() {
	if (_req.getReturnStatus() >= 0)
		checkRequest();
	else {
		if (ws::checkExtension(_req.getTarget(), ".css"))
			_response = "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\nContent-Length: ";
		else if (ws::checkExtension(_req.getTarget(), ".ico"))
			_response = "HTTP/1.1 200 OK\r\nContent-Type: image/x-icon\r\nContent-Length: ";
		else
			_response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: ";
		// now check autoindex flag; if error appears while opening a directory
		// return 403 error code; might need to change this later;
		if (_req.getAutoIndexFlag()) {
			_msg = createAutoIndex();
			if (!_msg.size()) {
				_req.setReturnStatus(403);
				checkRequest();
			}
		}
		else
			_msg = ws::readFile(_req.getPath());
		_response += std::to_string(_msg.size());
		_response += "\r\n\r\n";
		_response += _msg;
	}
}

// Check for an error code and create an according error response
void	ws::Response::checkRequest() {
	int	returnStatus = _req.getReturnStatus();
	std::map<int, std::string>	errorPath = _req.getConfig()->getErrorPages();
	_msg = ws::readFile(errorPath[returnStatus]);
	switch (returnStatus) {
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
		case 411:
			_response = "HTTP/1.1 411 Length Required\r\n";
			break;
		case 413:
			_response = "HTTP/1.1 413 Content Too Large\r\n";
			break;
		case 500:
			_response = "HTTP/1.1 500 Internal Server Error\r\n";
			break;
		case 501:
			_response = "HTTP/1.1 501 Not Implemented\r\n";
			break;
		default:
			break;
	}
	if (_msg.size() == 0) {
		_msg = ws::readFile("website/html/error_pages/default_error.html");
		_response = "HTTP/1.1 400 Bad Request\r\n";
	}
	_response += "Content-Type: text/html\r\n";
	_response += "Content-Length: " + std::to_string(_msg.length());
	_response += "\r\n\r\n";
	_response += _msg;
}

// Create a response for autoindex page with hyperlinks
std::string	ws::Response::createAutoIndex() {
	std::string			path = _req.getPath();
	std::string			target = _req.getTarget();
	std::stringstream	buf;
	DIR					*dir;
	struct dirent		*entry;

	buf << "<!DOCTYPE html>\n";
	buf << "<html lang=\"en\">\n";
	buf << "<head><title>AutoIndex for " << path << "</title></head>\n";
	buf << "<body>\n";
	buf << "<ul>\n";
	dir = opendir(path.c_str());
	if (!dir) {
		std::cerr << "Could not open the directory" << std::endl;
		return "";
	}
	else {
		entry = readdir(dir);
		while (entry) {
			if (!strcmp(".", entry->d_name) || !strcmp("..", entry->d_name)) {
				entry = readdir(dir);
				continue ;
			}
			if (!fileExists(path + target + entry->d_name)
				&& target[target.size() - 1] != '/')
				buf << "<li><a href=\"" << target + "/" + entry->d_name << "\">";
			else
				buf << "<li><a href=\"" << target + entry->d_name << "\">";
			buf << entry->d_name << "</a></li>\n";
			entry = readdir(dir);
		}
	}
	closedir(dir);
	buf << "</ul>\n";
	buf << "</body>\n";
	buf << "</html>\n";
	return buf.str();
}

// getters
std::string	ws::Response::getMsg()		const {return _msg;}
std::string	ws::Response::getResponse()	const {return _response;}
int			ws::Response::getRet()		const {return _ret;}
ws::Request	ws::Response::getRequest()	const {return _req;}