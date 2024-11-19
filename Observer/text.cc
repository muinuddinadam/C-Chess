#include "text.h"
#include <iostream>
using namespace std;

Text::Text(Studio* studio) : studio{studio} {}

void Text::notify() {
    auto board = studio->board()->getBoard();
    cout << endl << "   abcdefgh" << endl << endl; // print column numbers
    for (int y = 7; y >= 0; --y) {
        cout << (y + 1) << "  "; // print row numbers
		for (int x = 0; x <= 7; ++x) {
            cout << board[x][y]->getSymbol(); // print the symbol of each tile
		}
        cout << "  " << (y + 1); // print row numbers
        cout << endl;
	}
    cout << endl << "   abcdefgh" << endl; // print column numbers

}
