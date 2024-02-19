//***************************//
//*Template by pulgamecanica*//
//***************************//

extern "C" {
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <cstdlib>
	#include <unistd.h>
	#include <cerrno>
	#include <string.h>
	#include <fcntl.h>
}

#include "ZappyServer.hpp"
#include <algorithm>

using namespace Zappy;

const std::vector<std::string> ZappyServer::CLIENT_CMDS = {"help","join","spectate","status","exit"};
const std::vector<std::string> ZappyServer::CLIENT_CMDS_OPTS = {"","team name", "", "", ""};
const std::vector<std::string> ZappyServer::CLIENT_CMDS_DESC = {
	"\
Display info about the client the commands and server options",

	"\
Clients can use it to join a game, they must provide the name of the team they wish to join\n\
\t---------\n\
\tErrors\n\
\t- Team does not exist ~> [TEAM NOT FOUND]\n\
\t- Team is full, this means that the maximum players allowed on the team was reached ~> [TEAM FULL]",

	"\
Clients can use it to spectate a game, the user will imediately subscribe to the spectator list\n\
Spectators will receive the game data via compressed binary packages\n\
The game data will be updated according to the ms (ping) of the client, this will depend on the client capabilities",

"\
Clients can request at any time the status of the server\n\
\t---------\n\
\tStatus\n\
\tlatency of the server in ms (micro seconds)\n\
\tNumber of connected clients\n\
\tNumber of players\n\
\tNumber of spectators\n\
\tTeams ~> for each team the statistics\n\
\tTimestamp (server life)",

"Leave the server"};

const std::string ZappyServer::WELCOME_MSG = "\
Welcome to ZappyServer, you have the following command available:\n\
--------------------\n\
| help             |\n\
| join <team_name> |\n\
| spectate         |\n\
| status           |\n\
| exit             |\n\
--------------------\n";


ZappyServer::ZappyServer(int ip, int port): ip(ip), port(port) {
	std::cout << YELLOW << "=> Booting ZappyServer" << std::endl;
  std::cout << "=> ZappyServer " << ZappyServer::ZAPPY_VERSION << " application starting in " << game_mode << " mode" << std::endl;
  std::cout << "=> Run `./Zappy --help` for more startup options" << ENDC << std::endl;
  // Create a socket (IPv4, TCP man socket)
  this->fd = socket(AF_INET, SOCK_STREAM, 0);
  if (this->fd == -1) {
    std::cout << RED << "Failed to create socket RIP\tERROR: " << strerror(errno) << ENDC << std::endl;
    exit(EXIT_FAILURE);
  }
  // int enable = 1;
  // if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) ==
  //     -1) {
  //   perror("setsockopt()");
  //   return 1;
  // }
  std::cout << BLUE << "Zappy listening in single thread mode" << std::endl;
  std::cout << "* C++ version: 20" << std::endl;
  std::cout << "* Environment: " << game_mode << std::endl;
  std::cout << "* PID:\t" << getpid() << ENDC << std::endl;
  // Listen to port on any eventual address
  this->sockaddr.sin_family = AF_INET;
  this->sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  this->sockaddr.sin_port = htons(port); // htons is necessary to convert a number to network byte order
  if (bind(this->fd, (struct sockaddr*)&this->sockaddr, sizeof(this->sockaddr)) < 0) {
    std::cout << RED << "Failed to bind to port (" << BLUE << port << RED << ") RIP\tERROR: " << strerror(errno) << ENDC << std::endl;
    exit(EXIT_FAILURE);
  }
  // Make non blocking
	if (set_nonblocking(this->fd) == -1) {
    std::cerr << RED << "Failed to set non-blocking fd RIP" << ENDC << std::endl;
		exit(EXIT_FAILURE);
	}
  // Start listening.
  if (listen(this->fd, 10) < 0) {
    std::cerr << RED << "Failed to listen on socket RIP\tERROR: " << strerror(errno) << ENDC << std::endl;
    exit(EXIT_FAILURE);
  }
	this->epoll_fd = epoll_create1(0);
	if (this->epoll_fd == -1) {
		perror("epoll_create1");
		exit(EXIT_FAILURE);
	}
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = this->fd;
	if (epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, this->fd, &ev) == -1) {
		perror("epoll_ctl: listen_sock");
		exit(EXIT_FAILURE);
	}
  std::cout << BLUE << "* Epoll listening on http://" << "127.0.0.1" << ":" << port << ENDC << std::endl;
}

void ZappyServer::broadcast_to_client(int fd, const std::string & str) {
	std::string msg_to_broadcast;

	msg_to_broadcast.append(GREEN);
	msg_to_broadcast.append("Server");
	msg_to_broadcast.append(ENDC);
	msg_to_broadcast.append(" : ");
	msg_to_broadcast.append(str);
	msg_to_broadcast.append("\n$> ");

	send(fd, msg_to_broadcast.c_str(), msg_to_broadcast.length(), MSG_DONTWAIT);
}

// Set non blocking socket client
void ZappyServer::add_client(int fd) {
	struct epoll_event ev;

	if (set_nonblocking(fd) == -1) {
		std::cerr << RED << "Failed to add client, couldn't set non-blocking fd" << ENDC << std::endl;
		close(fd);
		return ;
	}
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = fd;
	if (epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		perror("epoll_ctl: fd");
		exit(EXIT_FAILURE);
	}
	clients.insert(std::pair<int, ZappyClient>(fd, fd));
	std::cout << GREEN << "Server" << ENDC << " : " << clients.at(fd) << " joined" << std::endl;
	this->broadcast_to_client(fd, "\
Welcome to ZappyServer, you have the following command available:\n\
--------------------\n\
| help             |\n\
| join <team_name> |\n\
| spectate         |\n\
| status           |\n\
| exit             |\n\
--------------------\n");
}

int ZappyServer::accept_client() {
	int conn_fd;
	socklen_t addrlen;

	addrlen = sizeof(this->sockaddr);
	conn_fd = accept(this->fd, 
		(struct sockaddr *) &this->sockaddr, &addrlen);
	if (conn_fd == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
    perror("accept()");
    exit(EXIT_FAILURE);
  }
	return (conn_fd);
}

void ZappyServer::remove_client(int fd) {
	// Check role and remove from role list as well
	clients.erase(fd);
	close(fd);
}

void ZappyServer::help_client(ZappyClient & zc) {
	(void)zc;
	std::string help_msg;
	int client_cmds_len;

	client_cmds_len = ZappyServer::CLIENT_CMDS.size();
	help_msg = "<help>\nCMD OPTIONS DESCRIPTION\n--- ------- -----------\n";
	for (int i = 0; i < client_cmds_len; ++i) {
		std::string cmd = ZappyServer::CLIENT_CMDS[i];
		std::string cmd_opts = ZappyServer::CLIENT_CMDS_OPTS[i];
		if (cmd_opts.empty())
			cmd_opts = "(X)";
		std::string cmd_desc = ZappyServer::CLIENT_CMDS_DESC[i];
		help_msg.append(BLUE);
		help_msg.append(cmd);
		help_msg.append(" " + cmd_opts);
		help_msg.append(YELLOW);
		help_msg.append(" " + cmd_desc);
		help_msg.append(ENDC);
		help_msg.append("\n");
	}
	broadcast_to_client(zc.get_fd(), help_msg);
}

void ZappyServer::handle_client_cmd(ZappyClient & zc) {
	std::string cmd;
	std::string cmd_opts;

	cmd = zc.get_cmd();
	cmd_opts = zc.get_cmd_opts();

	if (std::find(CLIENT_CMDS.begin(), CLIENT_CMDS.end(),
		zc.get_cmd()) != std::end(CLIENT_CMDS)) {
		if (cmd == "help")
			help_client(zc);
		else if (cmd == "exit") {
		 	std::cout << GREEN << "Server" << ENDC << " : " << zc << " exit" << std::endl;
			broadcast_to_client(zc.get_fd(), "Thank you for using the Zappy Server see you soon!\n");
			this->remove_client(zc.get_fd());
			return;
		}
		// else if (cmd == "join")
		// 	;
		// else if (cmd == "spectate")
		// 	;
		// else if (cmd == "status")
		// 	;
	} else {
		std::cout << GREEN << "Server" << ENDC << " : " << zc << " : [" << RED << "Error - invalid command" << " : " << zc.get_cmd() << ENDC << "]" << std::endl;
		broadcast_to_client(zc.get_fd(), "Error, '" + zc.get_cmd() + "' is not a valid command");
	}
	zc.reset_cmd();
}

void ZappyServer::run(int * signal) {
	// Epoll
	int nfds;
	int conn_fd;

	while (*signal <= 0) {
		if (DEBUG)
			std::cout << "Total clients: " << this->clients.size() << std::endl;
		nfds = epoll_wait(this->epoll_fd, this->events, ZappyServer::MAX_EVENTS, -1);
		if (nfds == -1) {
			perror("epoll_wait");
			break ;
		}
		for (int n = 0; n < nfds; ++n) {
			if ((events[n].events & EPOLLERR) || (events[n].events & EPOLLHUP) ||
          (!(events[n].events & EPOLLIN))) {
        std::cerr << RED << "epoll error " << ENDC << std::endl;        
        // close(events[n].data.fd); // Close fd in case of error? Perhaps..
        continue;
      }
			if (this->events[n].data.fd == this->fd) { // Something with the server fd
				for (;;) {
					conn_fd = accept_client();
					if (conn_fd == -1)
						break; // we processed all of the connections
					std::cout << GREEN << "Server" << ENDC " : " << "Request to connect accepted" << std::endl;
					this->add_client(conn_fd);
        }
		 	} else { // some client wants something
		 		ZappyClient & zc = clients.at(this->events[n].data.fd);
		    // do_use_fd(this->events[n].data.fd);
		 		zc.read_msg();
		    if (zc.client_should_disconnect()) {
		 			std::cout << GREEN << "Server" << ENDC << " : " << zc << std::endl;
		    	this->remove_client(zc.get_fd());
		    	continue;
		    }
		    if (zc.get_role() == CLIENT) {
		 			std::cout << GREEN << "Server" << ENDC << " : " << zc << " : [Recv msg]" << std::endl;
		 			if (zc.msg_is_ready())
		 				this->handle_client_cmd(zc);
		    } else if (zc.get_role() == PLAYER) {
		    	;
		    }
		 	}
		}
	}
}

ZappyServer::~ZappyServer() {
  close(this->fd);
  this->clients.clear();
  if (DEBUG)
		std::cout << "ZappyServer" << " destroyed" << std::endl;
}

std::ostream& Zappy::operator<<(std::ostream& os, const ZappyServer& sz) {
	(void)sz;

	os << ZappyServer::ZAPPY_VERSION;
	return (os);
}
