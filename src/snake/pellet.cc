#include "pellet.h"

#include <cstdlib>

#include "color.h"

void Pellet::move() {
  position.X = (rand() % WIDTH) + 1;
  position.Y = (rand() % HEIGHT) + 1;
}

void Pellet::draw(WINDOW* window) {
  // Switch between colors.
  Color color = blink ? Color::PelletBlinkC : Color::PelletDefaultC;
  blink = !blink;

  wattron(window, COLOR_PAIR(color));
  mvwprintw(window, position.Y, position.X, "*");
  wattroff(window, COLOR_PAIR(color));
}