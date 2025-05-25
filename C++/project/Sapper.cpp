#include "Field.h"
#include "GameSettings.h"
#include <cstdlib>

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    GameSettings settings;
    settings.parseArguments(argc, argv);

    Field field(settings.rows, settings.cols, settings.mines);

    while (true) {
        field.display();

        cout << "Игровое поле: " << settings.rows << "x" << settings.cols << ", мин: " << settings.mines << endl;
        cout << "Введите действие (o - открыть, f - флаг) и координаты x y: ";
        char action;
        int x, y;
        cin >> action >> x >> y;

        if (action == 'o') {
            field.openCell(x, y);
        }
        else if (action == 'f') {
            field.toggleFlag(x, y);
        }
        else {
            cout << "Неизвестное действие\n";
            continue;
        }

        if (field.isWon()) {
            field.display();
            cout << "Поздравляем! Вы выиграли!\n";
            break;
        }

        if (field.isGameOver()) {
            field.display();
            cout << "Вы проиграли!\n";
            break;
        }
    }

    return 0;
}