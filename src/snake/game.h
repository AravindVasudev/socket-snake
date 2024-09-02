#pragma once

#include <curses.h>

#include "pellet.h"
#include "snake.h"

class Game {
private:
  // ik ik, this is hacky af. The current design relies on the game knowing
  // who's the server. Maybe a better impl would be to use UDP sockets and
  // have no concept of a server? Making the communication resilient alone is
  // going to take days to implement, so I'm sticking with my shortcut.
  bool isServer;
  WINDOW *window; // Game window.
  Snake player, opponent;
  Pellet pellet;
  int score{};

  // The game holds an agnostic TCP socket id which it uses to read it's
  // opponent's move.
  int socket;

  void drawScore();
  void drawGameOver();

public:
  Game(bool isServer, const int socket);
  ~Game();
  void run();
};