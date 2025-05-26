#ifndef CELL_H
#define CELL_H

class Cell {
private:
	bool isOpen; //открыта ли ячейка
	bool isFlagged; //стоит ли на ячейке флаг
	int mineCount; //количество мин вокруг этой ячейки

	void setMines(int count);
public:
	Cell();

	void open();
	void toggleFlag();

	virtual bool isMine() const;

	int getMines() const;
	bool getIsOpen() const;
	bool getIsFlagged() const;

	friend class Field;
};

#endif // !CELL_H

