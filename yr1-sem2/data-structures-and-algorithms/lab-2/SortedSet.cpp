#include "SortedSet.h"
#include "SortedSetIterator.h"

SortedSet::SortedSet(Relation r) {
	this->capacityDA = 1;
	this->sizeDA = 0;
	this->elementsDA = new TComp[this->capacityDA];
	this->relationDA = r;
}
// WC = BC = TC = Theta(1), since we only have to allocate memory for the array with new which is considered to have linear complexity and 
// initialize the size and capacity

bool SortedSet::add(TComp elem) {
	if (this->sizeDA == this->capacityDA) {
		resize();
	}

	int i;
	for (i = 0; i < this->sizeDA; i++) {
		if (this->elementsDA[i] == elem) {
			return false;
		}
		if (this->relationDA(elem, this->elementsDA[i])) {
			break;
		}
	}

	for (int j = this->sizeDA; j > i; j--) {
		this->elementsDA[j] = this->elementsDA[j - 1];
	}

	this->elementsDA[i] = elem;
	this->sizeDA++;
	return true;
}
// WC = BC = TC = Theta(n), since even if we find elem in the set, we still have to shift the elements in the array to the right, 
// thus having to iterate through the entire set. Same if elem is not in the set, we have to iterate through the entire set to check that

void SortedSet::resize() {
	this->capacityDA *= 2;
	TComp* newElements = new TComp[this->capacityDA];
	for (int i = 0; i < this->sizeDA; i++) {
		newElements[i] = this->elementsDA[i];
	}
	delete[] this->elementsDA;
	this->elementsDA = newElements;
}
// WC = BC = TC = Theta(n), since we have to iterate through the entire set to copy the elements in the new array

bool SortedSet::remove(TComp elem) {
	for (int i = 0; i < this->sizeDA; i++) {
		if (this->elementsDA[i] == elem) {
			for (int j = i; j < this->sizeDA - 1; j++) {
				this->elementsDA[j] = this->elementsDA[j + 1];
			}
			this->sizeDA--;
			return true;
		}
	}
	return false;
}
// WC = BC = TC = Theta(n), since if we find elem, we still have to shift the elements in the array to the left, thus having to iterate 
// through the entire set. Same if elem is not in the set, we have to iterate through the entire set to check that

bool SortedSet::search(TComp elem) const {
	int l = 0, r = this->sizeDA - 1;
	while (l <= r) {
		int mij = (l + r) / 2;
		if (this->elementsDA[mij] == elem) {
			return true;
		}
		if (this->relationDA(elem, this->elementsDA[mij])) {
			r = mij - 1;
		}
		else {
			l = mij + 1;
		}
	}

	return false;
}
// WC = Theta(log n), when elem is not in the set and we have to keep searching for it until l > r
// BC = Theta(1), when elem is in the middle of the set
// TC = O(log n), since we have to iterate through the set to find elem but we can find it faster

int SortedSet::size() const {
	return this->sizeDA;
}
// WC = BC = TC = Theta(1), since we only have to return the size of the set(which is stored in memory)

bool SortedSet::isEmpty() const {
	return this->sizeDA == 0;
}
// WC = BC = TC = Theta(1), since we only have to check if sizeDA(which is stored in memory) is 0

SortedSetIterator SortedSet::iterator() const {
	return SortedSetIterator(*this);
}
// WC = BC = TC = Theta(1), since the iterator has all operations in Theta(1)

bool SortedSet::isSubsetOf(SortedSet& s) const {
	if (this->sizeDA > s.sizeDA) {
		return false;
	}

	for (int i = 0; i < this->sizeDA; i++) {
		if (!s.search(this->elementsDA[i])) {
			return false;
		}
	}
	return true;
}
// WC = Theta(n), when the current set is a subset of s
// BC = Theta(1), when the current set is not a subset of s and we tell this from the first element or when the current set is larger than s
// thus it can't be a subset
// TC = O(n), since we have to iterate through the entire set to check if the current set is a subset of s but it can happen that we find
// that the current set is not a subset of s faster

SortedSet::~SortedSet() {
	delete[] this->elementsDA;
}
// WC = BC = TC = Theta(1), since delete[] is considered to have constant complexity
