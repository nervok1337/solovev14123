#ifndef GAMESETTINGS_H
#include <iostream>
#include <cstdlib>

class GameSettings {
public:
    int rows;
    int cols;
    int mines;

    GameSettings();

    void parseArguments(int argc, char* argv[]);
};
#endif // !GAMESETTINGS_H

