#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../snake/game.h"

class Client {
private:
  // Server address is read from cmd args, which is a c_str, and passed to
  // sockaddr_in, which also takes c_str. No point in converting to C++
  // string in-between.
  char *serverAddress;
  int serverPort, clientSocket;
  Game game;

public:
  Client(char *serverAddress, int serverPort);
  ~Client();
  void startGameplay();
};