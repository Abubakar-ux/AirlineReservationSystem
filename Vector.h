#pragma once
#include <iostream>
using namespace std;
template<class T>
class Vector {

	T* arr;
	int size;
	int numElems;

public:
	Vector();
	Vector(const Vector& obj); //Copy constructor.
	void push(T value);
	void push(T value, int index);
	int getVal(int index);
	bool find(T val);
	void pop();
	T front();
	int Size();
	void display();
	void clear();
	void reverse();
	~Vector();
	const Vector operator=(const Vector& right);
	T& operator[](int index);
};

template<class T>
Vector<T>::Vector() {
	size = 1;
	arr = new T[size];
	numElems = 0;
}

template<class T>
Vector<T>::Vector(const Vector& obj) {
	size = obj.size;
	numElems = obj.numElems;
	arr = new T[size];

	for (int i = 0; i < size; i++)
		arr[i] = obj.arr[i];
}

template<class T>
void Vector<T>::push(T value) {
	if (numElems == size) {
		T* temp = new T[2 * size];

		for (int i = 0; i < size; i++) {
			temp[i] = arr[i];
		}

		delete[] arr;
		size *= 2;
		arr = temp;
	}

	arr[numElems] = value;
	numElems++;
}

template<class T>
void Vector<T>::push(T value, int index) {
	if (index == numElems)
		push(value);
	else
		arr[numElems] = value;
}

template<class T>
int Vector<T>::getVal(int index) {
	if (index < numElems)
		return arr[index];
}

template<class T>
bool Vector<T>::find(T val) {
	for (int i = 0; i < numElems; i++)
		if (val == arr[i])
			return true;

	return false;
}

template<class T>
void Vector<T>::pop() {
	if (numElems > 0)
	{
		numElems--;
	}
}

template<class T>
inline T Vector<T>::front()
{
	if (numElems != 0)
		return arr[numElems - 1];
}

template<class T>
int Vector<T>::Size() {
	return numElems;
}

template<class T>
void Vector<T>::display() {
	for (int i = 0; i < numElems; i++)
		cout << arr[i] << " ";
}

template<class T>
Vector<T>::~Vector() {
	delete[] arr;
	size = 0;
	numElems = 0;
}

template<class T>
const Vector<T> Vector<T>::operator=(const Vector<T>& right) {
	if (this != &right) {
		delete[] arr;
		size = right.size;
		numElems = right.numElems;
		arr = new T[size];
		for (int i = 0; i < size; i++)
			arr[i] = right.arr[i];
	}

	return *this;
}

template<class T>
T& Vector<T>::operator[](int index) {
	if (index < 0 || index >= size) {
		cout << "ERROR: Subscript out of range..." << endl;
		exit(0);
	}
	return arr[index];
}

template<class T>
void Vector<T>::clear() {
	delete[] arr;
	size = 1;
	numElems = 0;

	arr = new T[size];
}

template<class T>
void Vector<T>::reverse() {
	T* temp = new T[size];
	int idx = numElems - 1;
	for (int i = 0; i < numElems; i++) {
		temp[i] = arr[idx];
		idx--;
	}

	delete[] arr;
	arr = temp;
}
