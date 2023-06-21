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

bool	ws::checkNoExtension(const std::string &file) {
	size_t	lastDot = file.find_last_of('.');

	if (lastDot != std::string::npos && lastDot < file.size()) {
		return false;
	}
	return true;
}

bool	ws::fileExists(const std::string &file) {
	return access(file.c_str(), F_OK) == 0;
}

bool	ws::isDirectory(const std::string &path) {
	struct stat	params;
	//std::cout << "directory path " << path << "\n";
	if (stat(path.c_str(), &params) == 0)
		return S_ISDIR(params.st_mode);
	return false;
}

void	ws::trimTrailingChar(std::string &str, char trail) {
	size_t	pos = str.find_last_not_of(trail);

	if (pos != std::string::npos)
		str.erase(pos + 1);
}


void	ws::trimOneBlock(std::string &str, char trail) {
	size_t	pos = str.find_last_of(trail);

	if (pos != std::string::npos)
		str.erase(pos);
}

std::string	ws::removeRootPath(const std::string &target, const std::string &root) {
	std::string	toReturn = target;

	if (!root.compare("/"))
		return target;
	if (!toReturn.find(root))
		toReturn.erase(0, root.size());
	return toReturn;
}

std::string	ws::substrUntil(std::string str, char separ) {
	size_t		pos = str.find_first_of(separ);
	std::string	toReturn;

	if (pos != std::string::npos) {
		toReturn = str.substr(0, pos);
		return toReturn;
	}
	return "";
}