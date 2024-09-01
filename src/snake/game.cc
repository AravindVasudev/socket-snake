#include "game.h"

#include <locale.h>
#include <unistd.h>

#include <chrono>

#include "color.h"
#include "../common/constants.h"

Game::Game() {
  // Init graphics.
  setlocale(LC_ALL, "");  // Get the terminal outta boomer-mode.
  initscr();
  keypad(stdscr, true);
  curs_set(false);
  timeout(0);
  raw();
  noecho();

  // Init Colors.
  start_color();
  init_pair(Color::WindowC, COLOR_BLACK, COLOR_GREEN);
  init_pair(Color::SnakeC, COLOR_BLACK, COLOR_GREEN);
  init_pair(Color::PelletDefaultC, COLOR_WHITE, COLOR_GREEN);
  init_pair(Color::PelletBlinkC, COLOR_RED, COLOR_GREEN);

  // Init game window.
  window = newwin(HEIGHT, WIDTH, 0, 0);

  // Style window.
  wbkgd(window, COLOR_PAIR(Color::WindowC));

  // Randomize pellet position.
  pellet.move();
}

Game::~Game() {
  // Reset timeout and block.
  timeout(-1);
  getch();
  endwin();
}

void Game::drawScore() { mvwprintw(window, 0, 2, "Score: %d", score); }

void Game::drawGameOver() {
  wclear(window);
  box(window, 0, 0);
  // TODO: Fix centering.
  mvwprintw(window, WIDTH / 2 - 3, HEIGHT / 2 - 3, "You lost! Your score: %d", score);
  wrefresh(window);
}

void Game::run() {
  while (true) {
    int input = getch();
    if (input == 'q') {
      break;
    }

    // Reinit frame.
    // Maybe it's cheaper to cleanup only the points where necessary but given
    // this is all running in a terminal, redrawing the whole frame barely
    // costs anything.
    werase(window);
    box(window, 0, 0);

    // Process snake's move.
    snake.input(input);

    // Actually move the snake.
    switch (snake.move(pellet)) {
      case MoveState::DEAD:
        // We dead :(
        drawGameOver();
        return;
      case MoveState::EAT:
        score++;
        pellet.move();  // Relocate the pellet.
        break;
      case MoveState::NOOP:
        break;  //  NOOP
    }

    // Draw all objects.
    snake.draw(window);
    pellet.draw(window);
    drawScore();

    // Refresh frame.
    wrefresh(window);
    usleep(1000000 / FRAME_RATE);
  }
}