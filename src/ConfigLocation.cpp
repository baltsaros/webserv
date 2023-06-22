#include "../inc/ConfigLocation.hpp"


ConfigLocation::ConfigLocation(std::string & content) {

	this->_autoindex = 0;
	parseLocation(content);
	printMap(this->_errorPages);
	// std::cout << "=============START LOCATION===============\n";
	// std::cout << "root: " << this->_root << std::endl;
	// std::cout << "index: " << this->_index << std::endl;
	std::cout << "autoindex in parser: " << this->_autoindex << std::endl;
	// printVector(this->_methods);
	// printMap(this->_return);
	// std::cout << "==============END LOCATION================\n";
}

ConfigLocation::ConfigLocation(ConfigLocation const & src) {

	*this = src;
}

ConfigLocation::~ConfigLocation() {
}

/*
@param: one pair representing one directive of the config file.
Puts the second term of the pair into the map _errorPages with the first term being
the error code and the second being the path to the error page.
*/

void	ConfigLocation::parseErrorPage(std::pair<std::string, std::string> &pair) {

	size_t		posb = 0;
	size_t		pos = (pair.second).find(CHAR_SPACE);

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

void	ConfigLocation::parseAutoIndex(std::pair<std::string, std::string> &pair) {

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

void	ConfigLocation::parseMethods(std::pair<std::string, std::string> &pair) {

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

void	ConfigLocation::parseReturn(std::pair<std::string, std::string> &pair) {

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

void	ConfigLocation::dispatch(std::pair<std::string, std::string> &pair) {
	
	if (pair.first == ROOT)
		setRoot(pair.second);
	else if (pair.first == INDEX)
		setIndex(pair.second);
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
Parses the location block.
@param: content of the location block taken as a string.
*/

void	ConfigLocation::parseLocation(std::string &content) {

	std::list<std::pair<std::string, std::string> >	list;
	
	list = parseBlock(content);
	// for (std::list<std::pair<std::string, std::string> >::iterator	it = list.begin(); it != list.end(); it++)
	// 	std::cout << "First: " << (*it).first << ", second; " << (*it).second << std::endl;
	for (std::list<std::pair<std::string, std::string> >::iterator	it = list.begin(); it != list.end(); it++) {
		if (((*it).first).empty())
			continue ;
		dispatch(*it);
	}
	// for (int i = 1; i < this->_locationBlocks.size(); i++) {
	// 	this->_vectLocations.push_back(ConfigLocation(this->_locationBlocks[i]));
	// }
}

ConfigLocation & ConfigLocation::operator=(ConfigLocation const & rhs) {

	if (this != &rhs) {
		this->_errorPages = rhs._errorPages;
		this->_root = rhs._root;
		this->_index = rhs._index;
		this->_autoindex = rhs._autoindex;
		this->_methods = rhs._methods;
		this->_return = rhs._return;
	}
	return (*this);
}

void	ConfigLocation::setRoot(std::string &root) {

	this->_root = root;
}

void	ConfigLocation::setIndex(std::string &index) {

	this->_index = index;
}

std::map<int, std::string>	ConfigLocation::getErrorPages(void) const {

	return (this->_errorPages);
}

std::string	ConfigLocation::getRoot(void) const {

	return (this->_root);
}

std::string	ConfigLocation::getIndex(void) const {

	return (this->_index);
}

bool	ConfigLocation::getAutoIndex(void) const {

	return (this->_autoindex);
}

std::vector<std::string>	ConfigLocation::getMethods(void) const {

	return (this->_methods);
}

std::map<int, std::string>	ConfigLocation::getReturn(void) const {

	return (this->_return);
}