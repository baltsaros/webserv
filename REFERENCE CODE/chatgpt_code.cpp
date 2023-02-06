
// ############################################################################################
/*
	basic example and it does not handle things like error checking for invalid requests, 
	parsing the requested URL, or serving different types of files. 
	It's also worth noting that this is a blocking server, 
	which means that it can only handle one request at a time. 
	However, it can serve as a starting point for a more complete and robust HTTP server.
*/

#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

const int MAX_REQUEST_LENGTH = 8192;

int main() {
	
	/*
		Commented block will be conf file parsing + EPOLL usage
	*/
    
	// int server_sock, client_sock;
    // sockaddr_in server_addr, client_addr;
    // socklen_t client_addr_len = sizeof(client_addr);

    // // Create the socket
    // if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    //     cerr << "Error creating socket" << endl;
    //     return 1;
    // }

    // // Bind the socket to an address and port
    // memset(&server_addr, 0, sizeof(server_addr));
    // server_addr.sin_family = AF_INET;
    // server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // server_addr.sin_port = htons(8080);
    // if (bind(server_sock, (sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
    //     cerr << "Error binding socket" << endl;
    //     return 1;
    // }

    // // Listen for incoming connections
    // if (listen(server_sock, 5) < 0) {
    //     cerr << "Error listening for connections" << endl;
    //     return 1;
    // }

    while (true) {
        // Accept a new connection
        if ((client_sock = accept(server_sock, (sockaddr*) &client_addr, &client_addr_len)) < 0) {
            cerr << "Error accepting connection" << endl;
            continue;
        }

        // Read the request from the client
        char request[MAX_REQUEST_LENGTH];
        int bytes_received = recv(client_sock, request, MAX_REQUEST_LENGTH - 1, 0);
        if (bytes_received < 0) {
            cerr << "Error reading request" << endl;
            close(client_sock);
            continue;
        }
        request[bytes_received] = '\0';
	/*
	GET
	*/
        // Parse the request to check if it is a GET request
        string req_str(request);
        if (req_str.substr(0, 3) != "GET") {
            close(client_sock);
            continue;
        }

        // Send the response
        string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 12\r\n\r\nHello, World!";
        send(client_sock, response.c_str(), response.length(), 0);

	/*
	POST
	*/

		// Parse the request to check if it is a POST request
        string req_str(request);
        if (req_str.substr(0, 4) != "POST") {
            close(client_sock);
            continue;
        }

        // Extract the post data from the request
        int post_data_start = req_str.find("\r\n\r\n");
        string post_data = req_str.substr(post_data_start + 4);
        cout << "Received post data: " << post_data << endl;

        // Send the response
        string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 12\r\n\r\n\r\nPost data received";
		send(client_sock, response.c_str(), response.length(), 0);

	/*
	DELETE
	*/

       // Parse the request to check if it is a DELETE request
        string req_str(request);
        if (req_str.substr(0, 6) != "DELETE") {
            close(client_sock);
            continue;
        }

        // Send the response
        string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 14\r\n\r\nDelete successful";
        send(client_sock, response.c_str(), response.length(), 0);
		
	// ----------------------

        // Close the client socket
        close(client_sock);
    }

    close(server_sock);
    return 0;
}

// ############################################################################################

/*
	Super basic conf file parser
	Basically handles different blocks (server, location, ...) separately
*/

#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::string line;
    std::ifstream configFile("nginx.conf");

    if (configFile.is_open())
    {
        while (std::getline(configFile, line))
        {
            if (line.empty() || line[0] == '#')
            {
                continue;
            }

            // Here you would parse the line, for example:
            // if (line.substr(0, 7) == "server {")
            // {
            //     // Handle server block
            // }
            // else if (line.substr(0, 8) == "location")
            // {
            //     // Handle location block
            // }
            // ...

            std::cout << line << std::endl;
        }
        configFile.close();
    }
    else
    {
        std::cout << "Unable to open configuration file" << std::endl;
    }

    return 0;
}

// ############################################################################################
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
// ############################################################################################
