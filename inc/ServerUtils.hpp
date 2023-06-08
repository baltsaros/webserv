#pragma once

# include <string>
# include <iostream>
# include <fstream>
# include <sstream>
# include <unistd.h> // for access

namespace ws {
	std::string	readFile(const std::string &file);
	bool		checkExtension(const std::string &file, const std::string &ext);
	bool		fileExists(const std::string &file);
}