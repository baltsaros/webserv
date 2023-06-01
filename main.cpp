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
	Configuration	config(argv[1]);

	std::vector<ConfigLocation>	locations = config.getConfigServer(0).getLocation();
	std::cout << "location vector size: " << locations.size() << std::endl;

	// int	port = config.getConfigServer(0).getPorts()[0];
	// ws::Server  s1(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, 32, config);
	// s1.launcher();

	return (0);
}