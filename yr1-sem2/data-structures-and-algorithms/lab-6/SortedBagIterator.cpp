#include "SortedBagIterator.h"
#include "SortedBag.h"
#include <exception>

using namespace std;

SortedBagIterator::SortedBagIterator(const SortedBag& b) : bag(b) {
	this->current = 0;
	this->values = new TComp[b.size()];
	this->inOrder(b.root);
	this->current = 0;
}
// WC = BC = TC = Theta(n), since we have to go through all the nodes in the tree for inOrder

TComp SortedBagIterator::getCurrent() {
	if (!this->valid()) {
		throw exception();
	}
	
	return this->values[this->current];
}
// WC = BC = TC = Theta(1), since we only check if valid and return the current element

bool SortedBagIterator::valid() {
	return this->current < this->bag.size();
}
// WC = BC = TC = Theta(1), since we only check if the current is less than the size of the bag

void SortedBagIterator::next() {
	if (!this->valid()) {
		throw exception();
	}
	this->current++;
}
// WC = BC = TC = Theta(1), since we only increment the current

void SortedBagIterator::first() {
	this->current = 0;
}
// WC = BC = TC = Theta(1), since we only set the current to 0

void SortedBagIterator::inOrder(SortedBag::BSTNode* node) {
	if (node == nullptr) {
		return;
	}
	
	this->inOrder(node->left);
	int freq = 0;
	while (node->frequency > freq){
		this->values[this->current++] = node->element;
		freq++;
	}
	this->inOrder(node->right);
}
// WC = Theta(n), when we only have one node in the tree
// BC = Theta(1), when the node has a frequency of 1
// TC = O(n), since usually there is not only one node in the tree
