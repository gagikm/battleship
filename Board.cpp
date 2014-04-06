#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <vector>

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
  
    const Game& m_game;
    int m_rows;
    int m_cols;
    char grid[MAXROWS][MAXCOLS];
    int m_nShips;
    struct shipInfo
    {
        shipInfo(int shipID, Point topOrLeft, Direction dir): bm_startPoint(topOrLeft), bm_shipID(shipID), bm_segmentsHit(0), bm_direction(dir)
        {}
        Point  bm_startPoint;
        int   bm_shipID;
        int bm_segmentsHit;
        Direction bm_direction;
    };
    vector <shipInfo*> infoTrack;
};

BoardImpl::BoardImpl(const Game& g)
 : m_game(g), m_rows(g.rows()), m_cols(g.cols()), m_nShips(g.nShips())
{
    for(int i=0; i<m_rows; i++)
        for(int j=0; j<m_cols; j++)
        {
            grid[i][j]='.';
        }
}

void BoardImpl::clear()
{
    for(int i=0; i<m_rows; i++)
        for(int j=0; j<m_cols; j++)
        {
            grid[i][j]='.';
        }
}

void BoardImpl::block()
{
      // Block cells with 50% probability
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
            if (randInt(2) == 0)
            {
                grid[r][c]='#';
            }
}

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
        {
            if(grid[r][c]=='#')
                grid[r][c]='.';
        }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    if (shipId<0 || shipId>=m_game.nShips() || topOrLeft.r<0 ||topOrLeft.r>=m_game.rows() ||topOrLeft.c<0 || topOrLeft.c>=m_game.cols())
    {
        //cerr<<"Invalid ID or Point."<<endl;
        return false;}
    
    if((dir==HORIZONTAL && topOrLeft.c+m_game.shipLength(shipId)>m_game.cols())
    || (dir==VERTICAL && topOrLeft.r+m_game.shipLength(shipId)>m_game.rows()))
    {
       // cerr<<"Ship does not fit at that point."<<endl;
        return false;}
    
    for(int i=0; i<infoTrack.size(); i++) //check for already placed ship
    {
        if (infoTrack[i]->bm_shipID==shipId)
        {
            //cerr<<"ID already placed"<<endl;
            return false;}
            
    }
    if(dir==HORIZONTAL)
    {
        for (int i=topOrLeft.c; i<topOrLeft.c+m_game.shipLength(shipId); i++) //check for overlap
        {
            if(grid[topOrLeft.r][i]!='.')
            {
               // cerr<<"overlap"<<endl;
                return false;}
        }
        
        for (int i=topOrLeft.c; i<topOrLeft.c+m_game.shipLength(shipId); i++) //place ship
        {
            grid[topOrLeft.r][i]=m_game.shipSymbol(shipId);
        }
    }
    if(dir==VERTICAL)
    {
        for (int i=topOrLeft.r; i<topOrLeft.r+m_game.shipLength(shipId); i++) //check for overlap
        {
            if(grid[i][topOrLeft.c]!='.')
            {
                //cerr<<"overlap"<<endl;
                return false;}
        }
        
        for (int i=topOrLeft.r; i<topOrLeft.r+m_game.shipLength(shipId); i++) //place ship
        {
            grid[i][topOrLeft.c]=m_game.shipSymbol(shipId);
        }
    }
    //display(false);
    infoTrack.push_back(new shipInfo(shipId, topOrLeft, dir));
    return true;
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    bool validShip=false;                           //make sure ship with this ID has been placed and ship begins at point 'topOrLeft'
    for (int i=0; i<infoTrack.size(); i++)
    {
        if(infoTrack[i]->bm_shipID==shipId &&
           infoTrack[i]-> bm_startPoint.r==topOrLeft.r &&
           infoTrack[i]-> bm_startPoint.c == topOrLeft.c &&
           infoTrack[i]-> bm_direction==dir)
            validShip=true;
    }
    if (!validShip)
    {
        cerr<<"A ship with this ID has not been placed or ship does not start there!"<<endl;
        return false;}
    
    if(dir==HORIZONTAL)
    {
        for (int i=topOrLeft.c; i<topOrLeft.c+m_game.shipLength(shipId); i++) //unplace ship
        {
            grid[topOrLeft.r][i]='.';
        }
    }
    if(dir==VERTICAL)
    {
        
        for (int i=topOrLeft.r; i<topOrLeft.r+m_game.shipLength(shipId); i++) //unplace ship
        {
            grid[i][topOrLeft.c]='.';
        }
    }
    for (int i=0; i<infoTrack.size(); i++)
    {
        if(infoTrack[i]->bm_shipID==shipId)
        {
            delete infoTrack[i];    //deallocate memory
            infoTrack.erase(infoTrack.begin()+i);   //remove shipID from infoTrack vector
            return true;
        }
    }
    return true;
}

void BoardImpl::display(bool shotsOnly) const
{
    
        cout<<"  ";                                //output column numbers
        for(int i=0; i<m_game.cols(); i++)
            cout<<i;
        cout<<endl;
    
    
        int i=0;
        int j=0;
        for(i=0; i<m_game.rows(); i++)
        {
            
            for(j=0; j<m_game.cols(); j++)
            {
                if(j==0)
                    cout<<i<<" ";       //output row numbers
                
                if(!shotsOnly)
                cout<<grid[i][j];   //if shotsOnly is false, show everything
                else
                {
                    if (grid[i][j] != 'X' && grid[i][j] != 'o' && grid[i][j]!='.')  //if it's a letter, show a dot
                        cout<<'.';
                    else
                        cout <<grid[i][j];  //if it's not a letter, show what is really there
                }
            }
            cout<<endl;
        }


}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    if(p.r<0 || p.c<0 || p.r>=m_rows || p.c>=m_cols || grid[p.r][p.c]=='o' || grid[p.r][p.c]=='X')
    {
        shotHit=false;
        shipDestroyed=false;
        return false;
    }

    
    if(grid[p.r][p.c]=='.')
    {   grid[p.r][p.c]='o';
        shotHit=false;      }
    else
    {
        
        shotHit=true;
        for(int i=0; i<infoTrack.size(); i++)
        {
            if(grid[p.r][p.c]==m_game.shipSymbol(infoTrack[i]->bm_shipID))
            {
                (infoTrack[i]->bm_segmentsHit)++;
                if(infoTrack[i]->bm_segmentsHit>=m_game.shipLength(infoTrack[i]->bm_shipID))
                {
                    shipDestroyed=true;
                    shipId=infoTrack[i]->bm_shipID;
                    m_nShips--;
                    break;
                }
                else{
                    shipDestroyed=false;
                    break;
                    }
            }
        }
        
        grid[p.r][p.c]='X';
    }
    return true;
}

bool BoardImpl::allShipsDestroyed() const
{
    if(m_nShips==0)
        return true;
    else return false;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
