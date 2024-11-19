#ifndef _TILE_H_
#define _TILE_H_
#include <vector>
#include <utility>
#include "board.h"
using namespace std;

class Board;

class Tile {
    char symbol;

  public:
    // Constructor
    Tile(char sym) : symbol{sym} {}

    // Destructor
    virtual ~Tile() {}

    // Get symbol character
    char getSymbol() const { return symbol; }

    void setSymbol(char sym) { symbol = sym;}

    virtual bool Moved(){
      return true;
    }

    virtual bool getJustMoved(){ return false; }

    virtual void setJustMoved( bool torf ){}

    virtual bool getJustMoved2() { return false; }

    virtual void setJustMoved2( bool torf ){}

    // Finds all possible moves for that the player can make
    virtual vector<pair<int, int>> possibleMoves(const pair<int, int> &posn, Board &board){
      vector<pair<int, int>> vec;
      return vec;
    }

    // Finds all possible captures that the player can make
    virtual vector<pair<int, int>> possibleCaptures(const pair<int, int> &posn, Board &board, const bool secondcall){
      vector<pair<int, int>> vec;
      return vec;
    }

    // Finds all possible checks that the player can make
    virtual vector<pair<int, int>> possibleChecks(const pair<int, int> &posn, Board &board){
      vector<pair<int, int>> vec;
      return vec;
    }

    bool simulateMove(const pair<int, int> &posn1, const pair<int, int> &posn2, Board &board, Tile* piece){
      Board tempboard = board;
      tempboard.removeTile(posn2);
      piece = board.getTile(posn1);
      tempboard.addTile(piece->getSymbol(), posn2);
      tempboard.removeTile(posn1);
      return tempboard.inCheck(tempboard);
    }
};

#endif

