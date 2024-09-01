#pragma once

struct Point {
  int X, Y;

  static bool doesOverlap(const Point &p1, const Point &p2);
};