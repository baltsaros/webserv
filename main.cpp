#include "inc/Server.hpp"
#include "inc/Configuration.hpp"

int	main(int argc, char **argv) {
	// ws::Server	s1(AF_INET, SOCK_STREAM, 0, 9999, INADDR_ANY, 32);
	// s1.launcher();

	if (argc != 2) {
		std::cerr << "Error: Invalid parameter!" << std::endl;
		return 1;
	}
	// std::cout << "argv: " << argv[1] << std::endl;
	Configuration	config(argv[1]);

	// std::map<int, std::string>	errors = config.getConfigServer()[0]->getErrorPages();
	// printMap(errors);
	std::map<std::string, ConfigLocation*>	locs = config.getConfigServer()[0]->getLocation();
	// std::map<std::string, ConfigLocation*>	locs2 = config.getConfigServer()[1]->getLocation();
	// std::cout << "root css: " << locs["/css"]->getRoot() << std::endl;
	// std::cout << "root /: " << locs["/"]->getRoot() << std::endl;

	std::vector<int>	ports = config.getConfigServer()[0]->getPorts();
	// std::vector<int>	ports2 = config.getConfigServer()[1]->getPorts();
	ws::Server  s1(AF_INET, SOCK_STREAM, 0, ports, INADDR_ANY, 32, config);
	// ws::Server  s2(AF_INET, SOCK_STREAM, 0, ports2, INADDR_ANY, 32, config);
	s1.launcher();
	// s2.launcher();

	return (0);
}