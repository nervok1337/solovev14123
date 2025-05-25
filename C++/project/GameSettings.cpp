#include "GameSettings.h"

GameSettings::GameSettings() : rows(9), cols(9), mines(10) {}

int toInt(const char* str, int defaultValue) {
    if (str == nullptr || *str == '\0')
        return defaultValue;
    return std::atoi(str);
}

void GameSettings::parseArguments(int argc, char* argv[]) {
    if (argc > 1) 
        rows = toInt(argv[1], rows);
    if (argc > 2) 
        cols = toInt(argv[2], cols);
    if (argc > 3) 
        mines = toInt(argv[3], mines);

    int maxMines = (rows * cols) / 2;
    if (mines > maxMines) {
        std::cout << "Слишком много мин! Установлено максимальное допустимое количество: " << maxMines << '\n';
        mines = maxMines;
    }

    if (rows < 1) rows = 9;
    if (cols < 1) cols = 9;
    if (mines < 1) mines = 10;
}