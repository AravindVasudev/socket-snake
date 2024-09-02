#include "game.h"

#include <arpa/inet.h>
#include <locale.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <iostream>

#include "../common/constants.h"
#include "color.h"

Game::Game(bool isServer)
    : isServer(isServer), player(isServer), opponent(isServer) {};

// The original implementation does all the init stuff in the constructor.
// Given we have to wait to establish the connect before starting the game,
// Moved it to a separate fn. Both the server & the client would keep a game
// object stack initialized & hence will be constructed right away once the
// server starts and hence this workaround.
// TODO: Add a game state to ensure run() cannot be called before init().
void Game::init(const int sockfd) {
  // Init opponent's socket.
  socket = sockfd;

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
  mvwprintw(window, WIDTH / 2 - 3, HEIGHT / 2 - 3, "You lost! Your score: %d",
            score);
  wrefresh(window);
}

void Game::run() {
  while (true) {
    // Player's move.
    int input = getch();
    if (input == 'q') {
      break;
    }

    // Pass it to the opponent.
    // In the current impl, both instances play the game by themselves and
    // only share the inputs. This works because the rand() is not
    // seeded and both the instances generate pellets at the same location.
    // Ideally, the instances should be sharing more info. Any drop in
    // connection will disrupt the game instances.
    if (write(socket, &input, sizeof(input)) < 0) {
      throw std::runtime_error("Cannot communicate with client.");
    }

    // Read opponent's move.
    int opponentMove = '\0';
    int bytesRead =
        recv(socket, &opponentMove, sizeof(opponentMove), MSG_DONTWAIT);

    // Reinit frame.:
    // Maybe it's cheaper to cleanup only the points where necessary but given
    // this is all running in a terminal, redrawing the whole frame barely
    // costs anything.
    werase(window);
    box(window, 0, 0);

    // Process snake's move.
    player.input(input);

    // If we got an opponent move, process it.
    if (bytesRead > 0) {
      opponent.input(opponentMove);
    }

    // Actually move the snake.
    // This impl could have race-conditions. Say if both the player and the
    // opponent get to the pellet at the same time, each one's game would
    // consider they got it. We have to explicitly handle that to keep the
    // points in sync.
    switch (player.move(pellet)) {
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

    // Move the opponent.
    switch (opponent.move(pellet)) {
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
    player.draw(window);
    opponent.draw(window);
    pellet.draw(window);
    drawScore();

    // Refresh frame.
    wrefresh(window);
    usleep(1000000 / FRAME_RATE);
  }
}