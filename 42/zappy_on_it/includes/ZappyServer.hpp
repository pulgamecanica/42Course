//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __ZAPPYSERVER_HPP__
# define __ZAPPYSERVER_HPP__

extern "C" {
	#include <sys/epoll.h>
	#include <netinet/in.h>
}

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "Zappy.hpp"
#include "ZappyClient.hpp"

namespace Zappy {
	class ZappyServer {
		public:
			static constexpr std::string ZAPPY_VERSION = "1.42";
			static constexpr int MAX_EVENTS = 64;
			static const std::vector<std::string> CLIENT_CMDS;
			static const std::vector<std::string> CLIENT_CMDS_OPTS;
			static const std::vector<std::string> CLIENT_CMDS_DESC;
			static const std::string WELCOME_MSG;

			ZappyServer(int ip, int port = 4242);
			~ZappyServer();
			void run(int * signal);
			void remove_client(int fd);
			void add_client(int fd);
			int accept_client();
			void help_client(ZappyClient & zc);
			void handle_client_cmd(ZappyClient & zc);
			void broadcast_to_client(int fd, const std::string & str);
		private:
			const int 					ip;
			const int 					port;
			int 								fd;
			int 								epoll_fd;
  		sockaddr_in					sockaddr;
			struct epoll_event	events[MAX_EVENTS];
			std::map<int, ZappyClient> clients;
	};

	std::ostream&	operator<<(std::ostream& os, const ZappyServer& zs);
}

#endif

