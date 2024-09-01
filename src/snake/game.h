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

  // Inspired from cs:go, one instance of the game chooses to be the server
  // and another is the client. Given we're only support two-players and
  // will be keeping a TCP connection open throughout the lifetime of the
  // game, it doesn't really matter who's what post connection.
  // However, for the case of this fork, the game object has to have params
  // for both server and client.
  // Maybe a better approach could be is to keep the game agnostic of the
  // network logic and make both client & server hold a game instance. We init
  //

  void drawScore();
  void drawGameOver();

public:
  Game();
  ~Game();
  void run();
};