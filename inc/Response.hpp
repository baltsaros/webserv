#pragma once

# include "Server.hpp"

namespace ws {
class Response {
	private:
		std::string		_msg;
		std::string		_response;
		int				_ret;
		Configuration	_config;
		Request			_req;

	public:
		Response();
		Response(const Request &req, const Configuration &config);
		Response(const Response &src);
		~Response();

		Response&	operator=(const Response &rhs);
		void		createResponse();
		void		checkRequest();

		std::string	getMsg() const;
		std::string	getResponse() const;
		int			getRet() const;
		ws::Request	getRequest() const;
};
}