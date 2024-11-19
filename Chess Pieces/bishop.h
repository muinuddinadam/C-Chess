#ifndef _BISHOP_H
#define _BISHOP_H_

#include "tile.h"
using namespace std;

class Bishop : public Tile {
    bool justMoved = false;
    public:
        Bishop(char symbol) : Tile{symbol} {};
        void setJustMoved(bool torf){ justMoved = torf; }
        vector<pair<int, int>> possibleMoves(const pair<int, int> &posn, Board &board);
        vector<pair<int, int>> possibleCaptures(const pair<int, int> &posn, Board &board, const bool secondcall);
        vector<pair<int, int>> possibleChecks(const pair<int, int> &posn, Board &board);

};
#endif
