#ifndef _KING_H_
#define _KING_H_

#include "tile.h"
using namespace std;

class King : public Tile {
    bool moved = false;
    bool justMoved = false;
    void possibleCastles(const pair<int, int> &posn, Board &board, vector<pair<int, int>> &possiblemoves);
    
    public:
        King(char symbol) : Tile{symbol} {};
        bool Moved() { return moved; }
        void setJustMoved(bool torf){ 
            justMoved = torf;
            if(!moved && torf){
                moved = torf;
            }
        }

        vector<pair<int, int>> possibleMoves(const pair<int, int> &posn, Board &board);
        vector<pair<int, int>> possibleCaptures(const pair<int, int> &posn, Board &board, const bool secondcall);
        vector<pair<int, int>> possibleChecks(const pair<int, int> &posn, Board &board);
    
};
#endif
