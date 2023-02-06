#include "Server.hpp"

int	main(void) {
	ws::Server	s1(AF_INET, SOCK_STREAM, 0, 9999, INADDR_ANY, 10);

	s1.launcher();
	return (0);
}