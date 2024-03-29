#pragma once

# include "Server.hpp"
# include "ServerUtils.hpp"

namespace ws {
class Response {
	private:
		std::string		_msg;
		std::string		_response;
		int				_ret;
		Request			_req;

	public:
		Response();
		Response(const Request &req);
		Response(const Response &src);
		~Response();

		Response&		operator=(const Response &rhs);
		void			createResponse();
		void			checkRequest();
		std::string		createAutoIndex();

		std::string		getMsg() const;
		std::string		getResponse() const;
		int				getRet() const;
		ws::Request		getRequest() const;
};
}