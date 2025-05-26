#ifndef BOMB_H
#define BOMB_H
#include "Cell.h"

//Наследуется от класса Cell и переопределяет поведение для обозначения, что клетка — мина
class Bomb : public Cell {
public:
	Bomb();

	bool isMine() const override;
};

#endif // !BOMB_H
