
#define MAX_EVENTS 5
#define TIMEOUT 10000 //(in milliseconds, divide by 1000 for secs)
// #define READ_SIZE 10

// #include <stdio.h>     // for fprintf()
// #include <unistd.h>    // for close(), read()
// #include <sys/epoll.h> // for epoll_create1(), epoll_ctl(), struct epoll_event
// #include <string.h>    // for strncmp

int get_listener_socket(void)
{
    int listener;     // Listening socket descriptor
    int yes = 1;        // For setsockopt() SO_REUSEADDR, below
    int return_value;

    struct addrinfo hints, *ai, *p;

    // Get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((return_value = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(EXIT_FAILURE);
    }
    
    for(p = ai; p != NULL; p = p->ai_next) 
	{
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) { 
            continue;
        }
        
        // Lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    freeaddrinfo(ai); // All done with this

    // If we got here, it means we didn't get bound
    if (p == NULL) {
        return -1;
    }

    // Listen
    if (listen(listener, 10) == -1) {
        return -1;
    }

    return listener;
}

int main()
{
	int listener = get_listener_socket();
	if (listener == -1) 
	{
		fprintf(stderr, "error getting listening socket\n");
		exit(EXIT_FAILURE);
    }

	struct epoll_event ep_event;
	ep_event.event = EPOLLIN;
	ep_event.data.fd = listener;

	int epoll_fd = epoll_create1(0);
	if (epoll_fd == -1)
	{
		//errno
	}

	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listener, &ep_event) )
	{
		//error
		close(epoll_fd);
	}

	// int epoll_wait(int epfd, struct epoll_event *events,
                    //   int maxevents, int timeout);
	struct epoll_event *events;
	int event_count;
	while (1)
	{
		event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, TIMEOUT);
		for (int i = 0; i < event_count; i++)
		{
			//Do something with ready fds
		}
		// Add condition to stop infinite loop
	}

	if (close(epoll_fd) )
	{
		//error closing fd
	}
	
	return 0;
}