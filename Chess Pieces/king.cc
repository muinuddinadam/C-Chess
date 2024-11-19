#include "king.h"
#include "board.h"
#include <locale>
#include <iostream>
using namespace std;

vector<pair<int, int>> King::possibleMoves(const pair<int, int> &posn, Board &board){
    // Define variables
    vector<pair<int, int>> possiblemoves; // Define variable for possible moves
    pair<int, int> posn2; // Define variable for posn2 to check moves
    Tile *piece; // Define variable to store what is at that tile
    int currTurn = board.getCurrTurn(); // Define a variable to store who's turn it is
    bool incheck = false; // Define a variable to store whether you are in check once a simulated move is made
    vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}}; // Define a variable to store the directions a piece can move

    // Iterate through all moves the king can make
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
                        possiblemoves.push_back(posn2);
                    } 
                    continue;
                } else { // Black turn
                    if(isupper(piece->getSymbol())){
                        possiblemoves.push_back(posn2);
                    } 
                    continue; 
                }
            }
            possiblemoves.push_back(posn2);
        }
    }

    // Determine if castling is possible
    possibleCastles(posn, board, possiblemoves);

    // Return the possiblemoves
    return possiblemoves;
}




vector<pair<int, int>> King::possibleCaptures(const pair<int, int> &posn, Board &board, const bool secondcall){
    // Define variables
    vector<pair<int, int>> possiblecaptures; // Define variable for possible captures
    pair<int, int> posn2; // Define variable for posn2 to check moves
    Tile *piece; // Define variable to store what is at that tile
    int currTurn = board.getCurrTurn(); // Define a variable to store who's turn it is
    bool incheck = false; // Define a variable to store whether you are in check once a simulated move is made
    vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}}; // Define a variable to store the directions a piece can move

    // Iterate through all captures a king can make
    for(const pair<int, int> &dir : directions){
        posn2 = posn; // Reset the position of the piece
        posn2.first += dir.first; // Update the x coordinate
        posn2.second += dir.second; // Update the y coordinate

        // Ensure that the piece is still in bounds
        if (posn2.first < 0 || posn2.first > 7 || posn2.second < 0 || posn2.second > 7) break;
        
        piece = board.getTile(posn2); // Get the tile stored at posn2 of the board

        // If this isn't the recursive call of possiblecaptures
        if (!secondcall){
            // Simulate the move on a temporary board
            incheck = simulateMove(posn, posn2, board, piece);
        }

        if(!incheck){
            if(piece->getSymbol() != ' ' && piece->getSymbol() != '_'){
                if(currTurn % 2 == 0){ // White turn
                    if (islower(piece->getSymbol())){
                        possiblecaptures.push_back(posn2);
                    } 
                    break;
                } else { // Black turn
                    if (isupper(piece->getSymbol())){
                        possiblecaptures.push_back(posn2);
                    } 
                    break;
                }
            }
        }

        incheck = false; // reset incheck
    }

    // Return the possiblecaptures
    return possiblecaptures;
}

vector<pair<int, int>> King::possibleChecks(const pair<int, int> &posn, Board &board){
    vector<pair<int, int>> vec;
    return vec;
}

void King::possibleCastles(const pair<int, int> &posn, Board &board, vector<pair<int, int>> &possiblemoves){
    // Ensure the king has not moved
    if(moved || !((posn.second == 0 || posn.second == 7)) || !(posn.first == 4)) { return; }

    // Define variables
    pair<int, int> posn2; // Define variable for posn2 to check moves
    vector<pair<int, int>> directions = {{1, 0}, {-1, 0}}; // Only directions needed for castling
    Tile *piece = board.getTile(posn);
    int currTurn = board.getCurrTurn(); // Define a variable to store who's turn it is
    bool incheck = false; // Define a variable to store whether you are in check once a simulated move is made

    // Check if the right castle is available
    while(true){
        // Ensure that our rook is to the right of the king
        piece = board.getTile({posn.first + 3, posn.second});
        if((currTurn % 2 == 0 && piece->getSymbol() == 'R') || (currTurn % 2 == 1 && piece->getSymbol() == 'r')){ // Determine the turn
            if(!piece->Moved()){
                // Ensure that there are no pieces between the king and the rook
                piece = board.getTile({posn.first + 1, posn.second});
                if(piece->getSymbol() == ' ' || piece->getSymbol() == '_'){
                    piece = board.getTile({posn.first + 2, posn.second});
                    if(piece->getSymbol() == ' ' || piece->getSymbol() == '_'){
                        piece = board.getTile(posn);
                        // Ensure that the king is not in check in the current position, posn + 1, posn + 2 
                        for(int i = 0; i <= 2; i++){
                            incheck = simulateMove({posn.first, posn.second}, {posn.first + i, posn.second}, board, piece);
                            if(incheck){
                                break;
                            } else if (i == 2 && !incheck){
                                posn2.first = posn.first + 2;
                                posn2.second = posn.second;
                                // Pushback posn2 to possiblemoves
                                possiblemoves.push_back(posn2);
                                // Break the loop
                                break;
                            }
                        }
                    
                    }
                }
            }
        }

        break;
    }

    // Check if left castle is available
    while(true){
        // Ensure that our rook is to the right of the king
        piece = board.getTile({posn.first - 4, posn.second});
        if((currTurn % 2 == 0 && piece->getSymbol() == 'R') || (currTurn % 2 == 1 && piece->getSymbol() == 'r')){ // Determine the turn
            if(!piece->Moved()){
                // Ensure that there are no pieces between the king and the rook
                piece = board.getTile({posn.first - 1, posn.second});
                if(piece->getSymbol() == ' ' || piece->getSymbol() == '_'){
                    piece = board.getTile({posn.first - 2, posn.second});
                    if(piece->getSymbol() == ' ' || piece->getSymbol() == '_'){
                        piece = board.getTile({posn.first - 3, posn.second});
                        if(piece->getSymbol() == ' ' || piece->getSymbol() == '_'){
                            piece = board.getTile(posn);
                            // Ensure that the king is not in check in the current position, posn - 1, posn - 2 
                            for(int i = 0; i <= 2; i++){
                                incheck = simulateMove({posn.first, posn.second}, {posn.first - i, posn.second}, board, piece);
                                if(incheck){
                                    break;
                                } else if (i == 2 && !incheck){
                                    posn2.first = posn.first - 2;
                                    posn2.second = posn.second;
                                    // Pushback posn2 to possiblemoves
                                    possiblemoves.push_back(posn2);
                                    // Break the loop
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        break;
    }
}
