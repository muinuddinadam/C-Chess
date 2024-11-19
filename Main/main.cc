#include "game.h"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "window.h"
#include "human.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"

int main() {
    //grab iputs repeatedly looking for "setup" or "game __ __"
    Xwindow *window = new Xwindow(700, 700);
    Game game = Game{nullptr, nullptr, window};
    while (true) {

        while (true) {
            string input;
            getline(cin, input);
            if (input == "setup") {
                game.setupGame();
                if (game.getFinish()) {
                    delete window;
                    return 0;
                }
            } else {
                std::stringstream tokenize(input);
                std::vector<string> args;
                string arg;
                while(getline(tokenize, arg, ' ')) {
                    args.push_back(arg);
                }
                if (args.size() == 0){ 
                    game.finish();
                    delete window;
                    return 0;
                }
                if (args.size() != 3) {
                    cout << "Invalid Command: incorrect number of arguments" << endl;
                    continue;
                }
                if (args.at(0) != "game") {
                    cout << "Invalid Command: not a valid command" << endl;
                    continue;
                }

                for (int i = 1; i < 3; ++i) {
                    string colour = "white";
                    if (i == 2) colour = "black";

                    if (args.at(i) == "human" ) {
                        Human *player = new Human{colour};
                        game.getPlayer(i) = player;
                    } else if (args.at(i) == "computer1") {
                        Level1 *player = new Level1{colour};
                        game.getPlayer(i) = player;
                    } else if (args.at(i) == "computer2") {
                        Level2 *player = new Level2{colour};
                        game.getPlayer(i) = player;
                    } else if (args.at(i) == "computer3") {
                        Level3 *player = new Level3{colour};
                        game.getPlayer(i) = player;
                    }
                }
                if (game.getPlayer(1) == nullptr || game.getPlayer(2) == nullptr) {
                    cout << "Invalid Command: incorrect player names" << endl;
                    delete game.getPlayer(1);
                    delete game.getPlayer(2);
                    continue;
                }
                cout << "____________________________________________________________________" << endl;
                game.startGame();
                if (game.getFinish()) {
                    delete window;
                    return 0;
                } else {
                    break;
                }
            }
        }
        
    }
}
