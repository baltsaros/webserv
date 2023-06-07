#pragma once

# include <string>
# include <iostream>
# include "Configuration.hpp"
# include "ServerUtils.hpp"

namespace ws {
class Request {
	private:
		std::string			_buffer;
		Configuration		_config;
		std::string			_header;
		std::string			_body;
		std::string			_method;
		std::string			_target;
		std::string			_protocolVersion;
		std::string			_host;
		std::string			_uAgent;
		std::string			_accept;
		std::string			_path;
		std::string			_response;
		int					_errorCode;
		std::map<std::string, ConfigLocation *>	_locations;
		void				_checkPos(size_t pos);
		std::string			_getParam(std::string toGet, size_t offset);
		void				_checkPath();
		void				_parseStartingLine();

	public:
		Request();
		Request(const std::string &buffer, const Configuration &config, std::map<std::string, ConfigLocation*> locations);
		Request(const Request &src);
		~Request();

		Request&	operator=(const Request &rhs);
		void	readBuffer();

		// getters
		std::string	getMethod() const;
		std::string	getTarget() const;
		std::string	getProtocol() const;
		std::string	getHost() const;
		std::string	getUAgent() const;
		std::string	getAccept() const;
		std::string	getPath() const;
		std::string	getResponse() const;
		int			getErrorCode() const;
};
}