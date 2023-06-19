#include <iostream>
// #include <fstream>
// #include "./inc/Request.hpp"

	void	trimTrailingChar(std::string &str, char trail) {
	size_t	pos = str.find_last_not_of(trail);

	if (pos != std::string::npos)
		str.erase(pos + 1);
	}

int main()
{
	std::string str = "test/index/";
	trimTrailingChar(str, '/');
	std::string str2 = "test/index";
	trimTrailingChar(str2, '/');
	std::cout << "first " << str << "\n";
	std::cout << "second " << str2 << "\n";
	return (0);
}