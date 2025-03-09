#ifndef ARRAY_H
#define ARRAY_H

const int DEFAULT_CAPACITY = 10;

class ArrayException {};

class Array {
	int size;
	int capacity;
	int* ptr;
public:
	explicit Array(int startCapacity = DEFAULT_CAPACITY);
	~Array();

	Array(const Array& arr);
	Array& operator =(const Array& arr);

	int& operator [](int index);
	void insert(int index, int elem);
	void insert(int elem);
	void increaseCapacity(int newCapacity);
	int getSize();
	int getCapacity();
};

#endif
