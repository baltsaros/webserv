/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ejoo-tho <ejoo-tho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 13:26:20 by ejoo-tho          #+#    #+#             */
/*   Updated: 2023/06/01 15:13:13 by ejoo-tho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
Configuration::~Configuration()
{
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

	ifs.open(path);
	if (ifs.fail()) {
		std::cout << "Error : file couldn't open\n";
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
		this->_vectServers.push_back(ConfigServer(this->_configBlocks[i]));
	}
}

ConfigServer	Configuration::getConfigServer(int i) const {
	
	return (this->_vectServers[i]);
}