#include "SimpleServer.hpp"

#define BUFF_SIZE 1000

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
    std::cout << RED << "Failed to bind to port 4242 RIP. ERROR: " << errno << ENDC << std::endl;
    exit(EXIT_FAILURE);
  }

  // Start listening.
  if (listen(sockfd, 10) < 0) {
    std::cout << RED << "Failed to listen on socket RIP. ERROR: " << errno << ENDC << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << BLUE << "The server is up and runnig" << std::endl;
  std::cout << GREEN << "Server IP: " << sockaddr.sin_addr.s_addr << " [version 1.1, *middle-earth*]" << std::endl;
  std::cout << BLUE << "Listening on port 4242" << ENDC << std::endl;

  char buffer[BUFF_SIZE];
  bzero(buffer, BUFF_SIZE);
  while (1) {
    std::cout << GREEN << "Ready to accept a connection..."  << ENDC << std::endl;
    // Accept any connection (BLOCKING FUNCTION)
    int addrlen = sizeof(sockaddr);
    int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
    if (connection < 0) {
      std::cout << RED << "Failed to grab connection RIP. ERROR: " << errno << ENDC << std::endl;
      exit(EXIT_FAILURE);
    }
    std::cout << BLUE << "Connection established" << std::endl;
    std::cout << "Ready to read whatever this connection want to tell me" << ENDC << std::endl;
    // Ready to read from connection established (BLOCKING FUNCTION)
    long int bytesRead = read(connection, buffer, BUFF_SIZE);
    if (bytesRead == -1) {
      std::cout << RED << "Failed to read fd RIP. ERROR: " << errno << ENDC << std::endl;
      exit(EXIT_FAILURE);
    }
    buffer[BUFF_SIZE - 1] = 0;
    std::cout  << WHITE << "Message: " << ENDC << buffer;

    // Send a message to the connection
    std::string response = "Good talking to you\n";
    send(connection, response.c_str(), response.size(), 0);
    if (strcmp(buffer, "bye\n") == 0)
      break ;
    bzero(buffer, BUFF_SIZE);
    // Close connection
    close(connection);
  }

  // Close the socket fd
  close(sockfd);
}
