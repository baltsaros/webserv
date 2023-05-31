#include "inc/Server.hpp"
#include "inc/Configuration.hpp"

int	main(int argc, char **argv) {
	// ws::Server	s1(AF_INET, SOCK_STREAM, 0, 9999, INADDR_ANY, 32);
	// s1.launcher();

	if (argc != 2) {
		std::cerr << "Error: Invalid parameter!" << std::endl;
		return 1;
	}
	std::cout << "argv: " << argv[1] << std::endl;
	Configuration	s1(argv[1]);

	return (0);
}