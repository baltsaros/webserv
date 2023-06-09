#pragma once

# include <string>
# include <iostream>
# include "Configuration.hpp"
# include "ServerUtils.hpp"
# include "ConfigUtils.hpp"

# define COLON		":"
# define SPACE		" "
# define NEWLINE	"\n"

namespace ws {
class Request {
	private:
		std::string								_buffer;
		Configuration							_config;
		std::string								_header;
		std::string								_body;
		std::string								_method;
		std::string								_target;
		std::string								_protocolVersion;
		std::map<std::string, std::string>		_headerFields;
		std::string								_path;
		std::string								_response;
		int										_returnStatus;
		std::map<std::string, ConfigLocation *>	_locations;
		void									_checkPos(size_t pos);
		std::string								_getParam(std::string toGet, size_t offset);
		void									_checkPath();
		void									_parseStartingLine();
		void									_parseHeaderFields();

	public:
		Request();
		Request(const std::string &buffer, const Configuration &config, std::map<std::string, ConfigLocation*> locations);
		Request(const Request &src);
		~Request();

		Request&	operator=(const Request &rhs);
		void		readBuffer();

		// getters
		std::string							getBuffer() const;
		std::string							getHeader() const;
		std::string							getBody() const;
		std::string							getMethod() const;
		std::string							getTarget() const;
		std::string							getProtocol() const;
		std::map<std::string, std::string>	getHeaderFields() const;
		std::string							getPath() const;
		std::string							getResponse() const;
		int									getReturnStatus() const;
};
}