#ifndef BOARD_INCLUDED
#define BOARD_INCLUDED

#include "globals.h"

class Game;
class BoardImpl;

class Board
{
  public:
    Board(const Game& g);
    ~Board();
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
    BoardImpl* m_impl;
      // Boards can not be copied or assigned.  We enforce this by declaring
      // the copy constructor and assignment operator private and not
      // implementing them.
    Board(const Board&);
    Board& operator=(const Board&);
};

#endif // BOARD_INCLUDED
