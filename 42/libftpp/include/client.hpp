/**
 * @file client.hpp
 * @brief Header file for the `Client` class, a container for clients.
 */
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "message.hpp"
#include <functional>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <mutex>
#include <string>

/**
 * @class Client
 * @brief Represents a network client capable of connecting to a server,
 *        sending messages, and handling server responses.
 */
class Client {
 public:
  /**
   * @brief Default constructor.
   */
  Client();

  /**
   * @brief Destructor, ensures proper cleanup.
   */
  ~Client();

  /**
   * @brief Connects the client to a specific server.
   * @param address The server address to connect to.
   * @param port The server port to connect to.
   * @throw std::runtime_error if connection fails.
   */
  void connect(const std::string& address, const size_t& port);

  /**
   * @brief Disconnects the client from the server.
   */
  void disconnect();

  /**
   * @brief Subscribes an action to a specific message type.
   * @param messageType The type of message to handle.
   * @param action The action to execute for the given message type.
   */
  void defineAction(const Message::Type& messageType,
                    const std::function<void(const Message&)>& action);

  void defineAction(const int messageType,
                    const std::function<void(const Message&)>& action);

  /**
   * @brief Sends a message to the server.
   * @param message The message to send.
   */
  void send(const Message& message);

  /**
   * @brief Processes all received messages and executes their associated actions.
   */
  void update();

 private:
  int sock_fd_;        // Socket file descriptor
  bool is_connected_;  // Connection status

  std::unordered_map<Message::Type, std::function<void(const Message&)>> actions_;
  std::mutex action_mutex_;  ///< Protects access to the actions map.

  std::vector<uint8_t> incoming_buffer_;  ///< Buffer for receiving data.
  // std::mutex incoming_mutex_;  ///< Protects access to the incoming buffer.
};

#endif  // CLIENT_HPP
