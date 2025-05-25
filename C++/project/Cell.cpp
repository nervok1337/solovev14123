#include "Cell.h"

Cell::Cell() {
	isOpen = false;
	isFlagged = false;
	mineCount = 0;
}

void Cell::open() {
	isOpen = true;
}
void Cell::toggleFlag() {
	isFlagged = !isFlagged;
}

bool Cell::isMine() const {
	return false;
}

void Cell::setMines(int count) {
	mineCount = count;
}
int Cell::getMines() const {
	return mineCount;
}
bool Cell::getIsOpen() const {
	return isOpen;
}
bool Cell::getIsFlagged() const {
	return isFlagged;
}