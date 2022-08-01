#include "HTTPServer.hpp"

bool webserv_run = true;

void exit_webserv(int param) {
	(void)param;
	webserv_run = false;
	std::cout << std::endl << YELLOW << "Exit webserv..." << ENDC << std::endl;
}

const char * HTTPServer::AcceptException::what() const throw() {return ("Failed to accept a connection from the Socket");}
const char * HTTPServer::EpollAddException::what() const throw() {return ("Epoll Failed to add a file descriptor");}
const char * HTTPServer::EpollCreateException::what() const throw() {return ("Epoll Failed to return a file descriptor");}
const char * HTTPServer::EpollDeleteException::what() const throw() {return ("Epoll Failed to delete a descriptor");}
const char * HTTPServer::EpollWaitException::what() const throw() {return ("Epoll Failed to wait and return the events");}
const char * HTTPServer::ReadFdException::what() const throw() {return ("Failed to Read the event file descriptor");}

HTTPServer::HTTPServer(std::string const & file) throw (std::exception) : _config(file) {
	_epollfd = epoll_create(10);
	std::vector<Config::ServerConfig>::iterator it;
	std::vector<Config::ServerConfig> servers = _config._servers;
	
	if (_epollfd == -1)
	   throw EpollCreateException();
	for (it = servers.begin(); it != servers.end(); ++it) {
		(void)(*it);
		try {
			getOrCreateSocket(it->getIp(), it->getPort()).addServerConf(*it);
    		if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
				std::cout << std::endl << *it;
		} catch (std::exception & e) {
			std::cout << YELLOW << "[FAILED] " << RED << e.what() << ENDC << std::endl;
			std::cout << YELLOW << "This Server Configuration contains errors, or an invalid [ip_address:port]. Please review the configuration file" << ENDC << std::endl;
		}
	}
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << WHITE << "HTTPServer created" << ENDC << std::endl;
}

Socket & HTTPServer::getOrCreateSocket(std::string const & ip, int port) {
	std::vector<Socket>::iterator it;

	for (it = _sockets.begin(); it != _sockets.end(); it++) {
		if (it->getPort() == port && ip == it->getIpAddress())
			return (*it);
	}
	Socket s(ip, port);
	addSocket(s);
	std::pair<int, std::vector<Client> > p(s.getSocketFd(), std::vector<Client>());
	_clients.insert(p);
	return (_sockets.back());
}

HTTPServer::~HTTPServer() {
	std::vector<Socket>::iterator					it;
	std::vector<Client>::iterator					v_it;
	std::map<int, std::vector<Client> >::iterator	m_it;

	for (it = _sockets.begin(), m_it = _clients.begin(); it != _sockets.end() && m_it != _clients.end(); it++, ++m_it) {
		for (v_it = m_it->second.begin(); v_it != m_it->second.end(); ++v_it) {
			shutdown(v_it->getFd(), SHUT_RDWR);
			close(v_it->getFd());
			std::cout << RED << " * Closing Connection: " << WHITE << v_it->getFd() << ENDC << std::endl;
		}
	}
	for (it = _sockets.begin(); it != _sockets.end(); it++) {
		shutdown(it->getSocketFd(), SHUT_RDWR);
		close(it->getSocketFd());
		std::cout << RED << " * Closing Socket: " << WHITE << it->getSocketFd() << ENDC << std::endl;
	}
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << RED << "HTTPServer" << " destroyed" << ENDC << std::endl;
}

void HTTPServer::acceptConnectionAt(int fd) throw (std::exception) {
	int								conn_sock;
	struct epoll_event				ev;
	std::vector<Socket>::iterator	it;

	for (it = _sockets.begin(); it != _sockets.end(); it++) {
		if (it->getSocketFd() == fd) {
			conn_sock = (*it).acceptConnection();
			if (conn_sock == -1)
			   throw AcceptException();
			ev.events = EPOLLIN | EPOLLET;
			ev.data.fd = conn_sock;
			if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1)
			   throw EpollAddException();
			break ;
		}
	}
	Client c(conn_sock, *it);
	if (DEBUG_MSG || CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << WHITE << "Connection Established with Client [" << c << "]" << ENDC << std::endl;
	if (_clients.find(fd) != _clients.end())
		_clients.find(fd)->second.push_back(c);
}

void HTTPServer::addSocket(Socket & s) throw (std::exception) {
	struct epoll_event	ev;

	ev.events = EPOLLIN;
	ev.data.fd = s.getSocketFd();
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, s.getSocketFd(), &ev) == -1)
		throw EpollAddException();
	_sockets.push_back(s);
}

int HTTPServer::getEpollFd() const {
	return (_epollfd);
}

bool HTTPServer::isSocketFd(int fd) {
	std::vector<Socket>::iterator it;
	std::vector<Socket>::iterator end = _sockets.end();
	for (it = _sockets.begin(); it != end; it++) {
		if (it->getSocketFd() == fd)
			return (true);
	}
	return (false);
}

int HTTPServer::numSockets() const {
	return (_sockets.size());
}

void HTTPServer::initMsg() {
	std::vector<Socket>::iterator	it;
	std::map<int, std::vector<Client> >::iterator	m_it;

	std::cout << CYAN << "=> Booting webserv" << std::endl;
	std::cout << "=> HTTP server starting" << std::endl;
	std::cout << "=> Run `./webserv server --help` for more startup options" << std::endl;
	std::cout << "Starting with single thread mode..." << std::endl;
	std::cout << "* Version: 1.0 (c++98) (\"TBP's Version\")" << std::endl;
	std::cout << "*          PID: " << getpid() << std::endl;
	for (it = _sockets.begin(), m_it = _clients.begin(); it != _sockets.end() && m_it != _clients.end(); it++, ++m_it)
		std::cout << CYAN << "* Listening on " << *it << " " << PURPLE << m_it->first << " => #Clients: " << m_it->second.size() << ENDC << std::endl;
	std::cout << WHITE << "Use Ctrl-C to stop" << std::endl;
}

void HTTPServer::run() {
	int												n;
	int												nfds;
	struct epoll_event								events[MAX_EVENTS];
	std::vector<Client>::iterator					v_it;
	std::map<int, std::vector<Client> >::iterator	m_it;

	signal(SIGINT, exit_webserv);
	initMsg();
	timestamp_in_ms();
	while (true) {
		nfds = epoll_wait(_epollfd, events, MAX_EVENTS, 2000);
		if (!webserv_run)
			break;
		if (nfds == -1)
		   throw EpollWaitException();
		for (n = 0; n < nfds; ++n) {
			if (isSocketFd(events[n].data.fd)) {
				if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
					std::cout << WHITE << "[" << timestamp_in_ms() << "]" << YELLOW << " ServerSocket Accepting Connection" << ENDC << std::endl;
				acceptConnectionAt(events[n].data.fd);
			} else if (events[n].events == EPOLLIN) {
				if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
					std::cout << BLUE << "EPOLLIN on fd: " << events[n].data.fd << ENDC << std::endl;
				for (m_it = _clients.begin(); m_it != _clients.end(); ++m_it) {
					for (v_it = m_it->second.begin(); v_it != m_it->second.end(); ++v_it) {
						if (v_it->getFd() == events[n].data.fd) {
							if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
								std::cout << WHITE << "[" << timestamp_in_ms() << "] " << YELLOW << "Signal From Client: " << *v_it<< " " << CYAN << "[" << v_it->getSocket() << CYAN << "]" << ENDC << std::endl;		
							v_it->handleRequest();
							break;
					 	}
					}
				}
		   }
		}
		cleanEpollAndClientsList();
	}
}

void HTTPServer::removeClient(std::pair<int, int> & pair) {
	std::map<int, std::vector<Client> >::iterator	m_it;
	std::vector<Client>::iterator					v_it;

	m_it = _clients.find(pair.first);
	if (m_it == _clients.end())
		return ;
	for (v_it = m_it->second.begin(); v_it != m_it->second.end(); ++v_it) {
		if (v_it->getFd() == pair.second) {
			if (DEBUG_MSG || CONSTRUCTORS_DESTRUCTORS_DEBUG)
				std::cout << WHITE << "[" << timestamp_in_ms() << "]" << RED << " Removing Client " << pair.second << " from Socket " << pair.first << ENDC << std::endl;
			epoll_ctl(_epollfd, EPOLL_CTL_DEL, v_it->getFd(), 0);
			shutdown(v_it->getFd(), SHUT_RDWR);
			close(v_it->getFd());
			_clients[pair.first].erase(v_it);
			return ;
		}
	}
}

void HTTPServer::cleanEpollAndClientsList() {
	uint64_t										timestamp;
	std::vector<Client>::iterator					v_it;
	std::vector<std::pair<int, int> >				vi_it;
	std::map<int, std::vector<Client> >::iterator	m_it;

	timestamp = timestamp_in_ms();
	for (m_it = _clients.begin(); m_it != _clients.end() ; ++m_it) {
		for (v_it = m_it->second.begin(); v_it != m_it->second.end(); ++v_it) {
			if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
				std::cout << WHITE << "[" << timestamp << "] " << PURPLE << *v_it << " " << v_it->timeToDie() << ENDC << std::endl;
			if (v_it->timeToDie() < timestamp || !(v_it->keepAlive())) {
				std::pair<int, int> p(m_it->first, v_it->getFd());
				vi_it.push_back(p);
			}
		}
	}
	for (std::vector<std::pair<int, int> >::iterator it = vi_it.begin(); it < vi_it.end(); ++it) {
		removeClient(*it);
	}
}

std::ostream& operator<<(std::ostream& s, const HTTPServer& param) {
	s << "[epoll - " << param.getEpollFd() << "] # of sockets: " << param.numSockets();
	return (s);
}
