#include "Client.hpp"

// Hardcoded info, to be delevoped
Client::Client()
	: _host(host), _port(port), _request(request)
{}

void Client::ConnectAndSendRequest()
{
	int sockfd = 0, n = 0;
	struct addrinfo hints, *servinfo, *p;

	char buffer[4096];

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	int rv = getaddrinfo(_host.c_str(), _port.c_str(), &hints, &servinfo);
	if (rv != 0) {
		std::cerr << "Error: getaddrinfo: " << gai_strerror(rv) << std::endl;
		return;
	}

	for (p = servinfo; p != NULL; p = p->ai_next) {
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sockfd == -1) {
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			continue;
		}

		break;
	}

	if (p == NULL) {
		std::cerr << "Error: Could not connect to server" << std::endl;
		return;
	}

	freeaddrinfo(servinfo);

	std::string request = "GET " + _request + " HTTP/1.0\r\n\r\n";
	send(sockfd, request.c_str(), request.size(), 0);

	memset(buffer, 0, sizeof(buffer));
	while ((n = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
		std::cout << buffer;
		memset(buffer, 0, sizeof(buffer));
	}

	close(sockfd);
}