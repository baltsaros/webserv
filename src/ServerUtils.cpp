#include "../inc/ServerUtils.hpp"

std::string	ws::readFile(const std::string &file) {
	std::ifstream	input_file(file);
	std::stringstream	buffer;
	buffer << input_file.rdbuf();
	return buffer.str();
}

bool	ws::checkExtension(const std::string &file, const std::string &ext) {
	size_t	lastDot = file.find_last_of('.');

	if (lastDot != std::string::npos && lastDot < file.size()) {
		std::string extension = file.substr(lastDot);
		return (extension.compare(ext) == 0);
	}
	return false;
}

bool	ws::fileExists(const std::string &file) {
	return access(file.c_str(), F_OK) == 0;
}

bool	ws::isDirectory(const std::string &path) {
	struct stat	params;

	if (stat(path.c_str(), &params) == 0)
		return S_ISDIR(params.st_mode);
	return false;
}