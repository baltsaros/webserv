#ifndef CONFIGSERVER_HPP
# define CONFIGSERVER_HPP

# include <iostream>
# include <string>
# include <vector>
# include <list>
# include <map>
# include "ConfigUtils.hpp"
# include "ConfigLocation.hpp"

# define LOCATION			"location"
# define END_LOCATION_BLOCK	"}\n"
# define COMMENT_DELIMITER	"#"
# define LISTEN				"listen"
# define HOST				"host"
# define SERVER_NAME		"server_name"
# define CLIENT_SIZE		"client_max_body_size"
# define ERROR_PAGE			"error_page"
# define ROOT				"root"
# define INDEX				"index"
# define AUTOINDEX			"autoindex"
# define ON					"on"
# define OFF				"off"
# define METHODS			"methods"
# define RETURN				"return"
# define CHAR_SPACE			' '

class ConfigLocation;

class ConfigServer {

	private:
		std::vector<std::string>	_serverBlocks;
		std::vector<ConfigLocation>	_vectLocations;
		std::vector<int>			_ports;
		std::string					_host;
		std::vector<std::string>	_serverNames;
		int							_clientMaxBodySize;
		std::map<int, std::string>	_errorPages;
		std::string					_root;
		std::string					_index;
		bool						_autoindex;
		std::vector<std::string>	_methods;
		std::map<int, std::string>	_return;
		
		ConfigServer(void);

		void	createServerBlocks(std::string & content);
		void	parseListen(std::pair<std::string, std::string> &pair);
		void	parseServerNames(std::pair<std::string, std::string> &pair);
		void	parseClientSize(std::pair<std::string, std::string> &pair);
		void	parseErrorPage(std::pair<std::string, std::string> &pair);
		void	parseAutoIndex(std::pair<std::string, std::string> &pair);
		void	parseMethods(std::pair<std::string, std::string> &pair);
		void	parseReturn(std::pair<std::string, std::string> &pair);
		void	dispatch(std::pair<std::string, std::string> &pair);
		void	parseServer();
		
	public:
		ConfigServer(std::string & content);
		ConfigServer(ConfigServer const & src);
		~ConfigServer(void);

		ConfigServer & operator=(ConfigServer const & rhs);

		void	setHost(std::string &host);
		void	setRoot(std::string &root);
		void	setIndex(std::string &index);

		std::vector<std::string>	getServerBlocks(void) const;
		std::vector<ConfigLocation>	getLocation(void) const;
		ConfigLocation				getOneLocation(int i) const;
		std::vector<int>			getPorts(void) const;
		std::string					getHost(void) const;
		std::vector<std::string>	getServerNames(void) const;
		int							getClientMaxBodySize(void) const;
		std::map<int, std::string>	getErrorPages(void) const;
		std::string					getRoot(void) const;
		std::string					getIndex(void) const;
		bool						getAutoIndex(void) const;
		std::vector<std::string>	getMethods(void) const;
		std::map<int, std::string>	getReturn(void) const;
};

#endif