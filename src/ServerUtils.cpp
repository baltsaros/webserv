#include "../inc/ServerUtils.hpp"

/*
** Read a file and put its content in buffer
** @param const std::string & file : The file to read
** @return std::string : Buffer containing the file content
*/
std::string	ws::readFile(const std::string &file) {
	std::ifstream	input_file(file);
	std::stringstream	buffer;
	buffer << input_file.rdbuf();
	return buffer.str();
}

/*
** Check extension of a file
** @param const std::string & file : the file to check extension
** @param const std::string & ext : the extension to check
** @return true if extension match, false otherwise.
*/
bool	ws::checkExtension(const std::string &file, const std::string &ext) {
	size_t	lastDot = file.find_last_of('.');

	if (lastDot != std::string::npos && lastDot < file.size()) {
		std::string extension = file.substr(lastDot);
		return (extension.compare(ext) == 0);
	}
	return false;
}

/*
** Check if a file has no extension.
** @param const std::String & file : The file to check.
** @return true if the file has no extension, false otherwise.
*/
bool	ws::checkNoExtension(const std::string &file) {
	size_t	lastDot = file.find_last_of('.');

	if (lastDot != std::string::npos && lastDot < file.size()) {
		return false;
	}
	return true;
}

/*
** Check if a file exists.
** @param const std::string & file : the file to check.
** @return true if the file exists, false, otherwise.
*/
bool	ws::fileExists(const std::string &file) {
	return access(file.c_str(), F_OK) == 0;
}

/*
** Check if a path is a directory.
** @param const std::String & path : The path to check
** @return True if the path is a directory, false otherwise.
*/
bool	ws::isDirectory(const std::string &path) {
	struct stat	params;
	if (stat(path.c_str(), &params) == 0)
		return S_ISDIR(params.st_mode);
	return false;
}

/*
** Remove all trail chars at the end.
** @param std::string & str : The string to trim
** @param char trail : The char to remove
** @return No return
*/
void	ws::trimTrailingChar(std::string &str, char trail) {
	size_t	pos = str.find_last_not_of(trail);

	if (pos != std::string::npos)
		str.erase(pos + 1);
}

/*
** Remove what after a trail.
** @param std::string & str : The string to operate
** @param char trail : the char where to start triming
** @return No return
*/
void	ws::trimOneBlock(std::string &str, char trail) {
	size_t	pos = str.find_last_of(trail);

	if (pos != std::string::npos)
		str.erase(pos);
}

/*
** Remove the root of target path if present.
** @param const st::string & target : The target to trim
** @param const std::string & root : the path for root
** @return std::string : The new string trimmed.
*/
std::string	ws::removeRootPath(const std::string &target, const std::string &root) {
	std::string	toReturn = target;

	if (!root.compare("/"))
		return target;
	if (!toReturn.find(root))
		toReturn.erase(0, root.size());
	return toReturn;
}

/*
** Trim a string and return what's before the separator.
** @param std::String str : The string to trim.
** @param char separ : The separator to find.
** @return std::string : The string trimmed of empty string
		if separator is not found
*/
std::string	ws::substrUntil(std::string str, char separ) {
	size_t		pos = str.find_first_of(separ);
	std::string	toReturn;

	if (pos != std::string::npos) {
		toReturn = str.substr(0, pos);
		return toReturn;
	}
	return "";
}