/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ejoo-tho <ejoo-tho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 13:14:14 by ejoo-tho          #+#    #+#             */
/*   Updated: 2023/05/30 14:56:54 by ejoo-tho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include <iostream>
# include <fstream>
# include <iterator>
# include <vector>
# include "ConfigServer.hpp"

# define SERVER_START	"server {"
# define END_SERVER_BLOCK	"\n}"

class Server;

class Configuration {
	
	private:
		std::vector<ConfigServer>	_vectServers;
		std::vector<std::string>	_configBlocks;

		Configuration(Configuration const & src);
		Configuration	& operator=(Configuration const & rhs);

		void	createConfigBlocks(std::string const & path);
		void	parseConfig();

		Configuration(); 
	public:
		Configuration(std::string const & path);
		~Configuration();

		Server const	& getServer(int i);

};

#endif