//***************************//
//*Template by pulgamecanica*//
//***************************//

extern "C" {
	#include <unistd.h>
	#include <sys/socket.h>
	#include <string.h>
}

#include "Zappy.hpp"
#include "ZappyClient.hpp"

using namespace Zappy;

ZappyClient::ZappyClient(int fd): fd(fd), role(CLIENT) {
	this->cmd_history = {};
	this->msg = "";
}

ZappyClient::~ZappyClient() {
	if (DEBUG)
		std::cout << "ZappyClient" << " destroyed" << std::endl;
}

int ZappyClient::get_fd() const {
	return (this->fd);
}

bool ZappyClient::msg_is_ready() const {
	return (this->msg.back() == '\n');
};

std::string ZappyClient::get_cmd() const {
	size_t index = this->msg.find(' ');

	if (index != std::string::npos) {
		return this->msg.substr(0, index);
	} else {
		return this->msg.substr(0, this->msg.length() - 1);
	}
}

std::string ZappyClient::get_cmd_opts() const {
	size_t index = this->msg.find(' ');

  if (index != std::string::npos) {
		return this->msg.substr(index + 1);
  } else {
		return "";
  }
}

void ZappyClient::reset_cmd() {
	this->cmd_history.push_back(this->msg);
	this->msg.clear();
}

void ZappyClient::read_msg() {
	ssize_t total_bytes(0);
	ssize_t read_bytes;
	char buf[ZappyClient::BUF_SIZE + 1];

	for(;;) {
		bzero(buf, ZappyClient::BUF_SIZE + 1);
		read_bytes = recv(this->fd, buf, ZappyClient::BUF_SIZE, MSG_DONTWAIT); // man recv(2)
		if (read_bytes == -1) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				break; // Nothing else to read (avoided blocking)
			} else {
				perror("recv()");
				exit(EXIT_FAILURE);
			}
		} else if (read_bytes == 0) {
			this->role = DISCONNECTED;
			return ;
		} else {
			total_bytes += read_bytes;
			this->msg.append(buf, read_bytes);
		}
	}
	if (DEBUG)
		std::cout << *this << " msg [" << YELLOW << total_bytes << ENDC << "bytes]: " << this->msg << std::endl;
}

ClientRole ZappyClient::get_role() const {
	return (this->role);
}

std::string ZappyClient::get_role_str() const {
	return (client_role_str_map[this->role]); 
}

bool ZappyClient::client_should_disconnect() const {
	return (this->role == DISCONNECTED);
}

std::ostream& Zappy::operator<<(std::ostream& s, const ZappyClient& param) {
	s << "[" << BLUE << param.get_fd() << ENDC << "] (" << BLUE << param.get_role_str() << ENDC <<")";
	return (s);
}



