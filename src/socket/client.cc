#include "client.h"

#include <curses.h>

#include <iostream>

#include "../common/constants.h"

Client::Client(char* serverAddress, int serverPort)
    : serverAddress(serverAddress), serverPort(serverPort) {
  // Create a socket
  clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket == -1) {
    throw std::runtime_error("Error: Unable to create socket.");
  }

  sockaddr_in serverAddr;
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(serverPort);
  serverAddr.sin_addr.s_addr = inet_addr(serverAddress);

  if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
    throw std::runtime_error("Error: Cannot connect to the server.");
  }

  std::cout << "Connected to " << serverAddress << ":" << serverPort
            << std::endl;

  // while (true) {
  //   int c;
  //   std::cin >> c;

  //   if (send(clientSocket, &c, sizeof(c), 0) < 0) {
  //     throw std::runtime_error("Error: Cannot send msg to the server.");
  //   }

  //   int bytesRead = recv(clientSocket, &c, sizeof(c), 0);
  //   if (bytesRead > 0) {
  //     std::cout << "Server said: " << c << std::endl;
  //   }

  //   usleep(1000000 / FRAME_RATE);
  // }
}

Client::~Client() { close(clientSocket); }

void Client::startGameplay() {
  // Init the game.
  game.init(clientSocket);
  game.run();
}