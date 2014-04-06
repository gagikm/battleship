#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <stack>
using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);

  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result = (cin >> r >> c).good();
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

class HumanPlayer : public Player
{
  public:
    HumanPlayer(string nm, const Game& g);
    virtual bool isHuman() const;
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId){};
    
};
HumanPlayer::HumanPlayer(string nm, const Game& g): Player(nm, g)
{}
bool HumanPlayer::isHuman() const
{
    return true;
}

bool HumanPlayer::placeShips(Board& b)
{
    char direction=' ';
    for (int i=0; i<game().nShips(); i++)
    {
        
        while(direction!='v' && direction!='h')
        {
            cout<<"Enter h or v for direction of "<<game().shipName(i)<<" (length "<<game().shipLength(i)<<"): ";
            cin>>direction;
            cin.ignore(10000, '\n');
            if(direction!='v' && direction!='h')
            {
            cout<<"Direction must be h or v."<<endl;
            }
        }
        
        Direction dir;
        if(direction=='v')
            dir=VERTICAL;
        else
            dir=HORIZONTAL;
        
        bool success=false;
    while(success!=true)
    {
        cout<<"Enter row and column of leftmost cell (e.g. 3 5): ";
    int r, c;
    if(!getLineWithTwoIntegers(r, c))
        return false;
    else
    {
    if(b.placeShip(Point(r, c), i, dir)==true)
    {
        b.display(false);
        direction=0; //reset direction
        success=true;
    }
    }
    }
    
}
    return true;
}
Point HumanPlayer::recommendAttack()
{
    cout<<"Enter the row and column to attack (e.g, 3 5): ";
    int r, c;
    getLineWithTwoIntegers(r, c);
    return Point(r,c);
    
}
       
//*********************************************************************
//  MediocrePlayer
//*********************************************************************

class MediocrePlayer : public Player
{
  public:
    MediocrePlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                bool shipDestroyed, int shipId);
    bool placeRecursively(Board &b, int shipID);
private:
    const Game& m_game;
    Point m_lastCellAttacked;
    char attacked[MAXROWS][MAXCOLS];
    int state;
    
};

MediocrePlayer::MediocrePlayer(string nm, const Game& g):Player(nm,g),m_game(g),m_lastCellAttacked(0,0), state(1)
{
    for(int i=0; i<m_game.rows(); i++)
        for(int j=0; j<m_game.cols(); j++)
        {
            attacked[i][j]='n';
        }
};

//===========================Mediocre: Place Ships=========================?

bool MediocrePlayer::placeRecursively(Board &b, int shipID)
{
//--------------------------Recursive algorithm------------------------//
    if (shipID==game().nShips())
    {
            //shipID=0;
        return true;
    }
    
    for (int i=0;i<m_game.rows();i++)
        for(int j=0;j<m_game.cols();j++)
        {
            Point p(i,j);
            
                if(b.placeShip(p, shipID, VERTICAL))
                {
                    
                    if(placeRecursively(b, shipID+1))
                        return true;
                    b.unplaceShip(p, shipID, VERTICAL);
                    
                }
            
                if(b.placeShip(p, shipID, HORIZONTAL))
                {
                    if(placeRecursively(b, shipID+1))
                            return true;
                    b.unplaceShip(p, shipID, HORIZONTAL);
                }
        }
    return false;
}
//--------------------------------------------------------------------//

bool MediocrePlayer::placeShips(Board &b)
{
    b.block();
    if(!placeRecursively(b, 0))
    {   b.unblock();
        return false;
    }
    b.unblock();
    return true;
}



//===================Mediocre: Attack!==================================//
Point MediocrePlayer::recommendAttack()
{
    //state 1
    if(state==1)
    {
    
        for(int i=0; i<m_game.rows()*m_game.cols(); i++)
        {
            Point p =m_game.randomPoint();
            if(attacked[p.r][p.c]=='n')
            {
                attacked[p.r][p.c]='y';
                return p;
            }
        }
    }
    
    if(state==2)
    {
        for(int i=0; i<m_game.rows(); i++)
            for(int j=0; j<m_game.cols();j++)
            {   Point p(i,j);
                if(attacked[p.r][p.c]=='+')
                {
                    attacked[p.r][p.c]='y';
                    return p;
                }
            }
    }
    
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
                                bool shipDestroyed, int shipId)
{
    if(validShot && shotHit && !shipDestroyed && state==1)
    {
        //vertical line of cross
        for(int i=1; (p.r+i)<m_game.rows()&&i<5;i++)
        {
            if(attacked[p.r+i][p.c]=='n')
            attacked[p.r+i][p.c]='+';
        }
        for(int i=1; (p.r-i)>=0&&i<5;i++)
        {
            if(attacked[p.r-i][p.c]=='n')
            attacked[p.r-i][p.c]='+';
        }
        //horizontal line of cross
        for(int i=1; (p.c+i)<m_game.cols()&&i<5;i++)
        {
            if(attacked[p.r][p.c+i]=='n')
            attacked[p.r][p.c+i]='+';
        }
        for(int i=1; (p.c-i)>=0&&i<5;i++)
        {
            if(attacked[p.r][p.c-i]=='n')
            attacked[p.r][p.c-i]='+';
        }
        
        state=2;
    }
    if(!shotHit)
        ;
    if(shipDestroyed==true)
    {
        for(int i=0; i<m_game.rows(); i++)
            for(int j=0; j<m_game.cols();j++)
            {   Point p(i,j);
                if(attacked[p.r][p.c]=='+')
                {
                    attacked[p.r][p.c]='n';
                }
            }

        state=1;
    }
}
//*********************************************************************
//  GoodPlayer
//*********************************************************************

class GoodPlayer : public Player
{
public:
    GoodPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    bool placeRecursively(Board &b, int shipID);
    Point avoidWaste();
private:
    const Game& m_game;
    Point m_lastCellAttacked;
    char attacked[MAXROWS][MAXCOLS];
    int state;
    int turn;
    stack <Point> checkThese;
    
};

GoodPlayer::GoodPlayer(string nm, const Game& g):Player(nm,g),m_game(g),m_lastCellAttacked(0,0), state(1), turn(1)
{
    for(int i=0; i<m_game.rows(); i++)
        for(int j=0; j<m_game.cols(); j++)
        {
            attacked[i][j]='n';
        }
};

//===========================GoodPlayer: Place Ships=========================?

bool GoodPlayer::placeRecursively(Board &b, int shipID)
{
//--------------------------Recursive algorithm------------------------//
    if (shipID==game().nShips())
    {
        shipID=0;
        return true;
    }
    

        {
            Point p=m_game.randomPoint();
            
            if(b.placeShip(p, shipID, VERTICAL))
            {
                
                if(placeRecursively(b, shipID+1))
                    return true;
                b.unplaceShip(p, shipID, VERTICAL);
                
            }
            
            if(b.placeShip(p, shipID, HORIZONTAL))
            {
                if(placeRecursively(b, shipID+1))
                    return true;
                b.unplaceShip(p, shipID, HORIZONTAL);
            }
        }//cout<<"NO";
    return false;
}
//--------------------------------------------------------------------//

bool GoodPlayer::placeShips(Board &b)
{
    //b.block();
    
    if(!placeRecursively(b, 0))
    {   b.unblock();
        return false;
    }
    //b.unblock();
    return true;
}



//===================GoodPlayer: Attack!==================================//
Point GoodPlayer::avoidWaste()
{
    
    for(int i=0; ; i++)
    {
        Point p =m_game.randomPoint();
        
        {
            if(p.r%2==0 && p.c%2!=0)
                p.r=p.r+1;
            else if(p.c%2==0 && p.r%2!=0)
                p.c=p.c+1;
        }
        if(attacked[p.r][p.c]=='n')
        {
            attacked[p.r][p.c]='y';
            return p;
        }
    }

}
Point GoodPlayer::recommendAttack()
{
    //state 1
    if(state==1)
    {
        if(turn==1)
        {
            Point p(m_game.rows()/2, m_game.cols()/2);
            attacked[p.r][p.c]='y';
            turn++;
            return p;
        }
        
        if (turn==2)
        {
            Point p(m_game.rows()/4, 0);
            if(attacked[p.r][p.c]=='n')
            {
                attacked[p.r][p.c]='y';
                turn++;
                return p;
            }
        }
        
        if (turn==3)
        {
            Point p((m_game.rows())-(m_game.rows()/4), (m_game.cols())-1);
            if(attacked[p.r][p.c]=='n')
            {
                attacked[p.r][p.c]='y';
                turn++;
                return p;
            }
        }
        if (turn==4)
        {
            Point p(m_game.rows()/4, (m_game.cols())-1);
            if(attacked[p.r][p.c]=='n')
            {
                attacked[p.r][p.c]='y';
                turn++;
                return p;
            }
            
        }
        if (turn==5)
        {
            Point p((m_game.rows())-(m_game.rows()/4), 0);
            if(attacked[p.r][p.c]=='n')
            {
                attacked[p.r][p.c]='y';
                turn++;
                return p;
            }
        }
        for(int i=0; ; i++)
        {
            Point p =m_game.randomPoint();
            
            {
            if(p.r%2==0 && p.c%2!=0)
                p.r=p.r+1;
            else if(p.c%2==0 && p.r%2!=0)
                p.c=p.c+1;
            }
                if(attacked[p.r][p.c]=='n')
            {
                attacked[p.r][p.c]='y';
                return p;
            }
        }
        return avoidWaste();
    }
    
    if(state==2)
    {
        while(!checkThese.empty())
        {   Point p=checkThese.top();
            checkThese.pop();
            if(attacked[p.r][p.c]=='n')
            {
                attacked[p.r][p.c]='y';
                return p;
            }
        }
        return avoidWaste();
        
    }
    
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
                                        bool shipDestroyed, int shipId)
{
    if(validShot && shotHit && !shipDestroyed)
    {
        if(p.r+1<m_game.rows())
            checkThese.push(Point(p.r+1, p.c));
        if(p.c+1<m_game.cols())
            checkThese.push(Point(p.r, p.c+1));
        if(p.r-1>=0)
            checkThese.push(Point(p.r-1, p.c));
        if(p.c-1>=0)
            checkThese.push(Point(p.r, p.c-1));
        
        state=2;
    }
    if(checkThese.empty())
        state=1;
    
    if(!shotHit)
        ;
    if(shipDestroyed==true)
    {
        if(!checkThese.empty())
            state=2;
        else
        state=1;
    }
}

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return NULL;
    }
}
