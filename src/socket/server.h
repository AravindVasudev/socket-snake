#pragma once

#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class Server {
private:
  int serverSocket;
  int port;

  void handleRequest(int clientSocket);

public:
  Server(int port);
  void serve();
};