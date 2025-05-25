#include "Bomb.h"

Bomb::Bomb() : Cell() {}

bool Bomb::isMine() const{
	return true;
}