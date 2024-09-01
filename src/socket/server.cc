#include "server.h"

#include "../common/constants.h"

Server::Server(int port) : port(port) {
  // Create a socket
  serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) {
    throw std::runtime_error("Error: Unable to create socket.");
  }

  sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(port);

  // lose the pesky "Address already in use" error message.
  int yes = 1;
  if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) ==
      -1) {
    throw std::runtime_error("Error: setsockopt");
  }

  // Bind the socket to the port.
  if (bind(serverSocket, (struct sockaddr *)&server_address,
           sizeof(server_address)) < 0) {
    close(serverSocket);
    throw std::runtime_error("Error: Unable to bind to port 8080.");
  }
}

void Server::serve() {
  // Listen for incoming connections
  if (listen(serverSocket, BACKLOG) < 0) {
    close(serverSocket);
    throw std::runtime_error("Error: Unable to listen on socket.");
  }

  std::cout << "Server is listening on port " << port << "..." << std::endl;

  while (true) {
    // Accept incoming connection
    sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket =
        accept(serverSocket, (struct sockaddr *)&clientAddress,
               &clientAddressLength);
    if (clientSocket < 0) {
      close(serverSocket);
      throw std::runtime_error("Error: Unable to accept incoming connection.");
    }

    // Handle the request
    handleRequest(clientSocket);
  }
}

// Function to handle incoming requests
void Server::handleRequest(int clientSocket) {
  // Read the request.
  char buffer[1024];
  int bytesRead = read(clientSocket, buffer, sizeof(buffer));
  if (bytesRead > 0) {
    // Print the received HTTP request
    std::cout << "Received HTTP request:\n" << buffer << std::endl;

    // Send the HTTP response.
    // Used for testing the server, will be cleaned up to just maintain an
    // active connection in the upcoming changes.
    std::string response =
        "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!\r\n";
    write(clientSocket, response.c_str(), response.length());
  }
  // Close the client socket
  close(clientSocket);
}