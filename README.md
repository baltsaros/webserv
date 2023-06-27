# webserv

[WORK IN PROGRESS]

# **The team**
* Jérémy Vander Motte ([intra](https://profile.intra.42.fr/users/jvander-)/[git](https://github.com/JeremyVanderMotte))
* Elsa Joo Thomson ([intra](https://profile.intra.42.fr/users/ejoo-tho)/[git](https://github.com/ejt22))
* Aleksandr Buzdin ([intra](https://profile.intra.42.fr/users/abuzdin)/[git](https://github.com/baltsaros))


# **Project description** 
This project is about creating an HTTP server. Nginx serves as a reference.


## The server should:
* Take a configuration file as argument, or use a default path.
* Return a proper HTTP response status codes
* Work with GET, POST and DELETE methods
* Have CGI and ability to upload files
* Be able to listen to multiple prots and and launch several servers at the same time


## Allowed functions:
* Everything in C++98
* execve, du, dup2, pipe, fork
* strerror, gai_strerror, errno
* htons, htonl, ntohs, ntohl
* select, poll, epoll, kqueue
* socket, bind, connect, accept, listen
* getaddrinfo, freeaddrinfo, setsockopt, getsockname, getprotobyname
* recv, send
* fcntl, all FD macros (like FD_SET, FD_CLR, FD_ISSET)


# **How to use**
* *make* - to compile the executable file
* execute it with *./webserv* to launch it with a default config
* or *./webserv [path to a config]* to lauch with a config of your choice
* a server block in the config should contain port, host name, path for error pages and locations


# **Our approach**
## General idea
* Read config, save parameters from it, create and prepare listening sockets, launch main loop. Inside it save every request, parse it to get all the required parameters, create a proper response and send it back. The loop keeps running until timeout

## Parts of the project:
* Config parser
* Main loop (with socket creator, accepter, handler and responder)
* Request parser
* Responder
* Website pages
* CGI
* Unit-tests

## Config parser
* We decided to create several classes for the config parser: one for server blocks, one for locations, general one (that contains all other classes)

## Main loop
* We adapted the IBM's implementation for our main loop
* Loop starts with SELECT(), then it checks whether a socket is in the FD_SET and checks its type. If it is a listening socket, the server tries to accept the incoming connections. Otherwise it performs I/O operations
* Before the main loop we create listening sockets and add them to the FD_SET
* We mapped every listening socket to the corresponding server block in order to track locations properly
* In SELECT() we used only one FD_SET for the sake of simplicity

## Request parser
* We split header and body, then save parameters from the starting line. All other header's parameters are mapped
* Then we do error checks (mainly method and target) and create a proper path to the targeted page

## Unit-tests
* We used Python to create unit tests


# **Attempts**
* 1st attempt: 100 % 


# **Folders**
***config** - contains config files
***inc** - headers
***src** - cpp files
***unit-tests** - some basic Python tests
***website** - content for the website


# **Useful resources**
* [Article](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa) on how to create a basic HTTP server in C. Contains some useful links, including ones to RFC
* [RFC 7230](https://www.rfc-editor.org/rfc/rfc7230), where you can find information about request and response format
* [Video](https://www.youtube.com/watch?v=YwHErWJIh6Y) on how to build a basic HTTP server in C++
* [Video](https://www.youtube.com/watch?v=Y6pFtgRdUts) about SELECT()
* [Implementation](https://www.ibm.com/docs/en/i/7.2?topic=designs-example-nonblocking-io-select) of the main loop by IBM. Really good one, need just adapt it a bit for multiple ports and servers
