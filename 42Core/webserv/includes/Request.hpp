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
		std::map<std::string, std::string> _headers;
		int			_error_code;
	    std::string _method;
	    std::string _uri_target;
		std::string _location_root;
		std::string _final_path;
	    std::string _http_version;
	    std::string _query;
	    std::string _content;
		// std::string _post_content;
	    // std::string _body;
	    // std::string _transfer_encoding;
		Config::ServerConfig const & _server_config;

		std::map<std::string, std::vector<int> > _location_error_codes;
		std::map<std::string, std::vector<int> > _server_error_codes;
		
		bool _check_default_error_code_and_assign_path(int) const;

		// Socket _s;
		Request(const Request &);
		Request&	operator= (const Request&);
		Request();
	public:
		Request(std::string const &, Config::ServerConfig const &);
		~Request();

		int getErrorCode() const;
		bool isTargetDir() const;
		bool isTargetCGI() const;
		std::string const & getFinalPath() const;
		std::string const & getContent() const;
		std::string const & getQuery() const;
		std::string const & getMethod() const;
		std::string const & getUriTarget() const;
		const std::string getCGIBinPath() const; 
		const std::string getCGIFile() const;
		const std::string getContentType() const;
		const std::string getUserAgent() const;
		const std::string getRemoteHost() const;
		const std::string getAcceptedEncoding() const;
		// long const & get_content_length() const; //?????????????
		// std::string const & get_http_version() const;
		// std::string const & get_location_root() const;
		// bool const & get_is_default() const; //????
		// std::string const & get_default_error_path() const ;
		// Config::ServerConfig const & get_server_confing() const;

		Config::ServerConfig::Location * _loc;
		// std::string & get_error_code() const;
		// std::string & get_error_code() const;
		// std::string & get_error_code() const;
		// std::string & get_error_code() const;
		//Socket const & getSocket() const;

	private:
		// Request();
		// Request(const Request&);
		// Request&	operator= (const Request&); // const for safety... not super nesessary

	private:
};

std::ostream &	operator<<(std::ostream &, const Request &);

#endif

