#ifndef _BOARD_H_
#define _BOARD_H_
#include <vector>
#include <utility>
#include <string>
using namespace std;

class Tile;

class Board {
    inline static const int boardSize = 8;
    int currTurn;
    vector<vector<Tile*>> *theBoard;
    vector<Tile*> activeWhite;
    vector<Tile*> activeBlack;
    vector<Tile*> capturedBlack;
    vector<Tile*> capturedWhite;
  public:
    Board();
    virtual ~Board();
    Board(const Board &other);
    Board(Board &&other);
    Board &operator=(const Board &other);
    Board &operator=(Board &&other);
    void setCurrTurn(int turn) { currTurn = turn; }
    int getCurrTurn() const { return currTurn; }
    int getBoardSize() const { return boardSize; }
    vector<vector<Tile*>> getBoard() const { return *theBoard; }
    vector<Tile*> getActiveWhite() const { return activeWhite; }
    vector<Tile*> getActiveBlack() const { return activeBlack; }
    Tile* getTile(pair<int, int> tileCoords) const { return theBoard->at(tileCoords.first).at(tileCoords.second); }
    void addTile(char symbol, pair<int, int> tileCoords);
    void removeTile(pair<int, int> tileCoords);
    char makeMove(pair<int, int> from, pair<int, int> to);
    char makeMove(pair<int, int> from, pair<int, int> to, char promoSymbol); // makeMove for pawn promotion
    bool inCheck(Board temp);
    bool checkMate();
    bool staleMate();
};

#endif
