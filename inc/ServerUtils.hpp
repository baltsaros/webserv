#pragma once

# include <string>
# include <iostream>
# include <fstream>
# include <sstream>
# include <sys/stat.h> // to check whether a file is directory or not
# include <unistd.h> // for access

namespace ws {
	std::string		readFile(const std::string &file);
	bool			checkExtension(const std::string &file, const std::string &ext);
	bool			checkNoExtension(const std::string &file);
	bool			fileExists(const std::string &file);
	bool			isDirectory(const std::string &path);
	void			trimTrailingChar(std::string &str, char trail);
	void			trimOneBlock(std::string &str, char separ);
	std::string		substrUntil(std::string str, char separ);
	std::string		removeRootPath(const std::string &target, const std::string &root);
}