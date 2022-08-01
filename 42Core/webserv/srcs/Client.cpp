#include "Client.hpp"

Client::Client(int fd, Socket & s): _fd(fd), _keep_alive(true), _time_to_die(timestamp_in_ms() + TIME_TO_DIE), _socket(s) {
	if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << WHITE << "Client created" << std::endl;
}

Client::Client(const Client & param): _fd(param._fd), _keep_alive(param._keep_alive), _time_to_die(param._time_to_die), _socket(param._socket) {
	if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << WHITE << "Client Copy created" << std::endl;
}

Client& Client::operator= (const Client& param) {
	_fd = param._fd;
	_socket = param._socket;
	_time_to_die = param._time_to_die;
	_keep_alive = param._keep_alive;
	return (*this);
}

Client::~Client() {
	if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << "Client destroyed" << std::endl;
}

int Client::getFd() const {
	return	(_fd); 
}

Socket const & Client::getSocket() const {
	return	(_socket);
}

void Client::handleRequest() {
	char		buffer[30000] = {0};
	int			valread;
	uint64_t	ms_s;
	std::string	buf;

	ms_s = timestamp_in_ms();
	do {
		valread = read(_fd, buffer, 30000);
		if (valread <= 0)
			break ;
		std::string tmp(buffer, valread);
		buf += tmp;
	} while (valread == 30000); // CHECK FOR EOF... // READ BY CHUNCKS???
	if (buf.length() > 0) { // perhaps don't check this? return 408 if response is empty...?	
		_time_to_die = ms_s + TIME_TO_DIE;
		std::string line(buf.substr(0, buf.find_first_of("\n\r")));
		Request req(buf, _socket.getServerConfig(line));
		Response res(req, _socket.getServerConfig(line));
		std::string response_content(res.createResponse());
		write(_fd, response_content.c_str(), response_content.length());
		_keep_alive = res.getKeepAlive();
		if (DEBUG_MSG || CONSTRUCTORS_DESTRUCTORS_DEBUG)
			std::cout << WHITE << "\n====> Request <====" << std::endl << YELLOW << buf << WHITE << "====>Request Debug Info<====\n" << YELLOW << req << std::endl << std::endl << WHITE << "====> Response <====\n" << YELLOW << response_content.substr(0, 1000) << RED << (response_content.length() > 1000 ? ("...\n[SHOWING 1000 bytes maximum]\n") : "") << ENDC <<std::endl;
		else {
			std::cout << GREEN << " Completed " << res.getStatusCode() << " " << Response::_codeMessage[res.getStatusCode()] << " in " << timestamp_in_ms() - ms_s << "ms " << WHITE " at " << get_local_time() << ENDC;
		}
	} else {
		// RESPONSE 408!!!
		_keep_alive = false;
	}
}

bool Client::keepAlive() const {
	return (_keep_alive);
}

uint64_t const & Client::timeToDie() const {
	return (_time_to_die);
}

std::ostream& operator<<(std::ostream& s, const Client& param) {
	s << param.getFd();
	return (s);
}
