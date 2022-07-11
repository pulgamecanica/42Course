#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

# define AUTOINDEX			1
# define CGI				2
# define CGIBIN				3
# define CLIENTMAXBODYSIZE	4
# define ERRORPAGE			5
# define LIMITMETHODS		6
# define LISTEN				7
# define LOCATION			8
# define ROOT				9
# define SERVERNAME			10
# define UPLOAD				11
# define INDEX				12
# define RETURN_D			13
# define SERVER_CONTEXT_DIRECTIVES		8
# define LOCATION_CONTEXT_DIRECTIVES	8
# define TOTAL_DIRECTIVES	13
# define ALL_ERROR_CODES	40
# define PORT_MAX			65535
# define PORT_MIN			1
# define SEPARATORS			" \t\v\n\r\f"
# define SERVER_CONTEXT		1
# define LOCATION_CONTEXT	2

#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <map>

#include <stdlib.h>
#include "webserv.hpp"

/**************************Config***+***************************/
/*                          -----                              */
/* Config represents the whole configuration file              */
/* + Config custom exceptions                                  */
/*  - File is invalid                                          */
/*  - Configuration file syntax is invalid                     */
/*  - Wrong Directive / Wrong Scope                            */
/*                          -----                              */
/**************************ServerConfig*************************/
/*                          -----                              */
/* ServerConfig is an object to represent the server Directive */
/* We decided not to represent this as a directive but as      */
/* a parent to all directives inside it's scope                */
/*                          -----                              */
/**************************Directive****************************/
/*                          -----                              */
/* Abstract Class Directive                                    */
/* This class is usefull because it represents the concept of  */
/* a directive and we can treat all directives as one abstract */
/* class                                                       */
/*                          -----                              */
/***************************************************************/
class Config {
	public:
		class InvalidDirectiveException: public std::exception {
			public:
				virtual const char * what() const throw();
		};
		class WrongSyntaxException: public std::exception {
			public:
				virtual const char * what() const throw();
		};
		class InvalidConfigurationFileException: public std::exception {
			public:
				virtual const char * what() const throw();
		};
		class ServerConfig {
			public:
				class Directive {
					public:
						Directive(int);
						int	getId() const;
						virtual ~Directive();
						virtual void	setDirective(ServerConfig &, int) const = 0;
					private:
						int	_id;
						Directive();
				};
				class AutoIndex: public Directive {
					public:
						AutoIndex(const std::string &) throw (std::exception);
						virtual ~AutoIndex();
						virtual void	setDirective(ServerConfig &, int) const;	
					private:
						bool _option;
				};
 				class Cgi : public Directive {
					public:
						Cgi(const std::string &) throw (std::exception);
						virtual ~Cgi();
						virtual void setDirective(ServerConfig &, int) const;
					private:
						std::vector<std::string> _cgi;
						void _parseCgiContent(std::vector<std::string> &, const std::string&);
				};
				class CgiBin: public Directive {
					public:
						CgiBin(const std::string &) throw (std::exception);
						virtual ~CgiBin();
						virtual void setDirective(ServerConfig &, int) const;
					private:
						std::string	_path;
				};
				class ClientMaxBodySize: public Directive {
					public:
						ClientMaxBodySize(const std::string &) throw (std::exception);
						virtual ~ClientMaxBodySize();
						virtual void	setDirective(ServerConfig &, int) const;
					private:
						int	_max_size;
				};
                class ErrorCodePage: public Directive {
	                public:
	                    ErrorCodePage(const std::string &) throw (std::exception);
	                    virtual ~ErrorCodePage();
						virtual void	setDirective(ServerConfig &, int) const;
	                	static const int	_allErrorCodes[ALL_ERROR_CODES];
	                private:
	                    std::vector<int>	_error_codes;
	                    std::string			_error_path;
	                    bool	loadErrorCodes(const std::string &);
	                    ErrorCodePage();
                };
				class Index: public Directive {
					public:
						Index(const std::string &) throw (std::exception);
						virtual ~Index();
						virtual void	setDirective(ServerConfig &, int) const;
					private:
						std::vector<std::string> _indexes;
						Index();
				};
				class Methods: public Directive {
					public:
						Methods(const std::string &) throw (std::exception);
						virtual ~Methods();
						virtual void	setDirective(ServerConfig &, int) const;
						static const std::string	_valid_methods[4];
					private:
						std::vector<std::string>	_methods;
						bool	_validMethod(const std::string &);
						Methods();
				};
                class Listen: public Directive {
	                public:
	                    Listen(const std::string &) throw (std::exception);
	                    virtual ~Listen();
						virtual void	setDirective(ServerConfig &, int) const;
	                private:
	                    std::string	_ip;
	                    int			_port;
	                    bool	isIpValid(const std::string &);
						Listen();
				};
                class Location: public Directive {
	                public:
                    	Location (std::string const &) throw (std::exception);
                    	virtual ~Location();
	                    virtual void	setDirective(ServerConfig &, int) const;
						bool findMethod(std::string const &) const;
						std::vector<std::string>	_methods;
						std::string					_root_path;
						std::string					_target;
						std::vector<std::string>	_indexes;
						int							_max_body_size;
						bool						_autoindex;
						std::string					_cgi_bin;
						std::map<std::string, std::vector<std::string> > _cgi_map;
						std::map<std::string, std::vector<int> > _location_errors_map;
					private:
						Location();
                };
				class Root: public Directive {
					public:
						Root(const std::string &) throw (std::exception);
						virtual ~Root();
						virtual void	setDirective(ServerConfig &, int) const;
					private:
						const std::string &	_path;
						Root();
				};
				class ServerName: public Directive {
					public:
						ServerName(const std::string &) throw (std::exception);
						virtual ~ServerName();
						virtual void	setDirective(ServerConfig &, int) const;
					private:
						std::vector<std::string>	_server_names;
						ServerName();
				};
				ServerConfig();
				~ServerConfig();
				Location *			findLocation(std::string const &) const;
				int					getPort() const;
				std::string const &	getIp() const;
				// std::string & getRoot();
				// std::vector<int> & getErrorCodes();
				// std::string & getErrorPath();
				// std::vector<std::string>& getMethods();
				// std::string& getLocation(int index);
				// std::vector<Location> & getLocations();
				bool						_autoindex;
				int							_max_body_size;
				std::string					_ip;
                int							_port;
				std::string                 _root_path;
                std::vector<Location>		_locations;
				std::vector<std::string>	_indexes;
				std::vector<std::string>	_names;
				std::map<std::string, std::vector<int> > _server_errors_map;
				static const std::string	_valid_methods_server[3];
		};
	public:
		Config(std::string const &) throw(std::exception);
		~Config();
		std::vector<ServerConfig>	_servers;
	private:
		static const std::string	_server_directives[SERVER_CONTEXT_DIRECTIVES];
		static const std::string	_location_directives[LOCATION_CONTEXT_DIRECTIVES];
		bool								validDirective(const std::string &, const std::string *, int len) const;
		Config::ServerConfig::Directive *	createDirective(std::string const &, std::string const &) throw(std::exception);
		Config();
		Config(const Config &);
};
std::ostream&	operator<<(std::ostream &, const Config &);
std::ostream&	operator<<(std::ostream &, const Config::ServerConfig &);
#endif
