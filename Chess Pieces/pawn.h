#ifndef _PAWN_H_
#define _PAWN_H_

#include "tile.h"
using namespace std;

class Pawn : public Tile {
    bool moved = false;
    bool justMoved = false;
    bool justMoved2 = false;
    void possibleEnPassant(const pair<int, int> &posn, Board &board, vector<pair<int, int>> &possiblemoves);
    public:
        Pawn(char symbol) : Tile{symbol} {};
        bool Moved() { return moved; }
        void setJustMoved(bool torf){ 
            justMoved = torf;
            if(!moved && torf){
                moved = torf;
            }
        }
        bool getJustMoved(){ return justMoved; }
        void setJustMoved2(bool torf){ justMoved2 = torf; }
        bool getJustMoved2(){ return justMoved2; }
        vector<pair<int, int>> possibleMoves(const pair<int, int> &posn, Board &board);
        vector<pair<int, int>> possibleCaptures(const pair<int, int> &posn, Board &board, const bool secondcall);
        vector<pair<int, int>> possibleChecks(const pair<int, int> &posn, Board &board);


};
#endif
