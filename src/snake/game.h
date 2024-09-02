#pragma once

#include <curses.h>

#include "pellet.h"
#include "snake.h"

class Game {
private:
  WINDOW *window; // Game window.
  Snake snake;
  Pellet pellet;
  int score{};

  // The game holds an agnostic TCP socket id which it uses to read it's
  // opponent's move.
  int socket;

  void drawScore();
  void drawGameOver();

public:
  ~Game();
  void init(const int sockfd);
  void run();
};