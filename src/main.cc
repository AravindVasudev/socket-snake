#include <curses.h>

#include <iostream>

#include "snake/game.h"
#include "socket/client.h"
#include "socket/server.h"

int help() {
  std::cout << "Error: game must be started with either 'serve' or 'listen' "
               "keyword."
            << std::endl;
  std::cout << "For server, `./snake serve <port>`" << std::endl;
  std::cout << "Eg: ./snake serve 8080" << std::endl;
  std::cout << "For client, /snake connect <host> <port>" << std::endl;
  std::cout << "Eg: ./snake connect 127.0.0.1 8080" << std::endl;

  return 1;
}

int main(int argc, char *argv[]) {
  // Game must be initialized as client/server.
  if (argc < 3) {
    return help();
  }

  if (strncmp(argv[1], "serve", 5) == 0) {
    Server server(atoi(argv[2]));  // TODO: Handle malformed input.
    server.serve();

    return 0;
  }

  if (strncmp(argv[1], "connect", 7) == 0) {
    if (argc != 4) {
      return help();
    }

    Client client(argv[2], atoi(argv[3]));
    return 0;
  }

  // Undefined request.
  return help();
}