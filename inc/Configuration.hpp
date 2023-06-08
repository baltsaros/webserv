/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ejoo-tho <ejoo-tho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 13:14:14 by ejoo-tho          #+#    #+#             */
/*   Updated: 2023/06/01 15:22:09 by ejoo-tho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include <iostream>
# include <fstream>
# include <iterator>
# include <vector>
# include "ConfigServer.hpp"

# define START_SERVER_BLOCK	"server {"
# define END_SERVER_BLOCK	"\n}"

class ConfigServer;

class Configuration {
	
	private:
		std::vector<ConfigServer *>	_vectServers;
		std::vector<std::string>	_configBlocks;


		void	createConfigBlocks(std::string const & path);
		void	parseConfig();

		
	public:
		Configuration(); 
		Configuration(std::string const & path);
		Configuration(Configuration const & src);
		~Configuration();

		Configuration	& operator=(Configuration const & rhs);
		
		std::vector<ConfigServer *>	getConfigServer(void) const;
};

#endif