#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__

#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <ostream>
#include <string>
#include <limits>
#include <algorithm>

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>

#include "Request.hpp"
#include "webserv.hpp"
#include "utils.hpp"

class Response {
	public:
		class CGIFailure: public std::exception {
			public:
				virtual const char * what() const throw();
		};
		Response(Request const &, Config::ServerConfig const & sc);
		~Response();

		const std::string createResponse(void);
		bool getKeepAlive(void) const;
		int getStatusCode(void) const;
		static std::map<int, std::string> _codeMessage;
		static std::map<std::string, std::string> _mime_types;
	private:
		Response();
		Response(const Response&);
	private:
		int _status_code;
		bool _keep_alive;
		bool _autoindex;
		bool _cgi_response;
		std::string _date;
		std::string _server_name;
		std::string _content_type;
		std::string _content;
		Request const & _req;
		Config::ServerConfig const & _server_config;
		//bool _check_default_error_code_and_assign_path(int);
		//static std::string _mime_type_detector(std::string const & file_name);
		int execCGI(void);
		void setMimeType(std::string const &);
		const std::string	createRedirectionResponse(void);
		const std::string	createAutoindexResponse(void);
		const std::string	deleteResponse(void);
		const std::string	CGIResponse(void);
};

std::ostream&	operator<<(std::ostream&, const Response&);

#endif

