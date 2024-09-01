#pragma once

#include <curses.h>

#include "../common/constants.h"
#include "point.h"

class Pellet {
private:
  bool blink{};

public:
  Point position{};

  void move();
  void draw(WINDOW *window);
};