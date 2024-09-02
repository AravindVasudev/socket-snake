#include "server.h"

#include "../common/constants.h"

Server::Server(int port) : port(port) {
  // Create a socket
  serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) {
    throw std::runtime_error("Error: Unable to create socket.");
  }

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(port);

  // lose the pesky "Address already in use" error message.
  int yes = 1;
  if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) ==
      -1) {
    throw std::runtime_error("Error: setsockopt");
  }

  // Bind the socket to the port.
  if (bind(serverSocket, (struct sockaddr *)&serverAddress,
           sizeof(serverAddress)) < 0) {
    close(serverSocket);
    throw std::runtime_error("Error: Unable to bind to port 8080.");
  }
}

Server::~Server() {
  close(clientSocket);
  close(serverSocket);
}

void Server::serve() {
  // Listen for incoming connections
  if (listen(serverSocket, BACKLOG) < 0) {
    close(serverSocket);
    throw std::runtime_error("Error: Unable to listen on socket.");
  }

  std::cout << "Server is listening on port " << port << "..." << std::endl;
  std::cout << "Waiting for the client to connect..." << std::endl;

  // Block until the client joins.
  sockaddr_in clientAddress;
  socklen_t clientAddressLength = sizeof(clientAddress);
  clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress,
                        &clientAddressLength);
  if (clientSocket < 0) {
    close(serverSocket);
    throw std::runtime_error("Error: Unable to accept incoming connection.");
  }

  // Convert client IP -> c_str for logging.
  char clientAddressStr[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(clientAddress.sin_addr), clientAddressStr,
            INET_ADDRSTRLEN);

  std::cout << "Client " << clientAddressStr << " joined." << std::endl;

  // Start the game!
  startGameplay();
}

// Function to handle incoming requests
void Server::startGameplay() {
  // Init the game.
  Game game(true, clientSocket);
  game.run();
}