#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

#include <iostream>

#include "Socket.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "webserv.hpp"

class Client {
	private:
		int _fd;
		bool _keep_alive;
		uint64_t _time_to_die;
		Socket & _socket;
		Client();
	public:
		Client(int, Socket &);
		Client(const Client &);
		Client & operator= (const Client &);
		~Client();
		int					getFd() const;
		Socket const &		getSocket() const;
		void				handleRequest();
		bool 				keepAlive() const;
		uint64_t const &	timeToDie() const;
};
std::ostream &	operator<<(std::ostream &, const Client &);
#endif
