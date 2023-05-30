#include "../inc/ConfigServer.hpp"

ConfigServer::ConfigServer(std::string & content) {

	createServerBlocks(content);
	parseServer();
	std::cout << "=============START SERVER===============\n";
	printVector(this->_ports);
	std::cout << this->_host << "\n";
	printVector(this->_serverNames);
	std::cout << this->_clientMaxBodySize << "\n";
	printMap(this->_errorPages);
	std::cout << "==============END SERVER================\n";
}

ConfigServer::ConfigServer(ConfigServer const & src) {

	*this = src;
}

ConfigServer::~ConfigServer()
{}

/*
Cuts the server block into one first block containing the general block containing
the info about the server and the different location blocks contained in the server 
block.
@param: configuration block corresponding to one block
*/

void	ConfigServer::createServerBlocks(std::string & content) {

	size_t			posb = 0;
	size_t			pose = content.find(LOCATION, posb);

	this->_serverBlocks.push_back(content.substr(posb, pose - posb - 1));
	while (posb != std::string::npos) {
		posb = content.find(LOCATION, posb);
		pose = content.find(END_LOCATION_BLOCK, posb);
		if (posb < content.length() && pose << content.length()) {
			this->_serverBlocks.push_back(content.substr(posb, pose + 2 - posb));
			posb = pose + 1;
			pose = posb;
		}
		else 
			break ;
	}
	// for (int i = 0; i < this->_serverBlocks.size(); i++) {
	// 	std::cout << this->_serverBlocks[i];
	// 	std::cout << "\n=====================\n";
	// 	std::cout << "end i: " << i << "\n";
	// }
}

/*
@param: one pair representing one directive of the config file.
Puts the second term of the pair into the vector _ports after converting it into
an int.
*/

void	ConfigServer::parseListen(std::pair<std::string, std::string> &pair) {

	// check format
	this->_ports.push_back(stoi(pair.second));
}

/*
@param: one pair representing one directive of the config file.
Puts the second term of the pair into the vector _serverNames after splitting it if 
there are several names.
*/

void	ConfigServer::parseServerNames(std::pair<std::string, std::string> &pair) {

	size_t	posb = 0;
	size_t	pos = (pair.second).find(' ');

	while (pos != std::string::npos) {
		this->_serverNames.push_back((pair.second).substr(posb, pos - posb));
		posb = pos + 1;
		pos = (pair.second).find(' ', posb);
	}
	this->_serverNames.push_back((pair.second).substr(posb, (pair.second).length() - posb));
}

/*
@param: one pair representing one directive of the config file.
Puts the second term of the pair into its corresponding attribute after transforming
it into an int.
*/

void	ConfigServer::parseClientSize(std::pair<std::string, std::string> &pair) {

	// check format
	this->_clientMaxBodySize = stoi(pair.second);
}

/*
@param: one pair representing one directive of the config file.
Puts the second term of the pair into the map _errorPages with the first term being
the error code and the second being the path to the error page.
*/

void	ConfigServer::parseErrorPage(std::pair<std::string, std::string> &pair) {

	size_t		posb = 0;
	size_t		pos = (pair.second).find(' ');

	// check format code
	int			code = stoi((pair.second).substr(posb, pos - posb));
	std::string	page = (pair.second).substr(pos + 1, (pair.second).length() - pos);
	
	this->_errorPages.insert(std::pair<int, std::string>(code, page));
}

/*
@param: one pair representing one directive of the config file.
Parses the autoindex directive and puts it into a boolean (true if autoindex on and
false if off)
*/

void	ConfigServer::parseAutoIndex(std::pair<std::string, std::string> &pair) {

	if ((pair.second) == ON)
		this->_autoindex = true;
	else if ((pair.second) == OFF)
		this->_autoindex = false;
	else {
		std::cout << "Error: wrong autoindex format\n";
		exit(EXIT_FAILURE);
	}

}

/*
@param: one pair representing one directive of the config file.
Puts the second term of the pair into the vector _methods after splitting it if 
there are methods.
*/

void	ConfigServer::parseMethods(std::pair<std::string, std::string> &pair) {

	size_t	posb = 0;
	size_t	pos = (pair.second).find(CHAR_SPACE);

	while (pos != std::string::npos) {
		this->_methods.push_back((pair.second).substr(posb, pos - posb));
		posb = pos + 1;
		pos = (pair.second).find(CHAR_SPACE, posb);
	}
	this->_methods.push_back((pair.second).substr(posb, (pair.second).length() - posb));
}

/*
@param: one pair representing one directive of the config file.
Puts the second term of the pair into the map _return with the first term being
the code and the second being the path to redirect to.
*/

void	ConfigServer::parseReturn(std::pair<std::string, std::string> &pair) {

	size_t		posb = 0;
	size_t		pos = (pair.second).find(CHAR_SPACE);

	// check format code
	int			code = stoi((pair.second).substr(posb, pos - posb));
	std::string	page = (pair.second).substr(pos + 1, (pair.second).length() - pos);
	
	this->_return.insert(std::pair<int, std::string>(code, page));
}

/*
@param: one pair representing one directive of the config file.
Depending on the first term of the directive, dispatches the pair to the parsing
function of that directive.
*/

void	ConfigServer::dispatch(std::pair<std::string, std::string> &pair) {
	
	if (pair.first == HOST)
		setHost(pair.second);
	else if (pair.first == ROOT)
		setRoot(pair.second);
	else if (pair.first == INDEX)
		setIndex(pair.second);
	else if (pair.first == LISTEN)
		parseListen(pair);
	else if (pair.first == SERVER_NAME)
		parseServerNames(pair);
	else if (pair.first == CLIENT_SIZE)
		parseClientSize(pair);
	else if (pair.first == ERROR_PAGE)
		parseErrorPage(pair);
	else if (pair.first == AUTOINDEX)
		parseAutoIndex(pair);
	else if (pair.first == METHODS)
		parseMethods(pair);
	else if (pair.first == RETURN)
		parseReturn(pair);
	else {
		std::cout << "Error: unknown directive in config file\n";
		exit(EXIT_FAILURE);
	}
}

/*
Parses the first server block and constructs the location blocks.
*/

void	ConfigServer::parseServer() {

	std::list<std::pair<std::string, std::string> >	list;
	
	list = parseBlock(this->_serverBlocks[0]);
	// for (std::list<std::pair<std::string, std::string> >::iterator	it = list.begin(); it != list.end(); it++)
	// 	std::cout << "First: " << (*it).first << ", second; " << (*it).second << std::endl;
	for (std::list<std::pair<std::string, std::string> >::iterator	it = list.begin(); it != list.end(); it++) {
		if (((*it).first).empty())
			continue ;
		dispatch(*it);
	}
	for (int i = 1; i < this->_serverBlocks.size(); i++) {
		this->_vectLocations.push_back(ConfigLocation(this->_serverBlocks[i]));
	}
}

ConfigServer & ConfigServer::operator=(ConfigServer const & rhs) {

	if (this != &rhs) {
		this->_serverBlocks = rhs._serverBlocks;
		this->_ports = rhs._ports;
		this->_host = rhs._host;
		this->_serverNames = rhs._serverNames;
		this->_clientMaxBodySize = rhs._clientMaxBodySize;
		this->_errorPages = rhs._errorPages;
		this->_root = rhs._root;
		this->_index = rhs._index;
		this->_autoindex = rhs._autoindex;
		this->_methods = rhs._methods;
		this->_return = rhs._return;
	}
	return (*this);
}

void	ConfigServer::setHost(std::string &host) {

	this->_host = host;
}

void	ConfigServer::setRoot(std::string &root) {

	this->_root = root;
}

void	ConfigServer::setIndex(std::string &index) {

	this->_index = index;
}

// PUBLIC MB FUNCTIONS

std::vector<std::string>	ConfigServer::getServerBlocks(void) const {

	return (this->_serverBlocks);
}

std::vector<int>	ConfigServer::getPorts(void) const {

	return (this->_ports);
}

std::string	ConfigServer::getHost(void) const {

	return (this->_host);
}

std::vector<std::string>	ConfigServer::getServerNames(void) const {

	return (this->_serverNames);
}

int	ConfigServer::getClientMaxBodySize(void) const {

	return (this->_clientMaxBodySize);
}

std::map<int, std::string>	ConfigServer::getErrorPages(void) const {

	return (this->_errorPages);
}

std::string	ConfigServer::getRoot(void) const {

	return (this->_root);
}

std::string	ConfigServer::getIndex(void) const {

	return (this->_index);
}

bool	ConfigServer::getAutoIndex(void) const {

	return (this->_autoindex);
}

std::vector<std::string>	ConfigServer::getMethods(void) const {

	return (this->_methods);
}

std::map<int, std::string>	ConfigServer::getReturn(void) const {

	return (this->_return);
}
