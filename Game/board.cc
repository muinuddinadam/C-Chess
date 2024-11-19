#include <vector>
#include <algorithm>
#include <utility>
#include <iostream>
#include <locale>
#include "board.h"
#include "tile.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h" /***/
#include "bishop.h" /***/
#include "queen.h" /***/
#include "king.h"
using namespace std;

// Board Constructor
Board::Board() : currTurn{0}, theBoard{new vector<vector<Tile*>>(boardSize, vector<Tile*>(boardSize, nullptr))} {
    // initialize the Board
    for (int y = 0; y < boardSize; ++y) {
		for (int x = 0; x < boardSize; ++x) {
            if (x == 7) { // initialize back row of Black pieces
                if ((y == 0) || (y == 7)) addTile('r', {y, x});
                else if ((y == 1) || (y == 6)) addTile('n', {y, x});
                else if ((y == 2) || (y == 5)) addTile('b', {y, x});
                else if (y == 3) addTile('q', {y, x});
                else addTile('k', {y, x});
            }
            else if (x == 6) addTile('p', {y, x}); // initialize second last row with Black Pawns
            else if ((x >= 2) && (x <= 5)){ // initialize middle rows with empty Tiles
                if ((x % 2) == (y % 2)) addTile('_', {y, x}); // black Tiles
                else addTile(' ', {y, x}); // white Tiles
            }
            else if (x == 1) addTile('P', {y, x}); // initialize second row with White Pawns
            else { // initialize back row of White pieces
                if ((y == 0) || (y == 7)) addTile('R', {y, x});
                else if ((y == 1) || (y == 6)) addTile('N', {y, x});
                else if ((y == 2) || (y == 5)) addTile('B', {y, x});
                else if (y == 3) addTile('Q', {y, x});
                else addTile('K', {y, x});
            }
		}
	}
}

// Board Destructor
Board::~Board() {
    // delete all the Tiles on the board
    for (auto& row : *theBoard) {
        for (auto& tile : row) {
            delete tile;
        }
    }
    delete theBoard;
}

// Board Copy Constructor
Board::Board(const Board &other) : currTurn{other.currTurn} {
    theBoard = new vector<vector<Tile*>>(other.boardSize, vector<Tile*>(other.boardSize, nullptr));

    // copy the other Board
    for (int x = 0; x < other.boardSize; ++x) {
		for (int y = 0; y < other.boardSize; ++y) {
            addTile(other.getTile({x, y})->getSymbol(), {x, y}); // copy each tile from the other Board
		}
	}

    // copy the captured Black pieces from the other board
    for (auto* piece : other.capturedBlack) {
        if (piece->getSymbol() == 'r') capturedBlack.push_back(new Rook('r'));
        else if (piece->getSymbol() == 'n') capturedBlack.push_back(new Knight('n'));
        else if (piece->getSymbol() == 'b') capturedBlack.push_back(new Bishop('b'));
        else if (piece->getSymbol() == 'q') capturedBlack.push_back(new Queen('q'));
        else if (piece->getSymbol() == 'p') capturedBlack.push_back(new Pawn('p'));
    }

    // copy the captured White pieces from the other Board
    for (auto* piece : other.capturedWhite) {
        if (piece->getSymbol() == 'R') capturedWhite.push_back(new Rook('R'));
        else if (piece->getSymbol() == 'N') capturedWhite.push_back(new Knight('N'));
        else if (piece->getSymbol() == 'B') capturedWhite.push_back(new Bishop('B'));
        else if (piece->getSymbol() == 'Q') capturedWhite.push_back(new Queen('Q'));
        else if (piece->getSymbol() == 'P') capturedWhite.push_back(new Pawn('P'));
    }
}

// Board Move Constructor
Board::Board(Board &&other)
    : currTurn{other.currTurn}, theBoard{other.theBoard}, activeWhite{move(other.activeWhite)}, activeBlack{move(other.activeBlack)}, capturedBlack{move(other.capturedBlack)}, capturedWhite{move(other.capturedWhite)} {
        other.theBoard = nullptr;
    }

// Board Copy Assignment
Board &Board::operator=(const Board &other) {
    if (this == &other) return *this; // check if the current Board is being copied to itself

    // delete the initial Tiles on the Board
    for (auto& row : *theBoard) {
        for (auto& tile : row) {
            delete tile;
        }
    }
    delete theBoard;

    // clear the lists of the active and captured pieces on the Board
    activeWhite.clear();
    activeBlack.clear();
    capturedBlack.clear();
    capturedWhite.clear();

    // rest is the same as the Copy Constructor
    currTurn = other.currTurn;
    theBoard = new vector<vector<Tile*>>(boardSize, vector<Tile*>(boardSize, nullptr));

    // copy the other Board
    for (int x = 0; x < boardSize; ++x) {
		for (int y = 0; y < boardSize; ++y) {
            addTile(other.getTile({x, y})->getSymbol(), {x, y}); // copy each tile from the other Board
		}
	}

    // copy the captured Black pieces from the other board
    for (auto* piece : other.capturedBlack) {
        if (piece->getSymbol() == 'r') capturedBlack.push_back(new Rook('r'));
        else if (piece->getSymbol() == 'n') capturedBlack.push_back(new Knight('n'));
        else if (piece->getSymbol() == 'b') capturedBlack.push_back(new Bishop('b'));
        else if (piece->getSymbol() == 'q') capturedBlack.push_back(new Queen('q'));
        else if (piece->getSymbol() == 'p') capturedBlack.push_back(new Pawn('p'));
    }

    // copy the captured White pieces from the other Board
    for (auto* piece : other.capturedWhite) {
        if (piece->getSymbol() == 'R') capturedWhite.push_back(new Rook('R'));
        else if (piece->getSymbol() == 'N') capturedWhite.push_back(new Knight('N'));
        else if (piece->getSymbol() == 'B') capturedWhite.push_back(new Bishop('B'));
        else if (piece->getSymbol() == 'Q') capturedWhite.push_back(new Queen('Q'));
        else if (piece->getSymbol() == 'P') capturedWhite.push_back(new Pawn('P'));
    }

    return *this;
}

// Board Move Assignment
Board &Board::operator=(Board &&other) {
    if (this == &other) return *this; // check if the current Board is being moved to itself

    // delete the initial Tiles on the Board
    for (auto& row : *theBoard) {
        for (auto& tile : row) {
            delete tile;
        }
    }
    delete theBoard;

    // clear the lists of the active and captured pieces on the Board
    activeWhite.clear();
    activeBlack.clear();
    capturedBlack.clear();
    capturedWhite.clear();

    // rest is the same as the Move Constructor
    // initialized values from the MIL
    currTurn = other.currTurn;
    theBoard = other.theBoard;
    activeWhite = move(other.activeWhite);
    activeBlack = move(other.activeBlack);
    capturedBlack = move(other.capturedBlack);
    capturedWhite = move(other.capturedWhite);

    other.theBoard = nullptr;

    return *this;
}

// Adds the Tile "symbol" to the Board at "tileCoords"
void Board::addTile(char symbol, pair<int, int> tileCoords) {
    // remove the Tile at tileCoords on the Board
    if (getTile(tileCoords)) {
        delete getTile(tileCoords);
        theBoard->at(tileCoords.first).at(tileCoords.second) = nullptr;
    }

    if ((symbol == 'r') || (symbol  == 'R')) { // check if we want to add a Rook
        theBoard->at(tileCoords.first).at(tileCoords.second) = new Rook(symbol);
        if (symbol == 'r') { // check if Rook is Black
            activeBlack.push_back(getTile(tileCoords));
        } else {
            activeWhite.push_back(getTile(tileCoords));
        }
    }
    else if ((symbol == 'n') || (symbol  == 'N')) { // check if we want to add a Knight
        theBoard->at(tileCoords.first).at(tileCoords.second) = new Knight(symbol);
        if (symbol == 'n') { // check if Knight is Black
            activeBlack.push_back(getTile(tileCoords));
        } else {
            activeWhite.push_back(getTile(tileCoords));
        }
    }
    else if ((symbol == 'b') || (symbol  == 'B')) { // check if we want to add a Bishop
        theBoard->at(tileCoords.first).at(tileCoords.second) = new Bishop(symbol);
        if (symbol == 'b') { // check if Bishop is Black
            activeBlack.push_back(getTile(tileCoords));
        } else {
            activeWhite.push_back(getTile(tileCoords));
        }
    }
    else if ((symbol == 'q') || (symbol  == 'Q')) { // check if we want to add a Queen
        theBoard->at(tileCoords.first).at(tileCoords.second) = new Queen(symbol);
        if (symbol == 'q') { // check if Queen is Black
            activeBlack.push_back(getTile(tileCoords));
        } else {
            activeWhite.push_back(getTile(tileCoords));
        }
    }
    else if ((symbol == 'k') || (symbol  == 'K')) { // check if we want to add a King
        theBoard->at(tileCoords.first).at(tileCoords.second) = new King(symbol);
        if (symbol == 'k') { // check if King is Black
            activeBlack.push_back(getTile(tileCoords));
        } else {
            activeWhite.push_back(getTile(tileCoords));
        }
    }
    else if ((symbol == 'p') || (symbol  == 'P')) { // check if we want to add a Pawn
       theBoard->at(tileCoords.first).at(tileCoords.second) = new Pawn(symbol);
        if (symbol == 'p') { // check if King is Black
            activeBlack.push_back(getTile(tileCoords));
        } else {
            activeWhite.push_back(getTile(tileCoords));
        }
    }
    else if ((symbol == '_') || (symbol  == ' ')) { // check if we want to add an empty Tile
        theBoard->at(tileCoords.first).at(tileCoords.second) = new Tile(symbol);
    }
}

// Removes the Tile at "tileCoords" on the Board
void Board::removeTile(pair<int, int> tileCoords) {
    // remove the Tile from the active Black pieces
    auto activeTile = find(activeBlack.begin(), activeBlack.end(), getTile(tileCoords)); // check to see if the Tile is an active Black piece
    if (activeTile != activeBlack.end()) activeBlack.erase(activeTile);

    // remove the Tile from the active White pieces
    activeTile = find(activeWhite.begin(), activeWhite.end(), getTile(tileCoords)); // checks to see if the Tile is an active White piece
    if (activeTile != activeWhite.end()) activeWhite.erase(activeTile);

    if ((tileCoords.first % 2) == (tileCoords.second % 2)) addTile('_', {tileCoords.first, tileCoords.second}); // black Tiles
    else addTile(' ', {tileCoords.first, tileCoords.second}); // white Tiles
}

// Moves the Tile at "from" on the Board to "to" on the Board, returns true if successful
char Board::makeMove(pair<int, int> from, pair<int, int> to) {
    // reset the justMoved2 field for all pawns
    vector<Tile*> activePieces = (currTurn % 2 == 0) ? getActiveWhite() : getActiveBlack();
    for (unsigned x = 0; x < activePieces.size(); ++x) {
        if ((activePieces[x]->getSymbol() == 'p') || (activePieces[x]->getSymbol() == 'P')) {
            activePieces[x]->setJustMoved2(false);
         }
    }
    
    // set all tiles to not moved
    for (unsigned x = 0; x < activePieces.size(); ++x) {
        if ((activePieces[x]->getSymbol() != ' ') || (activePieces[x]->getSymbol() != '_')) {
            activePieces[x]->setJustMoved(false);
        }
    }

    // set Tile characters for initial and removed Tiles
    char removedTile = getTile(to)->getSymbol();
    char initialTile = getTile(from)->getSymbol();

    vector<pair<int, int>> pMoves = getTile(from)->possibleMoves(from, *this);

    // check if the move is invalid
    if (find(pMoves.begin(), pMoves.end(), to) == pMoves.end()) {
        return 'X'; // failed move indicator
    }

    // check if the Tile being removed was a Piece
    if ((removedTile != ' ') || (removedTile != '_')) {
        if (islower(removedTile)) {
            capturedBlack.push_back(getTile(to));
        }
        else if (isupper(removedTile)) {
            capturedWhite.push_back(getTile(to));
        }
    }

    // replace the Tile at "to" with the Tile at "from"
    removeTile(to);
    addTile(initialTile, to);
    removeTile(from);

    // set only the piece that is actually moving to just moved
    getTile(to)->setJustMoved(true);

    // if the piece that moved was a pawn that moved two spaces, make sure it is eligible to be enpassanted
    if (((initialTile == 'p') || (initialTile == 'P')) && (abs(to.second - from.second) == 2)) {
        getTile(to)->setJustMoved2(true);
    }

    // check for castling
    if (((to.first == from.first + 2) || (to.first == from.first - 2))) {
        char rook;
        if (initialTile == 'k' || initialTile == 'K') {
            if (islower(initialTile)) {
                rook = 'r';
                removedTile = 'c'; // black castle indicator
            }

            else if (isupper(initialTile)) {
                rook = 'R';
                removedTile = 'C'; // white castle indicator
            }

            if (to.first == (from.first + 2)) { // right castle
                removeTile({to.first - 1, to.second}); // remove the Tile directly to the left of the King
                addTile(rook, {to.first - 1, to.second}); // add a Rook to the Tile directly left of the King
                removeTile({to.first + 1, to.second}); // remove the Rook on the right side of the King

                // replace the Rook's old Tile on the right side of the King
                if (((to.first + 1) % 2) == (from.second % 2)) addTile('_', {to.first + 1, to.second}); // empty black Tile
                else addTile(' ', {to.first + 1, to.second}); // empty white Tile

            } else if (to.first == (from.first - 2)) { // left castle
                removeTile({to.first + 1, to.second}); // remove the Tile directly to the right of the King
                addTile(rook, {to.first + 1, to.second}); // add a Rook to the Tile directly right of the King
                removeTile({to.first - 2, to.second}); // remove the Rook on the left side of the King

                // replace the Rook's old Tile on the left side of the King
                if (((to.first - 2) % 2) == (from.second % 2)) addTile('_', {to.first - 2, to.second}); // empty black Tile
                else addTile(' ', {to.first - 2, to.second}); // empty white Tile
            }
        }
    }

    // check for en passant
    if (((removedTile == ' ') || (removedTile == '_')) && ((initialTile == 'p') || (initialTile == 'P')) && (to.first != from.first)) {
        int direction = (from.first < to.first) ? 1 : -1;

        if (initialTile == 'p') {
            capturedWhite.push_back(getTile({(from.first + direction), from.second}));
            removedTile = 'e'; // black en passant indicator
        }
        else {
            capturedBlack.push_back(getTile({(from.first + direction), from.second}));
            removedTile = 'E'; // white en passant indicator
        }

        removeTile({(from.first + direction), from.second});

        // replace removed pawn with empty Tile
        if (((from.first + direction) % 2) == (from.second % 2)) addTile('_', {(from.first + direction), from.second}); // empty black Tile
        else addTile(' ', {(from.first + direction), from.second}); // empty white Tile
    }

    // replace the Tile at "from" with an empty Tile
    if ((from.first % 2) == (from.second % 2)) addTile('_', from); // empty black Tile
    else addTile(' ', from); // empty white Tile

    ++currTurn;
    
    return removedTile;
}

// Moves the Tile at "from" on the Board to "to" on the Board and replaces that new Tile with "promoSymbol", returns true if successful
char Board::makeMove(pair<int, int> from, pair<int, int> to, char promoSymbol) {
    // use previous makeMove for efficiency
    char removedTile = makeMove(from, to);
    if (removedTile != 'X') {
        // replace the new Piece at "to" with a new symbol
        removeTile(to);
        addTile(promoSymbol, to);
        return removedTile;
    }
    return 'X';
}

// Returns true if the Player whose turn it is is in check
bool Board::inCheck(Board temp) {
    char king = ((currTurn % 2) == 1) ? 'k' : 'K'; // change the king depending on the current Player's turn
    vector<char> pieces = ((currTurn % 2) == 1) ? vector{'r', 'n', 'b', 'q', 'p'} : vector{'R', 'N', 'B', 'Q', 'P'}; // change the pieces depending on the current Player's turn

    for (int x = 0; x < temp.boardSize; ++x) { // loop through the temporary board and search for the King
        for (int y = 0; y < temp.boardSize; ++y) {
            if (temp.theBoard->at(x).at(y)->getSymbol() == king) {
                for (char piece : pieces) { // loop through the different possible Pieces
                    // replace the King with a specific Piece
                    temp.removeTile({x, y});
                    temp.addTile(piece, {x, y});

                    // the possible captures that each Piece would make in the same position as the King
                    vector<pair<int, int>> captures = temp.getTile({x, y})->possibleCaptures({x, y}, temp, true);

                    // check if any of those possible captures by a Piece would result in capturing another one of that same Piece
                    for (auto& capture : captures) {
                        piece = (currTurn % 2 == 1) ? toupper(piece) : tolower(piece);
                        if (temp.theBoard->at(capture.first).at(capture.second)->getSymbol() == piece) {
                            return true; // if so, the King is in check by the subsequent Piece
                        }
                    }
                }
                break; // break once the king is found
            }
        }
    }
    // there are no possible moves that would put the King in check
    return false;
}

// Returns true if the Player moving on turn "turn" is in checkmate
bool Board::checkMate() {
    if (inCheck(*this) && staleMate())
        return true;

    return false;
}

// Returns true if the Player moving on turn "turn" cannot make any moves
bool Board::staleMate() {
    int numOfMoves = 0;

    for (int x = 0; x < boardSize; ++x) { // loop through the Board to check if any moves can be made depending on whose turn it is to play
        for (int y = 0; y < boardSize; ++y) { 
            if ((((currTurn % 2) == 1) && (islower(getTile({x, y})->getSymbol())))  // the Black Player's turn
            || (((currTurn % 2) == 0) && (isupper(getTile({x, y})->getSymbol())))) {  // the White Player's turn
                numOfMoves += getTile({x, y})->possibleMoves({x, y}, *this).size();
            }
        }
    }

    return !numOfMoves;
}
