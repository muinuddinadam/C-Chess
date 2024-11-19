#include "knight.h"
#include "board.h"
#include <locale>
using namespace std;

vector<pair<int, int>> Knight::possibleMoves(const pair<int, int> &posn, Board &board){
    // Define variables
    vector<pair<int, int>> possibleMoves; // Define variable for possible moves
    pair<int, int> posn2; // Define variable for posn2 to check moves
    Tile *piece; // Define variable to store what is at that tile
    int currTurn = board.getCurrTurn(); // Define a variable to store who's turn it is
    bool incheck = false; // Define a variable to store whether you are in check once a simulated move is made
    vector<pair<int, int>> directions = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}}; // Define a variable to store the directions a piece can move

    // Iterate through all moves the knight can make
    for(const pair<int, int> &dir : directions){
        posn2 = posn; // Reset the position of the piece 

        posn2.first += dir.first; // Update the x coordinate
        posn2.second += dir.second; // Update the y coordinate

        // Ensure that the piece is still in bounds
        if (posn2.first < 0 || posn2.first > 7 || posn2.second < 0 || posn2.second > 7) continue;
        
        piece = board.getTile(posn2); // Get the tile stored at posn2 of the board

        // Simulate the move on a temporary board
        incheck = simulateMove(posn, posn2, board, piece);

        if(!incheck){
            if(piece->getSymbol() != ' ' && piece->getSymbol() != '_'){
                if(currTurn % 2 == 0){ // White turn
                    if(islower(piece->getSymbol())){
                        possibleMoves.push_back(posn2);
                    }
                    continue;
                } else { // Black turn
                    if(isupper(piece->getSymbol())){
                        possibleMoves.push_back(posn2);
                    }  
                    continue;
                }
            }
            possibleMoves.push_back(posn2);
        }
    }

    // Return the possibleMoves
    return possibleMoves;
}




vector<pair<int, int>> Knight::possibleCaptures(const pair<int, int> &posn, Board &board, const bool secondcall){
    // Define variables
    vector<pair<int, int>> possibleCaptures; // Define variable for possible captures
    pair<int, int> posn2; // Define variable for posn2 to check moves
    Tile *piece; // Define variable to store what is at that tile
    int currTurn = board.getCurrTurn(); // Define a variable to store who's turn it is
    bool incheck = false; // Define a variable to store whether you are in check once a simulated move is made
    vector<pair<int, int>> directions = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}}; // Define a variable to store the directions a piece can move

    // Iterate through all captures a knight can make
    for(const pair<int, int> &dir : directions){
        posn2 = posn; // Reset the position of the piece

        posn2.first += dir.first; // Update the x coordinate
        posn2.second += dir.second; // Update the y coordinate

        // Ensure that the piece is still in bounds
        if (posn2.first < 0 || posn2.first > 7 || posn2.second < 0 || posn2.second > 7) break;
        
        piece = board.getTile(posn2); // Get the tile stored at posn2 of the board

        // If this isn't the recursive call of possibleCaptures
        if (!secondcall){
            // Simulate the move on a temporary board
            incheck = simulateMove(posn, posn2, board, piece);
        }

        if(!incheck){
            if(piece->getSymbol() != ' ' && piece->getSymbol() != '_'){
                if(currTurn % 2 == 0){ // White turn
                    if (islower(piece->getSymbol())){
                        possibleCaptures.push_back(posn2);
                    } 
                    break;
                } else { // Black turn
                    if (isupper(piece->getSymbol())){
                        possibleCaptures.push_back(posn2);
                    } 
                    break;
                }
            }
        }

        incheck = false; // reset incheck
    }

    // Return the possibleCaptures
    return possibleCaptures;
}




vector<pair<int, int>> Knight::possibleChecks(const pair<int, int> &posn, Board &board){
    // Define variables
    vector<pair<int, int>> possibleChecks; // Define variable for possible checks
    vector<pair<int, int>> possiblemoves = possibleMoves(posn, board); // Define variable for possible moves
    vector<pair<int, int>> possiblecaptures; // Define variable for possible captures
    Tile *piece; // Define variable to store what is at that tile
    int currTurn = board.getCurrTurn(); // Define a variable to store who's turn it is
    char opponentKing = (currTurn % 2 == 0) ? 'k' : 'K'; // Define variable for the opponent king character

    // Iterate through possiblemoves of knight one by one
    for (const pair<int, int> &move: possiblemoves){
        Board tempboard = board;

        // Simulate the move
        piece = tempboard.getTile(move);
        tempboard.removeTile(move);
        tempboard.addTile(piece->getSymbol(), move);
        tempboard.removeTile(posn);

        // Iterate through each possible capture and determine if a possible capture is opponentKing
        possiblecaptures = possibleCaptures(move, tempboard, false);
        for(const pair<int, int> &capture : possiblecaptures){
            piece = tempboard.getTile(capture);
            if(piece->getSymbol() == opponentKing){
                possibleChecks.push_back(move);
                break; // Can not check multiple times in the same position
            }
        }
    }

    // Return the possibleChecks
    return possibleChecks;
}

