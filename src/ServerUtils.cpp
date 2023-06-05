#include "../inc/ServerUtils.hpp"

std::string	read_file(const std::string &file) {
	std::ifstream	input_file(file);
	std::stringstream	buffer;
	buffer << input_file.rdbuf();
	return buffer.str();
}

bool	checkExtension(const std::string &file, const std::string &ext) {
	size_t	lastDot = file.find_last_of('.');

	if (lastDot != std::string::npos && lastDot < file.size()) {
		std::string extension = file.substr(lastDot);
		return (extension.compare(ext) == 0);
	}
	return false;
}

bool	fileExists(const std::string &file) {
	return access(file.c_str(), F_OK) == 0;
}