#include "Field.h"

Field::Field(int rows, int cols, int mineCount) {
	this->rows = rows;
	this->cols = cols;
	this->mineCount = mineCount;
	gameOver = false;
	firstMove = true;
	won = false;
	openedCells = 0;

	field.resize(rows);
	for (int x = 0; x < rows; x++) {
		field[x].resize(cols);
		for (int y = 0; y < cols; y++) {
			field[x][y] = new Cell();
		}
	}
}

Field::~Field() {
	for (auto& row : field) {
		for (auto& cellPtr : row) {
			delete cellPtr;
			cellPtr = nullptr;
		}
	}
	field.clear();
}

void Field::generateMines(int safeX, int safeY) {
	static bool seeded = false;
	if (!seeded) {
		srand(static_cast<unsigned int>(time(nullptr)));
		seeded = true;
	}

	int placedMines = 0;
	while (placedMines < mineCount) {
		int x = rand() % rows;
		int y = rand() % cols;

		if (abs(x - safeX) <= 1 && abs(y - safeY) <= 1)
			continue;

		if (field[x][y]->isMine())
			continue;

		delete field[x][y];
		field[x][y] = new Bomb();
		placedMines++;
	}
}

void Field::calculateNeighbourMines() {
	for (int x = 0; x < rows; x++) {
		for (int y = 0; y < cols; y++) {
			if (field[x][y]->isMine())
				continue;

			int minesAround = 0;

			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					if (dx == 0 && dy == 0)
						continue;

					int nx = x + dx;
					int ny = y + dy;

					if (inBounds(nx, ny) && field[nx][ny]->isMine())
						minesAround++;
				}
			}
			field[x][y]->setMines(minesAround);
		}
	}
}

bool Field::inBounds(int x, int y) const {
	return (0 <= x && x < rows) && (0 <= y && y < cols);
}

void Field::openSafeCells(int x, int y) {
	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			if (dx == 0 && dy == 0)
				continue;

			int nx = x + dx;
			int ny = y + dy;

			if (!inBounds(nx, ny))
				continue;

			Cell* neighbour = field[nx][ny];
			if (neighbour->getIsOpen() || neighbour->getIsFlagged())
				continue;

			neighbour->open();
			openedCells++;

			if (neighbour->getMines() == 0)
				openSafeCells(nx, ny);
		}
	}
}

void Field::openCell(int x, int y) {
	if (!inBounds(x, y) || gameOver)
		return;
	if (firstMove) {
		generateMines(x, y);
		calculateNeighbourMines();
		firstMove = false;
	}

	if (field[x][y]->getIsOpen() || field[x][y]->getIsFlagged())
		return;

	field[x][y]->open();
	openedCells++;
	if (field[x][y]->isMine()) {
		gameOver = true;
		return;
	}
	
	if (field[x][y]->getMines() == 0)
		openSafeCells(x, y);

	if (openedCells == rows * cols - mineCount) {
		gameOver = true;
		won = true;
	}
}
void Field::toggleFlag(int x, int y) {
	if (!inBounds(x, y) || field[x][y]->getIsOpen())
		return;

	field[x][y]->toggleFlag();
}

bool Field::isWon() const {
	return won;
}
bool Field::isGameOver() const {
	return gameOver;
}

#include <windows.h>
#include <iomanip>

void Field::display() const {
	system("cls");

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	const WORD colorDefault = 7;      // white-gray (for empty opened cells)
	const WORD colorHeader = 9;       // blue (for headers)
	const WORD colorHidden = 2;       // green (hidden cells)
	const WORD colorNumber = 14;      // yellow (numbers)
	const WORD colorMine = 12;        // red-bright (bombs)
	const WORD colorFlag = 13;        // white-red / purple (flags)

	SetConsoleTextAttribute(hConsole, colorHeader);
	cout << "   ";
	for (int y = 0; y < cols; ++y) {
		cout << setw(3) << y;
	}
	cout << '\n';

	for (int x = 0; x < rows; ++x) {
		SetConsoleTextAttribute(hConsole, colorHeader);
		cout << setw(3) << x;

		for (int y = 0; y < cols; ++y) {
			Cell* cell = field[x][y];

			if (cell->getIsOpen()) {
				if (cell->isMine()) {
					SetConsoleTextAttribute(hConsole, colorMine);
					cout << setw(3) << '*';
				}
				else {
					int mines = cell->getMines();
					if (mines > 0) {
						SetConsoleTextAttribute(hConsole, colorNumber);
						cout << setw(3) << mines;
					}
					else {
						SetConsoleTextAttribute(hConsole, colorDefault);
						cout << "   ";
					}
				}
			}
			else if (cell->getIsFlagged()) {
				SetConsoleTextAttribute(hConsole, colorFlag);
				cout << setw(3) << 'F';
			}
			else {
				SetConsoleTextAttribute(hConsole, colorHidden);
				cout << setw(3) << '#';
			}
		}
		cout << '\n';
	}

	SetConsoleTextAttribute(hConsole, colorDefault);
}

