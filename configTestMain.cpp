#include <iostream>
#include "./inc/Configuration.hpp"
// #include <fstream>
// #include "./inc/Request.hpp"

int main()
{
	// std::ifstream	ifs; 
	
	// ifs.open("request.txt");
	// if (ifs.fail()) {
	// 	std::cout << "Error : file couldn't open\n";
	// 	exit(EXIT_FAILURE) ; // gestion d'erreur?
	// }
	// std::string	content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	// Request	test(content);
	// ifs.close();

	Configuration	config("./config/config.conf");
	return (0);
}