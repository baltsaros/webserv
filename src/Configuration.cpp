#include "../inc/Configuration.hpp"


Configuration::Configuration()
{
}

Configuration::Configuration(std::string const & path) {
	createConfigBlocks(path);
	parseConfig();
}

Configuration::Configuration(Configuration const & src) {
	
	*this = src;
}
Configuration::~Configuration() {
	
	this->_vectServers.clear();
}

Configuration	& Configuration::operator=(Configuration const & rhs) {

	if (this != &rhs) {
		this->_vectServers = rhs._vectServers;
		this->_configBlocks = rhs._configBlocks;
	}
	return (*this);
}
/*
Turns the content of the config file into a string (content) and separates it into blocks.
Puts the blocks into the vector _configBlocks.
@param : path of the config file;
*/

void	Configuration::createConfigBlocks(std::string const & path) {

	std::ifstream	ifs;
	size_t			posb = 0;
	size_t			pose = 0;

	if (!ws::checkExtension(path, ".conf")) {
		std::cout << "Error : wrong file extension\n";
		exit(EXIT_FAILURE) ;
	}
	ifs.open(path);
	if (ifs.fail()) {
		std::cout << "Error : nothing in the file\n";
		exit(EXIT_FAILURE) ; // gestion d'erreur?
	}
	std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	while (posb != std::string::npos) {
		posb = content.find(START_SERVER_BLOCK, posb);
		pose = content.find(END_SERVER_BLOCK, posb);
		if (posb < content.length() && pose << content.length()) {
			this->_configBlocks.push_back(content.substr(posb, pose + 2 - posb));
			posb = pose + 1;
			pose = posb;
		}
		else 
			break ;
	}
	ifs.close();
	// for (int i = 0; i < this->_vectBlocks.size(); i++) {
	// 	std::cout << this->_vectBlocks[i];
	// 	std::cout << "\n=====================\n";
	// }
}

/*
Takes the configuration blocks and calls the Server constructor, 
then pushes back the constructed Servers into _vectServers.
*/

void	Configuration::parseConfig() {

	for (int i = 0; i < this->_configBlocks.size(); i++) {
		this->_vectServers.push_back(new ConfigServer(this->_configBlocks[i]));
	}
}

std::vector<ConfigServer *>	Configuration::getConfigServer(void) const {
	
	return (this->_vectServers);
}