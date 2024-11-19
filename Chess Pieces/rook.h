#ifndef _ROOK_H_
#define _ROOK_H_

#include "tile.h"
using namespace std;

class Rook : public Tile {
    bool moved = false;
    bool justMoved = false;
    public:
        Rook(char symbol) : Tile{symbol} {};
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
