#include "history.h"

//NOTE:
//
//we need access to a board pointer to history class
//also undo()'s castling or other fancy moves should return empty vector to symbolize no need for makeMove()




void History::addHistory(std::pair<int, int> beg, std::pair<int, int> end, char taken, bool promo) {
	moveHistory.push(std::make_pair(beg, end));
	tileHistory.push(taken);
	promoHistory.push(promo);
}

void History::undo(Board *board, Studio *studio, std::vector<Observer *> obs) {
	if (moveHistory.empty()) {
		return;
	}
	std::pair<std::pair<int, int>, std::pair<int, int>> move = moveHistory.top();
	moveHistory.pop();


	string beg;
	beg +=  static_cast<char>(move.second.first + 'a');
	beg += to_string(move.second.second + '0');
	string end;
	end += static_cast<char>(move.first.first + 'a');
	end += to_string(move.first.second + '0');
	
	char tile = tileHistory.top();
	bool promo = promoHistory.top();
	tileHistory.pop();
	promoHistory.pop();

	if (tile == 'c' || tile == 'C') {
		//return king and rook to original positions
		if (end == "e1" && beg == "g1") {
			board->addTile('K',move.first);
			board->addTile('R', make_pair(7, 0));
			board->removeTile(make_pair(6, 0));
			board->removeTile(make_pair(5, 0));

			studio->attach(obs.at(8 * move.first.second + move.first.first + 1));
			studio->attach(obs.at(8));
			studio->attach(obs.at(7));
			studio->attach(obs.at(6));
			studio->render();
			studio->detach(obs.at(8));
			studio->detach(obs.at(7));
			studio->detach(obs.at(6));


		} else if (end == "e1" && beg == "c1") {
			board->addTile('K', move.first);
			board->addTile('R', make_pair(0, 0));
			board->removeTile(make_pair(2, 0));
            board->removeTile(make_pair(3, 0));
			studio->attach(obs.at(8 * move.first.second + move.first.first + 1));
			studio->attach(obs.at(1));
			studio->attach(obs.at(3));
			studio->attach(obs.at(4));
			studio->render();
			studio->detach(obs.at(8 * move.first.second + move.first.first + 1));
			studio->detach(obs.at(1));
			studio->detach(obs.at(3));
			studio->detach(obs.at(4));

		} else if (end == "e8" && beg == "g8") {
			board->addTile('k',move.first);
            board->addTile('r', make_pair(7, 7));
            board->removeTile(make_pair(6, 7));
            board->removeTile(make_pair(5, 7));
			studio->detach(obs.at(8 * move.first.second + move.first.first + 1));
			studio->attach(obs.at(8 * 7 + 8));
			studio->attach(obs.at(8 * 7 + 7));
			studio->attach(obs.at(8 * 7 + 6));
			studio->render();
			studio->detach(obs.at(8 * move.first.second + move.first.first + 1));
			studio->detach(obs.at(8 * 7 + 8));
			studio->detach(obs.at(8 * 7 + 7));
			studio->detach(obs.at(8 * 7 + 6));


		} else if (end == "e8" && beg=="g8") {
			board->addTile('k', move.first);
            board->addTile('r', make_pair(0, 7));
            board->removeTile(make_pair(2, 7));
            board->removeTile(make_pair(3, 7));
			studio->detach(obs.at(8 * move.first.second + move.first.first + 1));
			studio->attach(obs.at(8 * 7 + 1));
			studio->attach(obs.at(8 * 7 + 3));
			studio->attach(obs.at(8 * 7 + 4));
			studio->render();
			studio->detach(obs.at(8 * move.first.second + move.first.first + 1));
			studio->detach(obs.at(8 * 7 + 1));
			studio->detach(obs.at(8 * 7 + 3));
			studio->detach(obs.at(8 * 7 + 4));
		}
	} else if (tile == 'e' || tile == 'E') {
		int x = move.second.first;
		int y = move.first.second;
		board->makeMove(move.second, move.first);
		studio->attach(obs.at(8 * move.second.second + move.second.first + 1));
		studio->attach(obs.at(8 * move.first.second + move.first.first + 1));
		studio->attach(obs.at(8 * y + x + 1));
		if (board->getTile(move.first)->getSymbol() == 'p') {
			board->addTile('P', make_pair(x, y));
			studio->detach(obs.at(8 * y + x + 1));
		} else {
			board->addTile('p', make_pair(x, y));
		}
		studio->render();
		studio->detach(obs.at(8 * move.second.second + move.second.first + 1));
		studio->detach(obs.at(8 * move.first.second + move.first.first + 1));
		studio->detach(obs.at(8 * y + x + 1));

	} else if (promo) {//promotion
		board->makeMove(move.second, move.first);
		studio->attach(obs.at(8 * move.first.second + move.first.first + 1));
		studio->attach(obs.at(8 * move.second.second + move.second.first + 1));
		if (tile != ' ') {
			board->addTile(tile, move.second);
		}
		if (board->getTile(move.first)->getSymbol() > 'z') {
			board->addTile('P', move.first);
		} else {
			board->addTile('p', move.first);
		}	
		studio->render();
		studio->detach(obs.at(8 * move.first.second + move.first.first + 1));
		studio->detach(obs.at(8 * move.second.second + move.second.first + 1));
	} else {
		board->makeMove(move.second, move.first);
		if (tile != ' ') {
			board->addTile(tile, move.second);
		}	
		studio->attach(obs.at(8 * move.first.second + move.first.first + 1));
		studio->attach(obs.at(8 * move.second.second + move.second.first + 1));
		studio->render();
		studio->detach(obs.at(8 * move.first.second + move.first.first + 1));
		studio->detach(obs.at(8 * move.second.second + move.second.first + 1));
	}
}


