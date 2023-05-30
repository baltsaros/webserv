#pragma once

# include <string>
# include <iostream>

namespace ws {
class Request {
	private:
		std::string	_buffer;
		std::string	_header;
		std::string	_body;
		std::string	_method;
		std::string	_target;
		std::string	_protocolVersion;
		std::string	_host;
		std::string	_uAgent;
		std::string	_accept;
		int			_errorCode;
		void		_checkPos(size_t pos);
		std::string	_getParam(std::string toGet, size_t offset);
		void		_parseStartingLine();

	public:
		Request();
		Request(std::string buffer);
		Request(Request const &src);
		~Request();

		Request&	operator=(Request const &rhs);
		void	readBuffer();

		// getters
		std::string	getMethod() const;
		std::string	getTarget() const;
		std::string	getProtocol() const;
		std::string	getHost() const;
		std::string	getUAgent() const;
		std::string	getAccept() const;
		int			getErrorCode() const;
};
}