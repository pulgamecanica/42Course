#include "server.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <iostream>

Server::Server() : server_fd_(-1), stop_(false) {}

Server::~Server() {
  stop_ = true;
  for (auto& thread : worker_threads_) {
    if (thread.joinable()) thread.join();
  }
  if (server_fd_ >= 0) ::close(server_fd_);
}

void Server::start(const size_t& port) {
  int yes = 1;
  
  server_fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd_ < 0) {
    throw std::runtime_error("Failed to create socket");
  }

  // Set options (ease of use, not wait for reuse)
  if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    throw std::runtime_error("Failed to set socket options");
  }

  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);

  if (::bind(server_fd_, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    throw std::runtime_error("Bind failed");
  }

  if (::listen(server_fd_, 10) < 0) {
    throw std::runtime_error("Listen failed");
  }

  worker_threads_.emplace_back(&Server::_acceptLoop, this);
}

void Server::defineAction(const Message::Type& messageType,
                          const std::function<void(long long&, const Message&)>& action) {
  std::lock_guard<std::mutex> lock(action_mutex_);
  actions_[messageType] = action;
}

void Server::defineAction(const int messageType,
                          const std::function<void(long long&, const Message&)>& action) {
  defineAction(static_cast<Message::Type>(messageType), action);
}

void Server::sendTo(const Message& message, long long clientID) {
  std::lock_guard<std::mutex> lock(client_mutex_);
  auto it = client_sockets_.find(clientID);
  if (it == client_sockets_.end()) {
    throw std::runtime_error("Client ID not found");
  }

  auto serialized_data = message.getSerializedData();
  if (::send(it->second, serialized_data.data(), serialized_data.size(), 0) < 0) {
    throw std::runtime_error("Failed to send message to client");
  }
}

void Server::sendToArray(const Message& message, const std::vector<long long>& clientIDs) {
  for (long long clientID : clientIDs) {
    try {
      sendTo(message, clientID);
    } catch (...) {
      ThreadSafeIOStream() << "Failed to send to client ID: " << clientID << std::endl;
    }
  }
}

void Server::sendToAll(const Message& message) {
  std::lock_guard<std::mutex> lock(client_mutex_);
  for (const auto& [clientID, socket] : client_sockets_) {
    try {
      auto serialized_data = message.getSerializedData();
      if (::send(socket, serialized_data.data(), serialized_data.size(), 0) < 0) {
        throw std::runtime_error("Failed to send to client");
      }
    } catch (...) {
      ThreadSafeIOStream() << "Failed to send to client ID: " << clientID << std::endl;
    }
  }
}

void Server::update() {
  std::lock_guard<std::mutex> lock(client_mutex_);
  for (long long clientID : connected_clients_) {
    worker_threads_.emplace_back(&Server::_clientHandler, this, clientID);
  }
}

void Server::_acceptLoop() {
  while (!stop_) {
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket = ::accept(server_fd_, (struct sockaddr*)&client_addr, &client_len);

    if (client_socket < 0) {
      if (stop_) break;
      throw std::runtime_error("Failed to accept client connection");
    }

    std::lock_guard<std::mutex> lock(client_mutex_);
    long long clientID = reinterpret_cast<long long>(&client_socket);  // Simplistic ID assignment.
    client_sockets_[clientID] = client_socket;
    connected_clients_.insert(clientID);
  }
}

void Server::_clientHandler(long long clientID) {
  constexpr size_t buffer_size = 4096; // Arbitrary buffer size
  std::vector<uint8_t> temp_buffer(buffer_size);
  ssize_t bytes_received = 0;

  bytes_received = recv(client_sockets_[clientID], temp_buffer.data(), temp_buffer.size(), 0);

  if (bytes_received <= 0) {
    _removeClient(clientID);
    return ;
  }
  try {
    std::vector<Message> messages = Message::deserializeMessages(temp_buffer);

    for (auto msg: messages) {
      std::lock_guard<std::mutex> lock(action_mutex_);
      auto action_iter = actions_.find(static_cast<Message::Type>(msg.type()));
      if (action_iter != actions_.end()) {
        action_iter->second(clientID, msg);
      }
    }
  } catch (const std::exception& ex) {
    std::cerr << "Error processing message: " << ex.what() << "\n"; 
  }
}

void Server::_removeClient(long long clientID) {
  auto client_sock_it = client_sockets_.find(clientID);
  if (client_sock_it == client_sockets_.end())
    throw std::runtime_error("Client ID not found");
  client_sockets_.erase(client_sock_it);

  auto client_it = connected_clients_.find(clientID);
  if (client_it == connected_clients_.end())
    throw std::runtime_error("Client ID not found");
  connected_clients_.erase(client_it);
  
}