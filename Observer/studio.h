#ifndef STUDIO_H
#define STUDIO_H
#include <iostream>
#include "subject.h"
#include "board.h"

class Board;

class Studio : public Subject {
  Board *theBoard;

 public:
  explicit Studio( Board* board ): theBoard{board} {}

  Board*& board() { return theBoard; }
  void render();

  ~Studio();
};

#endif
