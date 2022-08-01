#ifndef __HTTPSERVER_HPP__
# define __HTTPSERVER_HPP__

#define MAX_EVENTS 10

#include <iostream>
#include <vector>
#include <map>

#include <sys/epoll.h>
#include <unistd.h>

#include "Config.hpp"
#include "Socket.hpp"
#include "Client.hpp"
#include "webserv.hpp"
#include "utils.hpp"

/*************************HTTPServer****************************/
/* The Engine of the webserv                                   */
/* This object represents an HTTP webserver                    */
/* The Webserver is configurated with a configuration file     */
/* Listens to more than one port                               */
/* The server is non blocking and it bounces propery if needed */
/* epoll function is used to monitor all incomming requests    */
/* The webserver is compatible with Mozila Firefox             */
/* ...                                                         */
/***************************************************************/
class HTTPServer {
	public:
		class AcceptException: public std::exception {
			public:
				virtual const char * what() const throw();
		};
		class EpollAddException: public std::exception {
			public:
				virtual const char * what() const throw();
		};
		class EpollCreateException: public std::exception {
			public:
				virtual const char * what() const throw();
		};
		class EpollDeleteException: public std::exception {
			public:
				virtual const char * what() const throw();
		};
		class EpollWaitException: public std::exception {
			public:
				virtual const char * what() const throw();
		};
		class ReadFdException: public std::exception {
			public:
				virtual const char * what() const throw();
		};
		HTTPServer(std::string const &) throw (std::exception);
		~HTTPServer();
		void	addSocket(Socket &) throw (std::exception);
		void	cleanEpollAndClientsList();
		void	removeClient(std::pair<int, int> &);
		int		getEpollFd() const;
		int		numSockets() const;
		void	run();
	private:
		int									_epollfd;
		std::vector<Socket>					_sockets;
		std::map<int, std::vector<Client> >	_clients;
		Config								_config;
		Socket &	getOrCreateSocket(std::string const &, int);
		void	acceptConnectionAt(int) throw (std::exception); 
		bool	isSocketFd(int);
		void	initMsg(void);
		HTTPServer(const HTTPServer &);
};
std::ostream &	operator<<(std::ostream &, const HTTPServer&);
#endif
