#include "MultiMap.h"
#include "MultiMapIterator.h"
#include <exception>
#include <iostream>
using namespace std;


MultiMap::MultiMap() {
	this->capacityLLA = 10;
	this->sizeLLA = 0;
	this->head = -1;
	this->tail = -1;
	this->firstEmpty = 0;
	this->elements = new Node[this->capacityLLA];
	for (int i = 0; i < this->capacityLLA - 1; i++) {
		this->elements[i].next = i + 1;
		this->elements[i].prev = i - 1;
	}
	this->elements[this->capacityLLA - 1].next = -1;
}
// WC = BC = TC = Theta(1), since all the operations are done in constant time.

void MultiMap::resize() {
	Node* newElements = new Node[this->capacityLLA * 2];
	for (int i = 0; i <= this->capacityLLA - 1; i++) {
		newElements[i].next = this->elements[i].next;
		newElements[i].prev = this->elements[i].prev;
		newElements[i].info = this->elements[i].info;
	}
	for (int i = this->capacityLLA; i <= this->capacityLLA * 2 - 2; i++) {
		newElements[i].next = i + 1;
		newElements[i].prev = i - 1;
	}
	newElements[this->capacityLLA * 2 - 1].next = -1;
	newElements[this->capacityLLA * 2 - 1].prev = this->capacityLLA * 2 - 2;
	delete[] this->elements;
	this->elements = newElements;
	this->firstEmpty = this->capacityLLA;
	this->capacityLLA *= 2;
}
// WC = BC = TC = Theta(n), since we have to copy all the elements from the old linked list array to the new one and then 
// set a few links that are done in constant time.


void MultiMap::add(TKey c, TValue v) {
	if (this->firstEmpty == -1) {
		this->resize();
	}
	int newPosition = this->firstEmpty;
	this->elements[newPosition].info.first = c;
	this->elements[newPosition].info.second = v;
	this->firstEmpty = this->elements[this->firstEmpty].next;
	if (this->head == -1) {
		this->head = newPosition;
		this->tail = newPosition;
		this->elements[newPosition].next = -1;
		this->elements[newPosition].prev = -1;
	}
	else {
		this->elements[newPosition].next = this->head;
		this->elements[newPosition].prev = -1;
		this->elements[this->head].prev = newPosition;
		this->head = newPosition;
	}
	this->sizeLLA++;
}
// WC = Theta(n), when the linked list array is full and we have to resize it, the other operations being done in constant time.
// BC = Theta(1), when the linked list array is not full (so no resize), the add part being done in constant time.
// TC = Theta(1) amortized, because the worst case is when the linked list array is full and we have to resize it, but it can happen that the linked
// list array is not full, the add part being done in constant time. Resizing is less and less frequent as the size of the linked list array increases.

bool MultiMap::remove(TKey c, TValue v) {
	int current = this->head;
	while ((this->elements[current].info.first != c || this->elements[current].info.second != v) && current != -1) {
		current = this->elements[current].next;
	}
	if (current == -1) {
		return false;
	}
	else {
		if (current == this->head) {
			this->head = this->elements[current].next;
			if (this->head != -1) {
				this->elements[this->head].prev = -1;
			}
			else {
				this->tail = -1;
			}
		}
		else if (current == this->tail) {
			this->tail = this->elements[current].prev;
			this->elements[this->tail].next = -1;
		}
		else {
			this->elements[this->elements[current].prev].next = this->elements[current].next;
			this->elements[this->elements[current].next].prev = this->elements[current].prev;
		}
		this->elements[current].next = this->firstEmpty;
		this->firstEmpty = current;
		this->sizeLLA--;
		return true;
	}
}
// WC = Theta(n), when the element to be removed is the last one in the linked list array or when the element is not in the linked 
// list array.
// BC = Theta(1), when the element to be removed is the first one in the linked list array.
// TC = O(n), because the worst case is when the element to be removed is the last one in the linked list array or when the 
// element is not in the linked list array, but it can happen that it is found earlier in the linked list array.

vector<TValue> MultiMap::search(TKey c) const {
	vector<TValue> v;
	int current = this->head;
	while (current != -1) {
		if (this->elements[current].info.first == c) {
			v.push_back(this->elements[current].info.second);
		}
		current = this->elements[current].next;
	}
	return v;
}
// WC = BC = TC = Theta(n), since we have to iterate through the entire linked list array every time because a key can have multiple
// values.


int MultiMap::size() const {
	return this->sizeLLA;
}
// WC = BC = TC = Theta(1), since we just return the value of a variable.

bool MultiMap::isEmpty() const {
	return (this->sizeLLA == 0);
}
// WC = BC = TC = Theta(1), since we just return the result of a comparison.

MultiMapIterator MultiMap::iterator() const {
	return MultiMapIterator(*this);
}
// WC = BC = TC = Theta(1), since all the operations of the iterator are done in constant time.

MultiMap::~MultiMap() {
	delete[] this->elements;
}
// WC = BC = TC = Theta(1), since delete[] is considered to be done in constant time.
