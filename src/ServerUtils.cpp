#include "../inc/ServerUtils.hpp"

std::string	read_file(const std::string &filename) {
	std::ifstream	input_file(filename);
	std::stringstream	buffer;
	buffer << input_file.rdbuf();
	return buffer.str();
}

bool	checkCssExtension(const std::string &filepath) {
	size_t	lastDot = filepath.find_last_of('.');

	if (lastDot != std::string::npos && lastDot < filepath.size()) {
		std::string extension = filepath.substr(lastDot);
		return (extension.compare(".css") == 0);
	}
	return false;
}