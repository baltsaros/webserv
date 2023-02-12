#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// In this example, the `Client` class takes in a host, a port, and a request as constructor arguments. 
// The `ConnectAndSendRequest` method creates a socket, connects to the server, and sends the HTTP GET request. 
// The response from the server is then displayed on the standard output. 
// The main function takes in the host, port, and request as command line arguments and creates an instance of the `Client` class.

class Client {
public:
    Client(const std::string& host, const std::string& port, const std::string& request);

    void ConnectAndSendRequest();

private:
    std::string _host;
    std::string _port;
    std::string _request;
};

#endif // CLIENT_HPP

