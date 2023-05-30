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

	getline(ss, buffer, '\n');							// just reads the first line
	while (getline(ss, buffer, '\n')) {
		if (buffer.empty())
			continue ;
		while (buffer[0] == '\t')
			buffer.erase(0, 1);
		if (!buffer.compare("}"))
			continue ;
		pos1 = buffer.find(' ');
		pair.first = buffer.substr(0, pos1);
		pos2 = buffer.find(';');
		pair.second = buffer.substr(pos1 + 1, pos2 - pos1 - 1);
		list.push_back(pair);
	}
	// for (std::list<std::pair<std::string, std::string> >::iterator it = list.begin(); it != list.end(); it++)
	// 	std::cout << "First: " << (*it).first << ", second; " << (*it).second << std::endl;
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

/*
int main() {

	// std::string content = "server {\n\tlisten 80;\t\t\t\t\t	# listening port (mandatory parameter) / if left blank, it will be 0.0.0.0:80\n\thost 127.0.0.1;\t\t\t\t\t# host\n\tserver_name plop;\t\t\t # server name / could be empty\n\tclient_max_body_size 1024;\t\t# max request body size in bytes\n\n\terror_page 404 /error/404.html;\t# default error page\n\n";
	std::string content = "\tlocation / {\n\t\troot /tmp/server_eval;\n\t\tindex index.html;\n\t}";
	
	parseBlock(content);
	return (0);
}
*/