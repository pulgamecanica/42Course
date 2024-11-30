#include "client.hpp"
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

Client::Client() : sock_fd_(-1), is_connected_(false) {}

Client::~Client() {
  if (is_connected_) {
    disconnect();
  }
}

// https://www.beej.us/guide/bgnet/html/#client-server-background
void Client::connect(const std::string& address, const size_t& port) {
  if (is_connected_) {
    throw std::runtime_error("Already connected.");
  }

  int yes = 1;
  struct addrinfo hints, *server_info, *p;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;        // Allow IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;   // TCP stream sockets

  // Convert port to string
  std::string port_str = std::to_string(port);

  // Resolve the address (supports both IP and hostname)
  int status = getaddrinfo(address.c_str(), port_str.c_str(), &hints, &server_info);
  if (status != 0) {
    throw std::runtime_error(std::string("getaddrinfo error: ") + gai_strerror(status));
  }

  // Loop through results and connect to the first valid one
  for (p = server_info; p != nullptr && sock_fd_ == -1; p = p->ai_next) {
    // Create socket
    sock_fd_ = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sock_fd_ == -1) {
      continue;  // Try the next address
    }

    // Set options (ease of use, not wait for reuse)
    if (setsockopt(sock_fd_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      throw std::runtime_error("Failed to set socket options");
    }

    // Attempt to connect
    if (::connect(sock_fd_, p->ai_addr, p->ai_addrlen) == -1) {
      close(sock_fd_);
      sock_fd_ = -1;
      continue;  // Try the next address
    }
    break; // Successfully connected
  }

  if (p == nullptr) {  // No valid addresses found
    freeaddrinfo(server_info);
    throw std::runtime_error("Failed to connect to the server. Make sure the server is running.");
  }

  freeaddrinfo(server_info);  // Free address info
  
  int flags = fcntl(sock_fd_, F_GETFL, 0);
  if (flags == -1 || fcntl(sock_fd_, F_SETFL, flags | O_NONBLOCK) == -1) {
    close(sock_fd_);
    sock_fd_ = -1;
    throw std::runtime_error("Failed to set socket to non-blocking mode");
  }

  is_connected_ = true;
  std::cout << "Connected to " << address << " on port " << port << ".\n";
}

void Client::disconnect() {
  if (!is_connected_) {
    throw std::runtime_error("Not connected.");
  }
  if (sock_fd_ != -1) {
    close(sock_fd_);
  }
  sock_fd_ = -1;
  is_connected_ = false;
  std::cout << "Disconnected successfully.\n";
}

void Client::defineAction(const Message::Type& messageType,
                          const std::function<void(const Message&)>& action) {
  message_observer_.subscribe(messageType, action);
}

void Client::defineAction(const int messageType,
                          const std::function<void(const Message&)>& action) {
  defineAction(static_cast<Message::Type>(messageType), action);
}

void Client::send(const Message& message) {
  if (!is_connected_) {
    throw std::runtime_error("send(). Server is not connected\n");
  }
  auto serialized_data = message.getSerializedData();
  if (::send(sock_fd_, serialized_data.data(), serialized_data.size(), 0) < 0) {
    throw std::runtime_error("Failed to send message");
  }
}

void Client::update() {
  if (!is_connected_) {
    throw std::runtime_error("update(). Not connected to a server.");
  }

  constexpr size_t buffer_size = 4096; // Arbitrary buffer size
  std::vector<uint8_t> temp_buffer(buffer_size);
  ssize_t bytes_received = 0;

  // Non-blocking receive loop
  bytes_received = recv(sock_fd_, temp_buffer.data(), buffer_size, 0);

  // If recv returns -1 and the error is not EAGAIN or EWOULDBLOCK, it indicates a failure
  if (bytes_received < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
    throw std::runtime_error("Failed to receive data from server.");
  }

  if (bytes_received == 0) {
    disconnect();
    throw std::runtime_error("Server disconnected from client.");
  }

  if (bytes_received == -1) {
    return ;
  }

  try {
    std::vector<Message> messages = Message::deserializeMessages(temp_buffer);
    for (const auto& msg: messages) {
      message_observer_.notify(static_cast<Message::Type>(msg.type()), msg);
    }
  } catch (const std::exception& ex) {
    std::cerr << "Error processing message: " << ex.what() << std::endl;
  }
}
