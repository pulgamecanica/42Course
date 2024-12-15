/**
 * @file server.hpp
 * @brief Header file for the `Server` class, using tcp protocol.
 */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "message.hpp"
#include "persistent_worker.hpp"
#include "observer.hpp"
#include <unordered_map>
#include <functional>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <set>

/**
 * @class Server
 * @brief Represents a network server capable of managing multiple clients, routing messages, and executing actions.
 */
class Server {
 public:
  /**
   * @brief Default constructor.
   */
  Server();

  /**
   * @brief Destructor, ensures proper cleanup.
   */
  ~Server();

  /**
   * @brief Starts the server at the specified port.
   * @param port The port to listen on.
   * @throw std::runtime_error if the server fails to start.
   */
  void start(const size_t& port);

  /**
   * @brief Subscribes an action to a specific message type.
   * @param messageType The type of message to handle.
   * @param action The action to execute for the given message type.
   */
  void defineAction(const Message::Type& messageType,
                    const std::function<void(long long&, const Message&)>& action);

  void defineAction(const int messageType,
                    const std::function<void(long long&, const Message&)>& action);

  /**
   * @brief Sends a message to a specific client.
   * @param message The message to send.
   * @param clientID The ID of the client to send the message to.
   */
  void sendTo(const Message& message, long long clientID);

  /**
   * @brief Sends a message to multiple clients.
   * @param message The message to send.
   * @param clientIDs A vector of client IDs to send the message to.
   */
  void sendToArray(const Message& message, const std::vector<long long>& clientIDs);

  /**
   * @brief Sends a message to all connected clients.
   * @param message The message to send.
   */
  void sendToAll(const Message& message);

  /**
   * @brief Processes all received messages and executes their associated actions.
   */
  void update();

 private:
  void _acceptLoop();  ///< Thread function for accepting new clients.
  void _clientHandler(long long clientID);  ///< Thread function for handling a specific client.
  void _removeClient(long long clientID);  ///< Thread function for removing existing clients.

  int server_fd_;  ///< Server's listening socket file descriptor.
  std::mutex client_mutex_;  ///< Protects access to the client_sockets_ map.
  std::unordered_map<long long, int> client_sockets_;  ///< Mapping of client IDs to their socket descriptors.
  std::set<long long> connected_clients_;  ///< List of active clients.
  Observer<Message::Type, long long&, const Message&> message_observer_; ///< Observer for message type actions
  PersistentWorker worker_; ///< Worker for accepting connections
  long long id_count_; ///< ID count
};

#endif // SERVER_HPP
