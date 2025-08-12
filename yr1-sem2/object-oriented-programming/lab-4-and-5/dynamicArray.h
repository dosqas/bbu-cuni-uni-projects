#pragma once
#include <iostream>
#include "domain.h"

template <typename TElem> class DynamicArray
{
	private:
		TElem* elems;
		int size;
		int capacity;

	public:
		DynamicArray() {
			this->size = 0;
			this->capacity = 1;
			this->elems = new TElem[1];
		}

		DynamicArray(int capacity) {
			this->size = 0;
			this->capacity = capacity;
			this->elems = new TElem[capacity];
		}

		~DynamicArray() {
			delete[] this->elems;
		}

		int getSize() const {
			return this->size;
		}

		int getCapacity() const {
			return this->capacity;
		}

		void setSize(int size) {
			this->size = size;
		}

		void setCapacity(int capacity) {
			this->capacity = capacity;
		}

		void resize() {
			/*
			This function resizes the dynamic array.
			Input:
				- none
			Output:
				- the dynamic array is resized
			*/
			this->capacity *= 2;
			TElem* newElems = new TElem[this->capacity];
			for (int i = 0; i < this->size; i++)
				newElems[i] = this->elems[i];
			delete[] this->elems;
			this->elems = newElems;
		}

		void addElem(TElem elem) {
			/*
			This function adds an element to the dynamic array.
			Input:
				- elem: TElem
			Output:
				- the element is added to the dynamic array
			*/
			if (this->size + 1 == this->capacity)
				this->resize();
			this->elems[this->size++] = elem;
		}

		void removeElem(int pos) {
			/*
			This function removes an element from the dynamic array.
			Input:
				- pos: int
			Output:
			- the element is removed from the dynamic array
			*/
			this->elems[pos] = this->elems[this->size - 1];
			this->size--;
		}

		TElem* getAllElems() const {
			return this->elems;
		}

		TElem& operator[](int pos) {
			return this->elems[pos];
		}
};