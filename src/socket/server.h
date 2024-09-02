#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "../snake/game.h"

class Server {
private:
  int port, serverSocket, clientSocket;

  void startGameplay();

public:
  Server(int port);
  ~Server();
  void serve();
};