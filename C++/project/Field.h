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
	int rows;
	int cols;
	int mineCount;
	int openedCells;

	vector<vector<Cell*>> field;

	bool firstMove;
	bool won;
	bool gameOver;

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
