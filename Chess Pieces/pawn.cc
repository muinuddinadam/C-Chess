#include "pawn.h"
#include "board.h"
#include <locale>
#include <iostream>
using namespace std;

vector<pair<int, int>> Pawn::possibleMoves(const pair<int, int> &posn, Board &board){
    // Define variables
    vector<pair<int, int>> possiblemoves; // Define variable for possible moves
    pair<int, int> posn2; // Define variable for posn2 to check moves
    Tile *piece; // Define variable to store what is at that tile
    int currTurn = board.getCurrTurn(); // Define a variable to store who's turn it is
    bool incheck = false; // Define a variable to store whether you are in check once a simulated move is made
    vector<pair<int, int>> directions; // Define a variable to store the directions a piece can move

    // Determine who's turn it is and set directions accordingly
    if (currTurn % 2 == 0){
        directions = {{0, 1}, {1, 1}, {-1, 1}, {0, 2}}; 
    } else {
        directions = {{0, -1}, {1,-1}, {-1, -1}, {0, -2}}; 
    }

    // Iterate through all moves the pawn can make
    for(const pair<int, int> &dir : directions){
        posn2 = posn; // Reset the position of the piece 

        if (moved && (dir.second == 2 || dir.second == -2)) continue; // If the pawn has moved, do not check the double forward case

        posn2.first += dir.first; // Update the x coordinate
        posn2.second += dir.second; // Update the y coordinate

        // Ensure that the piece is still in bounds
        if (posn2.first < 0 || posn2.first > 7 || posn2.second < 0 || posn2.second > 7) continue;
        
        piece = board.getTile(posn2); // Get the tile stored at posn2 of the board

        // Simulate the move on a temporary board
        incheck = simulateMove(posn, posn2, board, piece);

        if(!incheck){
            if(dir.first != 0){ // Possible captures
                if(piece->getSymbol() != ' ' && piece->getSymbol() != '_'){
                    if(currTurn % 2 == 0){ // White turn
                        if(islower(piece->getSymbol())){
                            possiblemoves.push_back(posn2);
                        } continue;
                    } else { // Black turn
                        if(isupper(piece->getSymbol())){
                        possiblemoves.push_back(posn2);
                        } continue; 
                    }
                }
            } else if(piece->getSymbol() == ' ' || piece->getSymbol() == '_'){ // Possible forward moves
                possiblemoves.push_back(posn2);
            } continue;
        }
    }

    // Check if an En Passant is available
    possibleEnPassant(posn, board, possiblemoves);

    // Return the possibleMoves
    return possiblemoves;
}




vector<pair<int, int>> Pawn::possibleCaptures(const pair<int, int> &posn, Board &board, bool secondcall){
    // Define variables
    vector<pair<int, int>> possibleCaptures; // Define variable for possible captures
    pair<int, int> posn2 = posn; // Define variable for posn2 to check moves
    Tile *piece; // Define variable to store what is at that tile
    int currTurn = board.getCurrTurn(); // Define a variable to store who's turn it is
    bool incheck = false; // Define a variable to store whether you are in check once a simulated move is made
    vector<pair<int, int>> directions; // Define a variable to store the directions a piece can move

    if (currTurn % 2 == 0){
        directions = {{1, 1}, {-1, 1}}; 
    } else {
        directions = {{1,-1}, {-1, -1}}; 
    }
    // Iterate through all captures a pawn can make
    for(const pair<int, int> &dir : directions){
        posn2 = posn; // Reset the position of the piece

        posn2.first += dir.first; // Update the x coordinate
        posn2.second += dir.second; // Update the y coordinate

        // Ensure that the piece is still in bounds
        if (posn2.first < 0 || posn2.first > 7 || posn2.second < 0 || posn2.second > 7) continue;
        
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
                    continue;
                } else { // Black turn
                    if (isupper(piece->getSymbol())){
                        possibleCaptures.push_back(posn2);
                    } 
                    continue;
                }
            }
        }
        incheck = false; // reset incheck
    }

    // Return the possibleCaptures
    return possibleCaptures;
}




vector<pair<int, int>> Pawn::possibleChecks(const pair<int, int> &posn, Board &board){
    // Define variables
    vector<pair<int, int>> possibleChecks; // Define variable for possible checks
    vector<pair<int, int>> possiblemoves = possibleMoves(posn, board); // Define variable for possible moves
    vector<pair<int, int>> possiblecaptures; // Define variable for possible captures
    Tile *piece; // Define variable to store what is at that tile
    int currTurn = board.getCurrTurn(); // Define a variable to store who's turn it is
    char opponentKing = (currTurn % 2 == 0) ? 'k' : 'K'; // Define variable for the opponent king character
    // Here or in the for loop?
    // Board tempboard = board; // Define a variable for a copy of the board that you can use to simulate moves

    // Iterate through possiblemoves of pawn one by one
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

void Pawn::possibleEnPassant(const pair<int, int> &posn, Board &board, vector<pair<int, int>> &possiblemoves){
    // Define variables
    pair<int, int> posn2; // Define variable for posn2 to check moves
    Tile *piece; // Define a variable to store a tile pointer
    int currTurn = board.getCurrTurn(); // Define a variable to store who's turn it is
    vector<pair<int, int>> directions; // Define a variable to store the directions that pawn can move
    bool incheck = false; // Define a variable to store whether you are in check once a simulated move is made
    char oppPawn = (currTurn % 2 == 0) ? 'p' : 'P'; // Define a variable to store the opponent pawn char
    
    // Assign the directions based on current move
    if (currTurn % 2 == 0){
        directions = {{1, 1}, {-1, 1}}; 
    } else {
        directions = {{1,-1}, {-1, -1}}; 
    }

    // Iterate through the directions and check if an en passant is available
    for(const pair<int, int> &dir : directions){
        posn2 = posn; 
        
        posn2.first += dir.first;
        //posn2.second +=dir.second;

        // Ensure that the piece is still in bounds
        if (posn2.first < 0 || posn2.first > 7 || posn2.second < 0 || posn2.second > 7) continue;

        piece = board.getTile(posn2); // Tile to the left or the right of the pawn

        posn2.second += dir.second;
        
        // Simulate the move on a temporary board
        incheck = simulateMove(posn, posn2, board, piece);

        posn2.second = posn.second;

        if(!incheck){
            if(piece->getSymbol() == oppPawn){
                if (piece->getJustMoved() && piece->getJustMoved2()){
                    posn2.second += dir.second;
                    possiblemoves.push_back(posn2);
                }
            }
        }
    }
   
    
}
