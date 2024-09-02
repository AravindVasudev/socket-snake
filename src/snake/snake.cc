#include "snake.h"

#include <algorithm>
#include <ranges>

#include "color.h"

Snake::Snake(bool isServer) {
  // TODO: Bad impl, fix this.
  if (isServer) {
    head.X = SERVER_SNAKE_X;
    head.Y = SERVER_SNAKE_Y;
  } else {
    head.X = CLIENT_SNAKE_X;
    head.Y = CLIENT_SNAKE_Y;
  }
};

void Snake::input(const int& input) {
  switch (input) {
    case 'w':
    case KEY_UP:
      if (velocity.Y != 1) {
        velocity.X = 0;
        velocity.Y = -1;
      }

      break;
    case 's':
    case KEY_DOWN:
      if (velocity.Y != -1) {
        velocity.X = 0;
        velocity.Y = 1;
      }

      break;
    case 'a':
    case KEY_LEFT:
      if (velocity.X != 1) {
        velocity.X = -1;
        velocity.Y = 0;
      }
      break;
    case 'd':
    case KEY_RIGHT:
      if (velocity.X != -1) {
        velocity.X = 1;
        velocity.Y = 0;
      }

      break;
  }
}

MoveState Snake::move(const Pellet& pellet) {
  // Move the tail.
  Point old = head;
  for (auto& t : tail) {
    std::swap(t, old);
  }

  // Move the head.
  head.X += velocity.X;
  head.Y += velocity.Y;

  // Bound check.
  if (head.X <= 0 || head.X >= WIDTH || head.Y <= 0 || head.Y >= HEIGHT) {
    return MoveState::DEAD;
  }

  // Check if the snake is eating it's own body.
  for (const auto& t : tail) {
    if (Point::doesOverlap(head, t)) {
      return MoveState::DEAD;
    }
  }

  // Snake ate the pellet!
  if (Point::doesOverlap(head, pellet.position)) {
    tail.push_back(Point{pellet.position.X, pellet.position.Y});
    return MoveState::EAT;
  }

  return MoveState::NOOP;
}

void Snake::drawHead(WINDOW* window) {
  switch (velocity.Y) {
    case -1:
      mvwprintw(window, head.Y, head.X, "▲");
      break;
    case 1:
      mvwprintw(window, head.Y, head.X, "▼");
      break;
    default:
      switch (velocity.X) {
        case -1:
          mvwprintw(window, head.Y, head.X, "◀");
          break;
        case 1:
          mvwprintw(window, head.Y, head.X, "▶");
          break;
      }
  }
}

void Snake::draw(WINDOW* window) {
  wattron(window, COLOR_PAIR(Color::SnakeC));
  drawHead(window);
  for (const auto& t : tail) {
    mvwprintw(window, t.Y, t.X, "@");
  }
  wattroff(window, COLOR_PAIR(Color::SnakeC));
}