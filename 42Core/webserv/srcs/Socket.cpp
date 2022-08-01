#include "Socket.hpp"

const char * Socket::CantBindException::what() const throw() {return ("Cannot bind socket");}
const char * Socket::SocketFdException::what() const throw() {return ("Socket failed to return a valid fd");}
const char * Socket::InvalidIpAddressException::what() const throw() {return ("Ip Address is not valid");}
const char * Socket::CantListenException::what() const throw() {return ("Listen failed");}

Socket::Socket(const std::string & ip, int port):
       _port(port), _ip_address(ip) {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG) {
        std::cout << PURPLE << *this << ENDC << std::endl;
    }
	if ((_socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) == 0)
        throw e_sfd;
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = inet_addr(_ip_address.c_str());
    if (_address.sin_addr.s_addr == INADDR_NONE)
    	throw e_ip_addr;
    _address.sin_port = htons(_port);
    _addrlen = sizeof(_address);
    bzero(_address.sin_zero, sizeof(_address.sin_zero));
    int opt = 1;
    if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw e_bind;
    if (bind(_socket_fd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
        throw e_bind;
    if (listen(_socket_fd, 10) < 0)
        throw e_listen;
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << WHITE << "Socket created" << ENDC << std::endl;
}

Socket::~Socket() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << RED << "Socket [" << _ip_address << ":" << _port <<"] destroyed" << ENDC << std::endl;
}

Socket::Socket(const Socket & s) {
    _socket_fd = s._socket_fd;
    _addrlen = s._addrlen;
    _port = s._port;
    _ip_address = s._ip_address;
    _address = s._address;
    _conf_servers = s._conf_servers;
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << WHITE << "Copy Socket created" << ENDC << std::endl;
}

int Socket::getPort() const {
	return (_port);
}

int Socket::getSocketFd() const {
	return (_socket_fd);
}
int Socket::getAddressLen() const {
	return (_addrlen);
}
const std::string & Socket::getIpAddress() const {
	return (_ip_address);
}
struct sockaddr_in Socket::getAddress() const {
	return (_address);
}

Config::ServerConfig const & Socket::getServerConfig(std::string const & line) const {
    std::string method;
    std::string target;
    Config::ServerConfig::Location * loc;
    std::vector<Config::ServerConfig>::const_iterator it;

    method = line.substr(0, line.find_first_of(" \r\t"));
    target = line.substr(method.length(), line.find_last_of(" \r\t?#") - method.length());
    target = strtrim(target, " \r\t");
    for (it = _conf_servers.begin(); it != _conf_servers.end(); ++it) {
        loc = it->findLocation(target);
        if (!loc)
            continue ;
        delete (loc);
        return (*it);
    }
    return (_conf_servers.front());
}

void Socket::addServerConf(Config::ServerConfig & sc) {
    _conf_servers.push_back(sc);
}

int Socket::acceptConnection() {
	return (accept(_socket_fd, (struct sockaddr *) & _address, (socklen_t *)& _addrlen));
}

std::ostream& operator<<(std::ostream& s, const Socket& param) {
	s << "http://" << param.getIpAddress() << ":" << param.getPort() << ENDC;
	return (s);
}
