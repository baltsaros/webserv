#include "inc/Server.hpp"
#include "inc/Configuration.hpp"

int	main(int argc, char **argv) {
	if (argc > 2) {
		std::cerr << "Error: Invalid parameter!" << std::endl;
		return 1;
	}
	// std::cout << "argv: " << argv[1] << std::endl;

	Configuration	*config;
	if (argc == 1)
		config = new Configuration("config/default.conf");
	else
		config = new Configuration(argv[1]);
	// std::map<int, std::string>	errors = config.getConfigServer()[0]->getErrorPages();
	// printMap(errors);
	// std::map<std::string, ConfigLocation*>	locs = config.getConfigServer()[0]->getLocation();
	// std::map<std::string, ConfigLocation*>	locs2 = config.getConfigServer()[1]->getLocation();
	// std::cout << "root css: " << locs["/assets"]->getRoot() << std::endl;
	// std::cout << "root /: " << locs["/"]->getRoot() << std::endl;

	// config.getConfigServer()[0];
	ws::Server  s1(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 32, *config);
	s1.launcher();
	delete config;
	return (0);
}