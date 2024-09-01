#include "point.h"

bool Point::doesOverlap(const Point &p1, const Point &p2) {
  return (p1.X == p2.X && p1.Y == p2.Y);
}