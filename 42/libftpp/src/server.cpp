// #include "server.hpp"
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <unistd.h>
// #include <stdexcept>
// #include <cstring>
// #include <iostream>

// Server::Server() : server_fd_(-1), stop_(false) {}

// Server::~Server() {
//   stop_ = true;
//   for (auto& thread : worker_threads_) {
//     if (thread.joinable()) thread.join();
//   }
//   if (server_fd_ >= 0) ::close(server_fd_);
// }

// void Server::start(const size_t& port) {
//   server_fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
//   if (server_fd_ < 0) {
//     throw std::runtime_error("Failed to create socket");
//   }

//   sockaddr_in server_addr{};
//   server_addr.sin_family = AF_INET;
//   server_addr.sin_addr.s_addr = INADDR_ANY;
//   server_addr.sin_port = htons(port);

//   if (::bind(server_fd_, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
//     throw std::runtime_error("Bind failed");
//   }

//   if (::listen(server_fd_, 10) < 0) {
//     throw std::runtime_error("Listen failed");
//   }

//   worker_threads_.emplace_back(&Server::_acceptLoop, this);
// }

// void Server::defineAction(const Message::Type& messageType,
//                           const std::function<void(long long&, const Message&)>& action) {
//   std::lock_guard<std::mutex> lock(action_mutex_);
//   actions_[messageType] = action;
// }

// void Server::sendTo(const Message& message, long long clientID) {
//   std::lock_guard<std::mutex> lock(client_mutex_);
//   auto it = client_sockets_.find(clientID);
//   if (it == client_sockets_.end()) {
//     throw std::runtime_error("Client ID not found");
//   }

//   auto serialized_data = message.serialize();
//   if (::send(it->second, serialized_data.data(), serialized_data.size(), 0) < 0) {
//     throw std::runtime_error("Failed to send message to client");
//   }
// }

// void Server::sendToArray(const Message& message, const std::vector<long long>& clientIDs) {
//   for (long long clientID : clientIDs) {
//     try {
//       sendTo(message, clientID);
//     } catch (...) {
//       ThreadSafeIOStream() << "Failed to send to client ID: " << clientID << std::endl;
//     }
//   }
// }

// void Server::sendToAll(const Message& message) {
//   std::lock_guard<std::mutex> lock(client_mutex_);
//   for (const auto& [clientID, socket] : client_sockets_) {
//     try {
//       auto serialized_data = message.serialize();
//       if (::send(socket, serialized_data.data(), serialized_data.size(), 0) < 0) {
//         throw std::runtime_error("Failed to send to client");
//       }
//     } catch (...) {
//       ThreadSafeIOStream() << "Failed to send to client ID: " << clientID << std::endl;
//     }
//   }
// }

// void Server::update() {
//   std::lock_guard<std::mutex> lock(client_mutex_);
//   for (long long clientID : connected_clients_) {
//     worker_threads_.emplace_back(&Server::_clientHandler, this, clientID);
//   }
// }

// void Server::_acceptLoop() {
//   while (!stop_) {
//     sockaddr_in client_addr;
//     socklen_t client_len = sizeof(client_addr);
//     int client_socket = ::accept(server_fd_, (struct sockaddr*)&client_addr, &client_len);

//     if (client_socket < 0) {
//       if (stop_) break;
//       throw std::runtime_error("Failed to accept client connection");
//     }

//     std::lock_guard<std::mutex> lock(client_mutex_);
//     long long clientID = reinterpret_cast<long long>(&client_socket);  // Simplistic ID assignment.
//     client_sockets_[clientID] = client_socket;
//     connected_clients_.insert(clientID);
//   }
// }

// void Server::_clientHandler(long long clientID) {
//   char buffer[1024];
//   while (!stop_) {
//     ssize_t bytes_received = ::recv(client_sockets_[clientID], buffer, sizeof(buffer), 0);
//     if (bytes_received <= 0) {
//       break;
//     }

//     Message message(std::vector<uint8_t>(buffer, buffer + bytes_received));
//     std::lock_guard<std::mutex> lock(action_mutex_);
//     auto action_iter = actions_.find(message.type());
//     if (action_iter != actions_.end()) {
//       action_iter->second(clientID, message);
//     }
//   }
// }
