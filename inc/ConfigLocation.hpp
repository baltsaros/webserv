#ifndef CONFIGLOCATION_HPP
# define CONFIGLOCATION_HPP

# include <iostream>
# include <string>
# include <vector>
# include <list>
# include <map>
# include "ConfigUtils.hpp"

# define COMMENT_DELIMITER	"#"
# define ERROR_PAGE			"error_page"
# define ROOT				"root"
# define INDEX				"index"
# define AUTOINDEX			"autoindex"
# define ON					"on"
# define OFF				"off"
# define METHODS			"methods"
# define RETURN				"return"
# define CHAR_SPACE			' '

class ConfigLocation {

	private:
		std::map<int, std::string>	_errorPages;
		std::string					_root;
		std::string					_index;
		bool						_autoindex;
		std::vector<std::string>	_methods;
		std::map<int, std::string>	_return;

		ConfigLocation(void);

		void						parseErrorPage(std::pair<std::string, std::string> &pair);
		void						parseAutoIndex(std::pair<std::string, std::string> &pair);
		void						parseMethods(std::pair<std::string, std::string> &pair);
		void						parseReturn(std::pair<std::string, std::string> &pair);
		void						dispatch(std::pair<std::string, std::string> &pair);
		void						parseLocation(std::string &content);

	public:
		ConfigLocation(std::string & content);
		ConfigLocation(ConfigLocation const & src);
		~ConfigLocation(void);

		ConfigLocation	& operator=(ConfigLocation const & rhs);

		void						setRoot(std::string &root);
		void						setIndex(std::string &index);

		std::map<int, std::string>	getErrorPages(void) const;
		std::string					getRoot(void) const;
		std::string					getIndex(void) const;
		bool						getAutoIndex(void) const;
		std::vector<std::string>	getMethods(void) const;
		std::map<int, std::string>	getReturn(void) const;
};

#endif