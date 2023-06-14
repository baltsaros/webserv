#include "../inc/Response.hpp"

ws::Response::Response() {}

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

// check for an error code and create an according response
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
		case 413:
			_response = "HTTP/1.1 413 Content Too Large\r\n";
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

std::string	ws::Response::createAutoIndex() {
	std::string			path = _req.getPath();
	std::string			target = _req.getTarget();
	std::stringstream	buf;
	DIR					*dir;
	struct dirent		*entry;

	// std::cout << "target in response: " << target << std::endl;
	// std::cout << "path in response: " << path << std::endl;
	buf << "<!DOCTYPE html>\n";
	buf << "<html lang=\"en\">\n";
	buf << "<head><title>AutoIndex for " << path << "</title></head>\n";
	buf << "<body>\n";
	buf << "<ul>\n";
	// buf << "<h1>\n";
	dir = opendir(path.c_str());
	if (!dir) {
		std::cerr << "Could not open the directory" << std::endl;
		return "";
	}
	else {
		entry = readdir(dir);
		while (entry) {
			buf << "<li><a href=\"" << target + "/" + entry->d_name << "\">";
			buf << entry->d_name << "</a></li>\n";
			entry = readdir(dir);
		}
	}
	closedir(dir);
	// buf << "</h1>\n";
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