#include <exception>
#include "BagIterator.h"
#include "Bag.h"
using namespace std;

BagIterator::BagIterator(const Bag& c): bag(c)
{
	this->currentPos = 0;
	while (this->currentPos < this->bag.m && this->bag.T[this->currentPos].key == NULL_TELEM)
		this->currentPos++;
	
	if (this->currentPos < this->bag.m)
		this->currentNode = &this->bag.T[this->currentPos];
}
// WC = Theta(m), when the first element is the last one in the hash table or the hash table is empty
// BC = Theta(1), when the first element is the first one in the hash table
// AC = O(m), since the first element can be anywhere in the hash table

void BagIterator::first() {
	this->currentPos = 0;
	while (this->currentPos < this->bag.m && this->bag.T[this->currentPos].key == NULL_TELEM)
		this->currentPos++;

	if (this->currentPos < this->bag.m)
		this->currentNode = &this->bag.T[this->currentPos];
}
// WC = Theta(m), when the first element is the last one in the hash table or the hash table is empty
// BC = Theta(1), when the first element is the first one in the hash table
// AC = O(m), since the first element can be anywhere in the hash table

void BagIterator::next() {
	if (!this->valid())
		throw exception();

	if (this->currentNode->next != nullptr) {
		this->currentNode = this->currentNode->next;
	}
	else {
		this->currentPos++;
		if (this->currentPos < this->bag.m) {
			while (this->currentPos < this->bag.m && this->bag.T[this->currentPos].key == NULL_TELEM)
				this->currentPos++;
			if (this->currentPos < this->bag.m)
				this->currentNode = &this->bag.T[this->currentPos];
			else
				this->currentNode = nullptr;
		}
		else {
			this->currentNode = nullptr;
		}
	}
}

// WC = Theta(m), when currentPos points to the first element and the next element is the last one in the hash table
// BC = Theta(1), when currentPos points to the first element and the next element is the second one in the hash table or in the chain
// AC = O(m), since the next element can be anywhere in the hash table

bool BagIterator::valid() const {
	if (this->currentNode == nullptr)
		return false;
	if (this->currentNode->key == NULL_TELEM)
		return false;
	return true;
}
// WC = BC = AC = Theta(1), since we only check if the current node is NULL or not

TElem BagIterator::getCurrent() const {
	if (!this->valid())
		throw exception();
	return this->currentNode->key;
}
// WC = BC = AC = Theta(1), since we only return the current node's key
