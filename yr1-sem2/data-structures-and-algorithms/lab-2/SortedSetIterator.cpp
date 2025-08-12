#include "SortedSetIterator.h"
#include <exception>

using namespace std;

SortedSetIterator::SortedSetIterator(const SortedSet& m) : multime(m)
{
	this->current = 0;
}
// WC = BC = TC = Theta(1), since we only have to initialize the current index as being 0

void SortedSetIterator::first() {
	this->current = 0;
}
// WC = BC = TC = Theta(1), since we only have to set the current index to 0

void SortedSetIterator::next() {
	if (!valid())
		throw exception();
	this->current++;
}
// WC = BC = TC = Theta(1), since we only have to increment the current index

TElem SortedSetIterator::getCurrent()
{
	if (!valid())
		throw exception();
	return this->multime.elementsDA[this->current];
}
// WC = BC = TC = Theta(1), since we only have to return the current element the iterator is pointing to

bool SortedSetIterator::valid() const {
	if (this->current < this->multime.sizeDA)
		if (this->current >= 0)
			return true;
	return false;
}
// WC = BC = TC = Theta(1), since we only have to check if current is less than the size of the set

