#include <iostream>
#include <string>
#include <sys/epoll.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

/*
	This code creates a socket using getaddrinfo to resolve the address 
	and port, bind it and set it to listen on the specified address and port. 
	Then it creates an epoll file descriptor and adds the listening socket to it.
	The infinite while loop uses epoll_wait to wait for events on the file 
	descriptor. When an event occurs, the code checks if the event is on the 
	listening socket, and if it is, it accepts the connection and adds the 
	new client socket to the epoll set. If the event is on a client socket, 
	it calls the handle_request function which reads the request, 
	checks if it's a GET request and sends back the appropriate response.
*/

const int MAX_EVENTS = 10;
const int PORT = 8080;
const char* ADDRESS = "0.0.0.0";

void handle_request(int fd) {
    char buffer[1024];
    int bytes_read = read(fd, buffer, 1024);

    std::string request(buffer, bytes_read);
    std::string method = request.substr(0, request.find(" "));
    std::string file = request.substr(request.find("/"), request.find("HTTP/") - request.find("/") - 1);

    if (method == "GET") {
        std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<h1>Hello World</h1>";
        write(fd, response.c_str(), response.size());
    } else {
        std::string response = "HTTP/1.1 400 Bad Request\nContent-Type: text/html\n\n<h1>Bad Request</h1>";
        write(fd, response.c_str(), response.size());
    }
    close(fd);
}

int main() {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo* res;
    int status = getaddrinfo(ADDRESS, std::to_string(PORT).c_str(), &hints, &res);
    if (status != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
        return 1;
    }

    int listen_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    int opt_val = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof opt_val);

    bind(listen_fd, res->ai_addr, res->ai_addrlen);

    listen(listen_fd, SOMAXCONN);

    int efd = epoll_create1(0);
    struct epoll_event event;
    event.data.fd = listen_fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(efd, EPOLL_CTL_ADD, listen_fd, &event);

    while (true) {
        struct epoll_event events[MAX_EVENTS];
        int n = epoll_wait(efd, events, MAX_EVENTS, -1);

        for (int i = 0; i < n; i++) {
			if (events[i].data.fd == listen_fd) {
				int client_fd = accept(listen_fd, nullptr, nullptr);
				setsockopt(client_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof opt_val);
				event.data.fd = client_fd;
				event.events = EPOLLIN | EPOLLET;
				epoll_ctl(efd, EPOLL_CTL_ADD, client_fd, &event);
			} 
			else if (events[i].events & EPOLLIN) {
				handle_request(events[i].data.fd);
			}
		}
	}
	close(listen_fd);
	return 0;
}