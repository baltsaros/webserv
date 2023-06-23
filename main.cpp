#include "inc/Server.hpp"
#include "inc/Configuration.hpp"

int	main(int argc, char **argv) {
	
	if (argc > 2) {
		std::cerr << "Error: Invalid parameter!" << std::endl;
		return 1;
	}

	Configuration	*config;
	
	if (argc == 1)
		config = new Configuration("config/default.conf");
	else
		config = new Configuration(argv[1]);

	ws::Server  s1(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 64, *config);
	
	s1.launcher();

	delete config;
	
	return (0);
}