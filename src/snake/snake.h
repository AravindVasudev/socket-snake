#pragma once

#include <curses.h>

#include <vector>

#include "../common/constants.h"
#include "pellet.h"
#include "point.h"

// Possible outcomes of a move.
enum MoveState {
  NOOP = 1,
  DEAD,
  EAT,
};

class Snake {
private:
  Point head{SNAKE_X, SNAKE_Y};
  std::vector<Point> tail;
  Point velocity{-1, 0};

  void drawHead(WINDOW *window);

public:
  void draw(WINDOW *window);
  void input(const int &input);
  MoveState move(const Pellet &pellet);
};