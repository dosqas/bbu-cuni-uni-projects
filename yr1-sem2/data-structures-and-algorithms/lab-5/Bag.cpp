#include "Bag.h"
#include "BagIterator.h"
#include <exception>
#include <iostream>
#include <cmath>
using namespace std;

int Bag::hashFunction(TElem key) {
	return std::abs(key) % this->m;
}
// WC = BC = TC = Theta(1), since we only return the remainder of the division between key and m

Bag::Bag() {
	this->h = std::bind(&Bag::hashFunction, this, std::placeholders::_1);
	this->m = 11;
	this->n = 0;
	this->T = new Node[this->m];
	for (int i = 0; i < this->m; i++) {
		this->T[i].key = NULL_TELEM;
		this->T[i].next = nullptr;
	}
}
// WC = BC = TC = Theta(m), where m is the number of elements in the hash table

int Bag::nextPrime(int n) {
	if (n <= 1)
		return 2;
	bool found = false;
	while (!found) {
		n++;
		bool isPrime = true;
		for (int i = 2; i <= n / 2; i++) {
			if (n % i == 0) {
				isPrime = false;
				break;
			}
		}
		if (isPrime)
			found = true;
	}
	return n;
}
// WC = O(n*loglogn), when the next prime number is quite far from n (n*loglogn is the time complexity of the sieve of Eratosthenes)
// BC = O(n), when the next prime number is very close to n
// TC = O(n*loglogn), since the distance to next prime number varies

void Bag::resize() {
	int copyOfm = this->m;
	this->m = this->nextPrime(this->m * 2);
	Node* copyOfT = new Node[this->m];
	for (int i = 0; i < this->m; i++) {
		copyOfT[i].key = NULL_TELEM;
		copyOfT[i].next = nullptr;
	}
	for (int i = 0; i < copyOfm; i++) {
		if (this->T[i].key != NULL_TELEM) {
			int position = (this->h)(this->T[i].key);
			if (copyOfT[position].key == NULL_TELEM) {
				copyOfT[position].key = this->T[i].key;
				copyOfT[position].next = nullptr;
			}
			else {
				Node* newNode = new Node;
				newNode->key = this->T[i].key;
				newNode->next = copyOfT[position].next;
				copyOfT[position].next = newNode;
			}
		}
		Node* current = this->T[i].next;
		while (current != nullptr) {
			int position = (this->h)(current->key);
			if (copyOfT[position].key == NULL_TELEM) {
				copyOfT[position].key = current->key;
				copyOfT[position].next = nullptr;
			}
			else {
				Node* newNode = new Node;
				newNode->key = current->key;
				newNode->next = copyOfT[position].next;
				copyOfT[position].next = newNode;
			}
			Node* next = current->next;
			delete current;
			current = next;
		}
	}
	delete[] this->T;
	this->T = copyOfT;
}
// WC = O(n^2*loglogn + m), when the next prime number is quite far from m (n*loglogn is the time complexity of the sieve of
// Eratosthenes)
// BC = O(n + m), when the next prime number is very close to m
// TC = O(n^2*loglogn + m), since the distance to next prime number varies

void Bag::add(TElem elem) {
	if (this->n / this->m > 0.7)
		this->resize();

	int position = (this->h)(elem);
	if (this->T[position].key == NULL_TELEM) {
		this->T[position].key = elem;
		this->T[position].next = nullptr;
	}
	else {
		Node* newNode = new Node;
		newNode->key = elem;
		newNode->next = this->T[position].next;
		this->T[position].next = newNode;
	}
	this->n++;
}
// WC = O(n^2*loglogn + m), when we need resizing and the next prime number is quite far from m 
// BC = Theta(1), when we do not need resizing
// TC = O(n^2*loglogn + m), since the distance to next prime number varies

bool Bag::remove(TElem elem) {
	int position = (this->h)(elem);
	if (this->T[position].key == elem) {
		if (this->T[position].next == nullptr) {
			this->T[position].key = NULL_TELEM;
		}
		else {
			Node* next = this->T[position].next;
			this->T[position].key = next->key;
			this->T[position].next = next->next;
			delete next;
		}
		this->n--;
		return true;
	}
	else {
		Node* current = this->T[position].next;
		Node* previous = &this->T[position];
		while (current != nullptr) {
			if (current->key == elem) {
				previous->next = current->next;
				delete current;
				this->n--;
				return true;
			}
			previous = current;
			current = current->next;
		}
	}
	return false;
}
// WC = Theta(n), when all elements are on the same position in the hash table
// BC = Theta(1), when the element is not in the bag and its position is empty or when the element is the first in the chain of nodes
// TC = O(n), since usually all elements are not on the same position in the hash table

bool Bag::search(TElem elem) const {
	Node* currentNode = &this->T[(this->h)(elem)];
	while (currentNode != nullptr) {
		if (currentNode->key == elem)
			return true;
		currentNode = currentNode->next;
	}
	return false;
}
// WC = Theta(n), when all elements are on the same position in the hash table
// BC = Theta(1), when the element is not in the bag and its position is empty
// TC = O(n), since usually all elements are not on the same position in the hash table

int Bag::nrOccurrences(TElem elem) const {
	int count = 0;
	Node* currentNode = &this->T[(this->h)(elem)];
	while (currentNode != nullptr) {
		if (currentNode->key == elem)
			count++;
		currentNode = currentNode->next;
	}
	return count;
}
// WC = Theta(n), when all elements are on the same position in the hash table
// BC = Theta(1), when the element is not in the bag and its position is empty
// TC = O(n), since usually all elements are not on the same position in the hash table

int Bag::size() const {
	return this->n;
}
// WC = BC = TC = Theta(1), since we only return n (the number of elements in the bag)

bool Bag::isEmpty() const {
	return this->n == 0;
}
// WC = BC = TC = Theta(1), since we only check if n is 0 or not

BagIterator Bag::iterator() const {
	return BagIterator(*this);
}
// WC = Theta(m), when all chosen operations have the worst complexity
// BC = Theta(1), when all chosen operations have the best complexity
// TC = O(m), since the operations are chosen randomly and can have any complexity

Bag::~Bag() {
	for (int i = 0; i < this->m; i++) {
		Node* current = this->T[i].next;
		while (current != nullptr) {
			Node* next = current->next;
			delete current;
			current = next;
		}
	}
	delete[] this->T;
}
// WC = BC = TC = Theta(n), where n is the number of elements in the bag
