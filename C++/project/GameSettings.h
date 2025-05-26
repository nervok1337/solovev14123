#ifndef GAMESETTINGS_H
#include <iostream>
#include <cstdlib>

//обрабатывает аргументы командной строки для установки пользовательских параметров
class GameSettings {
public:
    int rows;
    int cols;
    int mines;

    GameSettings();

    void parseArguments(int argc, char* argv[]);
};
#endif // !GAMESETTINGS_H

