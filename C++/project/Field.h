#ifndef FIELD_H
#define FIELD_H
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <windows.h>

#include "Bomb.h"
#include "Cell.h"
using namespace std;

class Field{
private:
	int rows; //количество строк игрового поля
	int cols; //количество столбцов игрового поля
	int mineCount; //количество мин на поле
	int openedCells; //количество уже открытых ячеек

	vector<vector<Cell*>> field; //двумерный вектор указателей на объекты Cell 

	bool firstMove; //флаг, указывающий, был ли сделан первый ход
	bool won; //флаг победы
	bool gameOver; //флаг окончания игры

	void generateMines(int safeX, int safeY);
	void calculateNeighbourMines();
	bool inBounds(int x, int y) const;
	void openSafeCells(int x, int y);


public:
	Field(int rows, int cols, int mineCount);
	~Field();

	void openCell(int x, int y);
	void toggleFlag(int x, int y);
	bool isWon() const;
	bool isGameOver() const;
	void display() const;
};

#endif // !FIELD_H
