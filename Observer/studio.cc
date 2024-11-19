#include "studio.h"
#include "board.h"

void Studio::render() {
    notifyObservers();
}

Studio::~Studio() {};
