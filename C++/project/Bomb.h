#ifndef BOMB_H
#define BOMB_H
#include "Cell.h"

class Bomb : public Cell {
public:
	Bomb();

	bool isMine() const override;
};

#endif // !BOMB_H
