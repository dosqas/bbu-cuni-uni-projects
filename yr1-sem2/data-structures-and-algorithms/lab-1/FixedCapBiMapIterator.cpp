#include "FixedCapBiMap.h"
#include "FixedCapBiMapIterator.h"
#include <exception>
using namespace std;


FixedCapBiMapIterator::FixedCapBiMapIterator(FixedCapBiMap& d) : map(d)
{
	this->currentPosition = 0;
}
//WC/TC/BC = Theta(1)

void FixedCapBiMapIterator::first() {
	this->currentPosition = 0;
}
//WC/TC/BC = Theta(1)

void FixedCapBiMapIterator::next() {
	if (!this->valid())
		throw exception();
	else
		this->currentPosition++;
}
//WC/TC/BC = Theta(1)

TElem FixedCapBiMapIterator::remove() {
	if (!this->valid())
		throw exception();
	else {
		this->map.elements[this->currentPosition] = this->map.elements[this->map.mapSize];
		this->map.mapSize--;
		return this->map.elements[this->currentPosition];
	}
}

TElem FixedCapBiMapIterator::getCurrent(){
	if (!this->valid())
			throw exception();
	else
		return this->map.elements[this->currentPosition];
}
//WC/TC/BC = Theta(1)

bool FixedCapBiMapIterator::valid() const {
	return this->currentPosition < this->map.size();
}
//WC/TC/BC = Theta(1)
