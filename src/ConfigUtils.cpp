#include "../inc/ConfigUtils.hpp"

/*
@param: content of a configuration block starting with "server {" and ending with "}"
as a string.
Takes this content and puts it into a list of pairs, the first element of the pair being
the left side of the directive and the second element le right side of it.
*/

std::list<std::pair<std::string, std::string> >	parseBlock(std::string &content) {

	std::list<std::pair<std::string, std::string> >	list;
	std::pair<std::string, std::string>				pair;
	std::istringstream								ss(content);
	std::string										buffer;
	size_t											pos1, pos2;

	getline(ss, buffer);							// just reads the first line
	while (getline(ss, buffer)) {
		if (buffer.empty())
			continue ;
		while (buffer[0] == CHAR_TAB)
			buffer.erase(0, 1);
		if (!buffer.compare(CLOSING_BRACE))
			continue ;
		pos1 = buffer.find(CHAR_SPACE);
		pair.first = buffer.substr(0, pos1);
		pos2 = buffer.find(CHAR_SEMI_COLON);
		pair.second = buffer.substr(pos1 + 1, pos2 - pos1 - 1);
		list.push_back(pair);
	}
	return (list);
}

void	printMap(std::map<int, std::string> map) {

	std::map<int, std::string>::iterator it = map.begin();

	while (it != map.end()) {
		std::cout << it->first << ", " << it->second << "\n";
		it++;
	}
}

void	printMapStrings(std::map<std::string, std::string> map) {

	std::map<std::string, std::string>::iterator it = map.begin();

	while (it != map.end()) {
		std::cout << it->first << ", " << it->second << "\n";
		it++;
	}
}
