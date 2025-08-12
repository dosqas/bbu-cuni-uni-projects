#include "MultiMapIterator.h"
#include "MultiMap.h"


MultiMapIterator::MultiMapIterator(const MultiMap& c): col(c) {
	this->currentElem = c.head;
}
// WC = BC = TC = Theta(1), since we just initialize the iterator with the head of the list

TElem MultiMapIterator::getCurrent() const{
	if (!valid())
		throw std::exception();
	return this->col.elements[this->currentElem].info;
}
// WC = BC = TC = Theta(1), since we just return the element at the current position

bool MultiMapIterator::valid() const {
	if (this->currentElem == -1)
		return false;
	return true;
}
// WC = BC = TC = Theta(1), since we just check if the current element is valid

void MultiMapIterator::next() {
	if (!valid())
		throw std::exception();
	this->currentElem = this->col.elements[this->currentElem].next;
}
// WC = BC = TC = Theta(1), since we just move to the next element

void MultiMapIterator::first() {
	this->currentElem = this->col.head;
}
// WC = BC = TC = Theta(1), since we just move to the first element
