#include "SimpleServer.hpp"

#define BUFF_SIZE 100

int main() {
  // Create a socket (IPv4, TCP)
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    std::cout << "Failed to create socket RIP. ERROR: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Listen to port 4242 on any address
  sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = INADDR_ANY;
  sockaddr.sin_port = htons(4242); // htons is necessary to convert a number to network byte order
  if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
    std::cout << "Failed to bind to port 4242 RIP. ERROR: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Start listening.
  if (listen(sockfd, 10) < 0) {
    std::cout << "Failed to listen on socket RIP. ERROR: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << "The server is up and runnig" << std::endl;
  std::cout << "Server IP: " << sockaddr.sin_addr.s_addr << " [version 1.1, *middle-earth*]" << std::endl;
  std::cout << "Listening on port 4242" << std::endl;
  // Grab a connection from the queue
  /*int addrlen = sizeof(sockaddr);
  int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
  if (connection < 0) {
    std::cout << "Failed to grab connection RIP. ERROR: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }*/

  // Read from the connection
  char buffer[BUFF_SIZE];
  while (1) {
    std::cout << "Ready to accept a connection..." << std::endl;
    int addrlen = sizeof(sockaddr);
    int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
    if (connection < 0) {
      std::cout << "Failed to grab connection RIP. ERROR: " << errno << std::endl;
      exit(EXIT_FAILURE);
    }
    std::cout << "Connection established" << std::endl;
    std::cout << "Ready to read whatever this connection want to tell me" << std::endl;
    long int bytesRead = read(connection, buffer, BUFF_SIZE);
    if (bytesRead == -1) {
      std::cout << "Failed to read fd RIP. ERROR: " << errno << std::endl;
      exit(EXIT_FAILURE);
    }
    std::cout << "Message: " << buffer;

    // Send a message to the connection
    std::string response = "Good talking to you\n";
    send(connection, response.c_str(), response.size(), 0);
    if (strcmp(buffer, "bye\n") == 0)
	break ;
    bzero(buffer, 100);
    close(connection);
  }

  // Close the connections
//  close(connection);
  close(sockfd);
}
