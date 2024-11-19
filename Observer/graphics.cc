#include "graphics.h"
#include <sstream>

Graphics::Graphics(int x, int y, Studio* studio, Xwindow* window)
    : x{x}, y{y}, studio{studio}, window{window} {}

void Graphics::notify() {
    if (!displayed) {
        displayLabels();
        displayed = true;
    }

    displayTile();
}

void Graphics::displayLabels() {
    window->fillRectangle(0, 0, window->getWidth(), window->getHeight(), Xwindow::Burlywood);
    // draw the board
    for (int x = 0; x <= 7; ++x) {
        window->drawString((border / 2), (((7 - x) * tileSize) + (tileSize / 2) + border), to_string(x + 1), Xwindow::Black); // row labels
        window->drawString(((x * tileSize) + (tileSize / 2) + border), (window->getHeight() - (border / 2)), string(1, ('A' + x)), Xwindow::Black); // column labels
        window->drawString((window->getWidth() - (border / 2)), (((7 - x) * tileSize) + (tileSize / 2) + border), to_string(x + 1), Xwindow::Black); // row labels
        window->drawString(((x * tileSize) + (tileSize / 2) + border), (0 + (border / 2)), string(1, ('A' + x)), Xwindow::Black); // column labels
    }
}

void Graphics::displayTile() {
    auto board = studio->board()->getBoard();
    char piece = board[x][y]->getSymbol();

    // draw the empty Tiles
    if ((x % 2) == (y % 2)) window->fillRectangle(((x * tileSize) + border), (((7 - y) * tileSize) + border), tileSize, tileSize, Xwindow::Brown); // black Tiles
    else window->fillRectangle(((x * tileSize) + border), (((7 - y) * tileSize) + border), tileSize, tileSize, Xwindow::White); // white Tiles

    if (piece != ' ' && piece != '_') {
        if (islower(piece)) window->drawString(((x * tileSize) + (tileSize / 2) + border), (((7 - y) * tileSize) + (tileSize / 2) + border), string(1, piece), Xwindow::Black); // black Pieces
        else if (isupper(piece)) window->drawString(((x * tileSize) + (tileSize / 2) + border), (((7 - y) * tileSize) + (tileSize / 2) + border), string(1, piece), Xwindow::Grey); // white Pieces
    }
}
