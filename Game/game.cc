#include "game.h"
#include <sstream>
#include <utility>
#include <vector>
#include "player.h"
#include "window.h"
#include "text.h"
#include "studio.h"
#include "history.h"
#include "graphics.h"

Player *&Game::getPlayer(int playerNum) {
	if (playerNum == 1) return whitePlayer;
	return blackPlayer;
}

Game::Game (Player *white, Player *black, Xwindow *window): whitePlayer{white}, blackPlayer{black}, board{new Board}, history{new History}, studio{new Studio{board}}, window{window} {
	cout << "____________________________________________________________________" << endl;
	cout << endl << "Welcome to Chess! Please choose from the following commands:" << endl;
	cout << "'game <white-player> <black-player>' starts a new game. The parameters <white-player> and <black-player> can be either human or computer[1-4]." << endl;
	cout << "	eg. 'game human computer2' starts a game with the White player being Human and the Black player being a computer." << endl;
	cout << "'resign' concedes the game to your opponent. This is the only way, outside of winning or drawing the game, to end a game." << endl;
	cout << "'move <start> <end>' moves one of the pieces on the board. The parameters <start> and <end> specify the starting and ending coordinates of the piece to be moved." << endl;
	cout << "	eg. 'move e2 e4' moves the piece on e2 to e4" << endl;
	cout << "	Castling would specified by the two-square move for the king." << endl;
	cout << "		eg. 'move e1 g1' (right castle) or 'move e1 c1' (left castle) for white." << endl;
	cout << "	Pawn promotion would additionally specify the piece type to which the pawn is promoted." << endl;
	cout << "		eg. 'move e7 e8 Q' moves a Pawn from e7 to e8 (the other end of the board), and subsequently makes it a Queen" << endl;
	cout << "	In the case of a computer player, the command move (without arguments) makes the computer player make a move" << endl;
	cout << "'setup' enters setup mode, within which you can set up your own initial board configurations. This can only be done when a game is not currently running. Within setup mode, the following language is used:" << endl;
	cout << "	'+ <piece> <square>' places the piece onto the board. Specifically, it places <piece> onto <square>. If a piece is already on that square, it is replaced." << endl;
	cout << "		eg. '+ K e1' places the piece K (i.e., white king in this case) on the square e1" << endl;
	cout << "	'- <square>' removes the piece from the parameter <square>. If there is no piece at that square, nothing happens." << endl;
	cout << "		eg. '- e1' remvoes the piece from e1" << endl;
	cout << "	'= <colour>' makes it colour’s turn to go next." << endl;
	cout << "		eg. '= black' makes it the Black player's turn to go next." << endl << endl;
	cout << "***Remember*** White Pieces = UPPERCASE, Black Pieces = lowercase" << endl;
	cout << "____________________________________________________________________" << endl;
	
	Text *textScreen = new Text{studio};

	obs.push_back(textScreen);
	studio->attach(textScreen);
	textScreen->notify();

	for (int x = 0; x < 8; ++x) {
		for (int y = 0; y < 8; ++y) {
			Graphics *tileScreen = new Graphics{x, y, studio, window};
			obs.push_back(tileScreen);
			//studio->attach(tileScreen);
			tileScreen->notify();
		}
	}
}

Game::~Game() {
	delete board;
	delete history;
	//delete whitePlayer;
	//delete blackPlayer;
	delete studio;
	for (auto it = obs.begin(); it != obs.end(); ++it) {
      delete *it;
  	}
	if (final) {
		printScore();
	}
}

void Game::simplePrint(std::pair<int, int> beg, std::pair<int, int> end) {

	studio->attach(obs.at(8 * beg.first + beg.second + 1));
	studio->attach(obs.at(8 * end.first + end.second + 1));
	studio->render();
	studio->detach(obs.at(8 * beg.first + beg.second + 1));
	studio->detach(obs.at(8 * end.first + end.second + 1));
}

void Game::startGame() {
	while (!board->checkMate() && !board->staleMate()) {
		if (board->getCurrTurn() % 2 == 0) // White's turn
			cout << endl << "White's Turn: " << endl;
		else 
			cout << endl << "Black's Turn: " << endl;

		//this will determine who is the current player and get their move from them
		std::vector<string> input;


		if(board->getCurrTurn()%2 == 0) {
			input = whitePlayer->getMove(*board);
		} else {
			input = blackPlayer->getMove(*board);
		}
		if (input.size() == 0) {
			finish();
			return;
		}
		//if the word is resign, give the opposite Player the win
		if (input.size() < 2 && input.at(0) == "resign") {
			break;
		} else if (input.size() < 2 && input.at(0) == "undo") {
			history->undo(board, studio, obs);
			continue;
		}

		if (input.size() != 3 && input.size() != 4) {
			cout << "Improper command: incorrect number of arguments" << endl;
			continue;
		}

		if (input.at(0) != "move") {
			cout << "Improper command: available commands are move or resign" << endl;
			continue;
		}
	
		if (input.at(1).size() != 2) {
			cout << "Improper command: bad starting coordinate" << endl;
			continue;
		}
		int x1 = (input.at(1))[0] - 'a';
		int y1 = (input.at(1))[1] - '1';
		std::pair<int, int> beg = std::make_pair(x1, y1);
		
		if (x1 < 0 || x1 > 7 || y1 < 0 || y1 > 7) {//improper coord
			cout << "Improper command: bad starting coordinate" << endl;
			continue;
		}

		
		if(input.at(2).size() != 2) {
			cout << "Improper command: bad ending coordinate" << endl;
			continue;
		}
		int x2 = (input.at(2))[0] - 'a';
		int y2 = (input.at(2))[1] - '1';
		std::pair<int, int> end = std::make_pair(x2, y2);
		
		if (x2 < 0 || x2 > 7 || y2 < 0 || y2 > 7) { //improper coord
			cout << "Improper command: bad ending coordinate" << endl;
			continue;
		}

		char returned;
		if (input.size() == 3) {
			returned = board->makeMove(beg, end);

			if (returned != 'X') { //character represents a failed move
				history->addHistory(beg, end, returned, false);
			} else  {
				cout << "Improper command: move must be legal" << endl;
				continue;
			}


			if (returned == 'e' || returned == 'E') {
				studio->attach(obs.at(beg.second * 8 + end.first - beg.first * 2 + 1));
				simplePrint(beg, end);
				studio->detach(obs.at(beg.second * 8 + end.first - beg.first * 2 + 1));
			} else if (returned == 'c' || returned == 'C') {
				if (beg.first > end.first) {
					//short castle
					studio->attach(obs.at(beg.second * 8 + beg.first + 3));
					studio->attach(obs.at(beg.second * 8 + beg.first + 5));
					simplePrint(beg, end);
					studio->detach(obs.at(beg.second * 8 + beg.first + 3));
					studio->detach(obs.at(beg.second * 8 + beg.first + 5));
				} else {
					//long castle
					studio->attach(obs.at(beg.second * 8 + beg.first));
					studio->attach(obs.at(beg.second * 8 + beg.first - 3));
					simplePrint(beg, end);
					studio->detach(obs.at(beg.second * 8 + beg.first));
					studio->detach(obs.at(beg.second * 8 + beg.first - 3));
				}
			} else {
				simplePrint(beg, end);
			}
			continue;
		}

		if (input.at(3).size() != 1) {
			cout << "Improper command: must be a character representation of a pawn promotion" << endl;
			continue;
		}

		returned = board->makeMove(beg, end, (input.at(3))[0]);
		if (returned != 'X') {
			simplePrint(beg, end);
			history->addHistory(beg, end, returned, true);
		} else {
			cout << "Improper command: move must be legal" << endl;
			continue;
			
		}

    	cout << "____________________________________________________________________" << endl;
		board->setCurrTurn(board->getCurrTurn() + 1);
	}

	if (board->checkMate()) {
		cout << endl << "Checkmate" << endl << endl;	
		window->drawString(400, 400, "Checkmate!", Xwindow::Black);

		if (board->getCurrTurn()%2 == 0) {
			++blackWins;
			cout << "Black has won!" << endl;
			window->drawString(400, 400, "Black Wins!", Xwindow::Black);
		} else {
			++whiteWins;
			cout << "White has won!" << endl;
			window->drawString(400, 400, "White Wins!", Xwindow::Black);
		}
	} else if (board->staleMate()) {
		whiteWins += 0.5;
		blackWins += 0.5;
		cout << endl << "Stalemate" << endl << endl;
		cout << "It's a tie!" << endl;
		window->drawString(70, 70, "Stalemate!", Xwindow::Black);	
	} else {
		if (board->getCurrTurn()%2 == 0) {
			++blackWins;
			cout << endl << "Black has won!" << endl;
			window->drawString(400, 400, "Black Wins!", Xwindow::Black);
		} else {
			++whiteWins;
			cout << endl << "White has won!" << endl;
			window->drawString(400, 400, "White Wins!", Xwindow::Black);
		}
	}
}

void Game::setupGame() {
	string input;

	while (getline(cin, input)) {
		stringstream tokenize(input);
		string arg1;
		if (!getline(tokenize, arg1, ' ')) {
			cout << "Improper command: no input given" << endl;
			continue;		
		}
		
		if (arg1 == "done") {
			if(checkValid()) {
				cout << "Exiting setup mode" << endl;
				return;
			} else {
				cout << "Invalid board: there must be one of each king, and no pawns in the outer rows" << endl;
				continue;
			}
		}

		string arg2;
		if (!getline(tokenize, arg2, ' ')) {
			cout << "Improper command: no second argument" << endl;
			continue;
		}

		if (arg1 == "+" && arg2.size() == 1) {
			string arg3;
			if (!getline(tokenize, arg3, ' ') || arg3.size() != 2) {
				cout << "Improper command: no coordinates" << endl;
				continue;
			}

			int x = arg3[0] - 'a';
			int y = arg3[1] - '1';
			std::pair<int, int> location = std::make_pair(x, y);
			if (x < 0 || x > 7 || y < 0 || y > 7) {
				cout << "Improper command: bad coordinate" << endl;
				continue;
			}
			board->addTile(arg2[0], location);
			studio->attach(obs.at(location.first + 8 * location.second + 1));
			studio->render();
			studio->detach(obs.at(location.first + 8 * location.second + 1));
		} else if (arg1 == "-") {
			int x = arg2[0] - 'a';
			int y = arg2[1] - '1';
			std::pair<int, int> location = std::make_pair(x, y);
			if (x < 0 || x > 7 || y < 0 || y > 7) {
				cout << "Improper command: bad coordinate" << endl;
				continue;
			}
			board->removeTile(location);
			studio->attach(obs.at(location.first + 8 * location.second + 1));
			studio->render();
			studio->detach(obs.at(location.first + 8 * location.second + 1));
		} else if (arg1 == "="){
			if (arg2 == "black") {
				board->setCurrTurn(1);
			} else if (arg2 == "white") {
				board->setCurrTurn(0);
			} else {
				cout << "Improper command: no valid player given" << endl;
				continue;
			}
		} else {
			cout << "Improper command: Invalid beginning command or format" << endl;
		}
	}
}

//player class needs to hold a win counter
void Game::printScore() {
	cout << endl << "Final Score:" << endl;
	cout << "White: " << whiteWins << endl;
	cout << "Black: " << blackWins << endl;	
}

Board *&Game::getBoard() {
	return board;
}

bool Game::checkValid() {
	int whiteKings = 0;
	int blackKings = 0;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			char tile = (board->getTile(make_pair(i, j)))->getSymbol();
			if (tile == 'k') {
				++blackKings;	
			} else if (tile == 'K') {
				++whiteKings;
			} else if ((j == 0 || j == 7) && (tile == 'p' || tile == 'P')) {
				return false;
			}
		}	
	}
	if (blackKings && blackKings < 2 && whiteKings && whiteKings < 2) {
		return true;
	}
	return false;
}
