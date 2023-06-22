#ifndef CONFIGUTILS_HPP
# define CONFIGUTILS_HPP

# include <iostream>
# include <list>
# include <utility>
# include <sstream>
# include <vector>
# include <map>

# define CHAR_TAB			'\t'
# define CHAR_SPACE			' '
# define CHAR_SEMI_COLON	';'
# define CLOSING_BRACE	"}"

template < typename T >
void	printVector(std::vector<T> vector) {

	for (size_t i = 0; i < vector.size(); i++)
		std::cout << i << ": " << vector[i] << "\n";
}

std::list<std::pair<std::string, std::string> >	parseBlock(std::string &content);
void											printMap(std::map<int, std::string> map);
void											printMapStrings(std::map<std::string, std::string> map);

#endif