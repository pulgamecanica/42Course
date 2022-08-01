#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__

#include <iostream>
#include <map>
#include <algorithm>

#include "Config.hpp"
#include "Socket.hpp"
#include "webserv.hpp"
#include "utils.hpp"

class Request {
	private:
		int			_error_code;
	    std::string _method;
	    std::string _uri_target;
		std::string _final_path;
	    std::string _http_version;
	    std::string _query;
	    std::string _content;
		std::map<std::string, std::string> _headers;
		Config::ServerConfig const & _server_config;
		
		bool _check_default_error_code_and_assign_path(int) const;
		Request(const Request &);
		Request&	operator= (const Request&);
		Request();
	public:
		Request(std::string const &, Config::ServerConfig const &);
		~Request();

		int getErrorCode() const;
		bool isTargetDir() const;
		bool isTargetCGI() const;
		bool isTargetRedirect() const;
		std::string const & getFinalPath() const;
		std::string const & getContent() const;
		std::string const & getQuery() const;
		std::string const & getMethod() const;
		std::string const & getUriTarget() const;
		const std::string getIndex() const;
		const std::string getCGIBinPath() const; 
		const std::string getCGIFile() const;
		const std::string getContentType() const;
		const std::string getUserAgent() const;
		const std::string getAcceptedEncoding() const;
		const std::string getCookies() const;
		
		Config::ServerConfig::Location * _loc;

};
std::ostream &	operator<<(std::ostream &, const Request &);
#endif

