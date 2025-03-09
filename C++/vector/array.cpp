#include "Array.h"
#include <iostream>

Array::Array(int startCapacity)
{
    if (startCapacity <= 0)
        capacity = DEFAULT_CAPACITY;
    else
        capacity = startCapacity;
    ptr = new int[capacity];
    size = 0;
}

Array::~Array()
{
    delete[] ptr;
}

Array::Array(const Array& arr) {
    ptr = new int[arr.capacity];
    size = arr.size;
    capacity = arr.capacity;
    for (size_t i = 0; i < size; i++)
        ptr[i] = arr.ptr[i];
}

Array& Array::operator=(const Array& arr) {
    if (this == &arr)
        return *this;

    if (capacity != arr.capacity)
    {
        delete[] ptr;
        ptr = new int[arr.capacity];
        capacity = arr.capacity;
    }

    size = arr.size;
    for (size_t i = 0; i < size; i++)
        ptr[i] = arr.ptr[i];
    return *this;
}

int& Array::operator[](int index) {
    if (index >= capacity || index < 0)
        throw ArrayException();
    else {
        if (index > size)
            size++;
        return ptr[index];
    }
}

void Array::insert(int index, int elem) {

    size++;

    std::cout << size << '\n';
    if (index > capacity || index < 0)
        throw ArrayException();

    if (size == capacity)
        increaseCapacity(size + 1);

    for (size_t i = size - 1; i >= size; i--)
        ptr[i + 1] = ptr[i];

    ptr[index] = elem;
}

void Array::insert(int elem) {
    insert(size, elem);
}

void Array::increaseCapacity(int newCapacity) {
    capacity = newCapacity < capacity * 2 ? capacity * 2 : newCapacity;
    int* newPtr = new int[capacity];
    for (int i = 0; i < size; i++)
        newPtr[i] = ptr[i];
    delete[] ptr;
    ptr = newPtr;
}
int Array::getCapacity() {
    return capacity;
}

int Array::getSize() {
    return size;
}
