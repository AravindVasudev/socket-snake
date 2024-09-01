#include <curses.h>

#include <iostream>

#include "snake/game.h"
#include "socket/server.h"

void help() {
  std::cout << "Error: game must be started with either 'serve' or 'listen' "
               "keyword."
            << std::endl;
  std::cout << "For server, `./snake serve <port>`" << std::endl;
  std::cout << "Eg: ./snake serve 8080" << std::endl;
  std::cout << "For client, /snake listen <host> <port>" << std::endl;
  std::cout << "Eg: ./snake listen 127.0.0.1 8080" << std::endl;
}

int main(int argc, char *argv[]) {
  // Game must be initialized as client/server.
  if (argc < 3) {
    help();
    return 1;
  }

  if (strncmp(argv[1], "serve", 5) == 0) {
    Server server(atoi(argv[2]));  // TODO: Handle malformed input.
    server.serve();

    return 0;
  }

  if (strncmp(argv[1], "listen", 6) == 0) {
    throw std::runtime_error("Unimplemented error");
  }

  // Undefined request.
  help();
  return 1;
}