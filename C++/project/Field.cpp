#include "Field.h"

//создаёт поле заданного размера, инициализирует ячейки объектами Cell, устанавливает начальные состояния
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
//освобождает память, удаляя все объекты ячеек
Field::~Field() {
	for (auto& row : field) {
		for (auto& cellPtr : row) {
			delete cellPtr;
			cellPtr = nullptr;
		}
	}
	field.clear();
}

//рандомно размещает мины, избегая области вокруг координат safeX, safeY 
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

//подсчитывает количество мин вокруг каждой клетки, не являющейся миной
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

//проверяет, находятся ли координаты внутри границ поля
bool Field::inBounds(int x, int y) const {
	return (0 <= x && x < rows) && (0 <= y && y < cols);
}

//рекурсивно открывает соседние клетки без мин
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

//открывает клетку
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
//ставит или снимает флаг на клетке
void Field::toggleFlag(int x, int y) {
	if (!inBounds(x, y) || field[x][y]->getIsOpen())
		return;

	field[x][y]->toggleFlag();
}

//возвращает, выиграна ли игра
bool Field::isWon() const {
	return won;
}
//возвращает, завершена ли игра
bool Field::isGameOver() const {
	return gameOver;
}

//отображает игровое поле в консоли
void Field::display() const {
	system("cls");

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	const WORD colorDefault = 7;      // светло-серый (для пустых открытых ячеек)
	const WORD colorHeader = 9;       // синий (для заголовков)
	const WORD colorHidden = 2;       // зелёный (скрытые ячейки)
	const WORD colorNumber = 14;      // жёлтый (числа)
	const WORD colorMine = 12;        // ярко-красный (бомбы)
	const WORD colorFlag = 13;        // светло-красный / фиолетовый (флаги)

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

