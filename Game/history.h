#ifndef HISTORY_H_
#define HISTORY_H_
#include <utility>
#include <string>
#include "board.h"
#include <vector>
#include <stack>
#include "tile.h"
#include "studio.h"
#include "observer.h"

class History {
	std::stack<std::pair<std::pair<int, int>, std::pair<int, int>>> moveHistory;
	std::stack<char> tileHistory;
	std::stack<bool> promoHistory;
	public:
	void addHistory(std::pair<int, int> beg, std::pair<int, int> end, char taken, bool promo);
	void undo(Board *board, Studio *studio, std::vector<Observer *> obs);
};

#endif
