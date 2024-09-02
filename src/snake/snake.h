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
  Point head;
  std::vector<Point> tail;
  Point velocity{-1, 0};

  void drawHead(WINDOW *window);

public:
  Snake(bool isServer);
  void draw(WINDOW *window);
  void input(const int &input);
  MoveState move(const Pellet &pellet, const Snake &other);
};