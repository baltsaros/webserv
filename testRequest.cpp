#include "inc/Request.hpp"

int	main(void) {
	// std::string	buffer = "GET /index.html HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: Mozilla\r\nAccept: text\r\n\r\n";
	std::string	buffer = "GET /index.html HTTP/1.1\r\nHost: www.example.com\r\n\r\n";
	ws::Request req(buffer);
	req.readBuffer();
	return (0);
}