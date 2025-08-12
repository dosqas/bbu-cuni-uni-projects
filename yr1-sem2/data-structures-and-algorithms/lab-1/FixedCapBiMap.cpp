#include "FixedCapBiMap.h"
#include "FixedCapBiMapIterator.h"
#include <iostream>

FixedCapBiMap::FixedCapBiMap(int capacity) {
	if (capacity <= 0) {
		throw exception();
	}
	this->capacity = capacity;
	this->mapSize = 0;
	this->elements = new TElem[capacity];
}
//WC/AC/BC = Theta(1)

FixedCapBiMap::~FixedCapBiMap() {
	delete[] this->elements;
}
//WC/TC/BC = Theta(1)

bool FixedCapBiMap::add(TKey c, TValue v){
	if (isFull()) {
		throw exception();
	}
	int count = 0, index = 0;
	while (count < 2 && index < this->mapSize) {
		if (this->elements[index].first == c) {
			count++;
		}
		index++;
	}
	if (count == 2) {
		return false;
	}
	this->elements[this->mapSize].first = c;
	this->elements[this->mapSize].second = v;
	this->mapSize++;
	return true;
}
//WC = Theta(mapSize) - when there are 0 or 1 keys equal to c in the map
//TC = O(mapSize)
//BC = Theta(1) - when the first two keys from the map are equal to c

ValuePair FixedCapBiMap::search(TKey c) const{
	ValuePair result;
	result.first = NULL_TVALUE;
	result.second = NULL_TVALUE;
	int count = 0, index = 0;
	while (index < this->mapSize && count < 2)
	{
		if (this->elements[index].first == c) {
			if (count == 0) {
				result.first = this->elements[index].second;
				count++;
			}
			else {
				result.second = this->elements[index].second;
				count++;
			}
		}
		index++;
	}
	return result;
}
// WC = Theta(mapSize) - when there are 0 or 1 keys equal to c in the map
// TC = O(mapSize)
// BC = Theta(1) - when the first two keys from the map are equal to c

bool FixedCapBiMap::remove(TKey c, TValue v){
	int index = 0;
	while (index < this->mapSize) {
		if (this->elements[index].first == c && this->elements[index].second == v) {
			this->elements[index] = this->elements[this->mapSize - 1];
			this->mapSize--;
			return true;
		}
		index++;
	}
	return false;
}
// WC = Theta(mapSize) - when the element to be removed is the last one or it is not in the map
// TC = O(mapSize)
// BC = Theta(1) - when the first element from the map is the one to be removed


int FixedCapBiMap::size() const {
	return (this->mapSize);
}
//WC/TC/BC = Theta(1)

bool FixedCapBiMap::isEmpty() const{
	return (this->mapSize == 0);
}
//WC/TC/BC = Theta(1)

bool FixedCapBiMap::isFull() const {
	return (this->mapSize == this->capacity);
}
//WC/TC/BC = Theta(1)

int FixedCapBiMap::countUniqueKeys() const {
	int count = 0, index = 0;
	while (index < this->mapSize) {
		ValuePair result = search(this->elements[index].first);
		if (result.second == NULL_TVALUE)
			count++;
		index++;
	}
	return count;
}
//WC/TC/BC = Theta(mapSize^2)

FixedCapBiMapIterator FixedCapBiMap::iterator(){
	return FixedCapBiMapIterator(*this);
}
//WC/TC/BC = Theta(1)
