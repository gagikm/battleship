#ifndef GLOBALS_INCLUDED
#define GLOBALS_INCLUDED

#include <cstdlib>

const int MAXROWS = 10;
const int MAXCOLS = 10;

const int MAX_PLACEMENT_TRIES = 1000;

enum Direction {
    HORIZONTAL, VERTICAL
};

class Point
{
  public:
    Point() : r(0), c(0) {}
    Point(int rr, int cc) : r(rr), c(cc) {}
    int r;
    int c;
};

  // Return a random int between 0 and limit-1 inclusive
inline int randInt(int limit)
{
    return limit <= 1 ? 0 : (std::rand() % limit);
}

#endif // GLOBALS_INCLUDED
