#include "Cell.h"

//Конструктор инициализирует закрытую ячейку без флага и с нулём мин вокруг
Cell::Cell() {
	isOpen = false;
	isFlagged = false;
	mineCount = 0;
}

//открывает ячейку
void Cell::open() {
	isOpen = true;
}
//ставит или убирает флаг
void Cell::toggleFlag() {
	isFlagged = !isFlagged;
}

//проверяет, является ли ячейка миной
bool Cell::isMine() const {
	return false;
}
//устанавливает число соседних мин 
void Cell::setMines(int count) {
	mineCount = count;
}
//возвращает количество мин вокруг ячейки
int Cell::getMines() const {
	return mineCount;
}
//проверяет, открыта ли ячейка
bool Cell::getIsOpen() const {
	return isOpen;
}
//проверяет, стоит ли флаг
bool Cell::getIsFlagged() const {
	return isFlagged;
}
