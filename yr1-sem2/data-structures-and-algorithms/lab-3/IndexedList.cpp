#include <exception>

#include "IndexedList.h"
#include "ListIterator.h"

IndexedList::IndexedList() {
    this->head = NULL;
    this->tail = NULL;
    this->sizeSLL = 0;
}
// WC = BC = TC = Theta(1) since we only initialize the head, tail and sizeSLL variables with NULL, NULL and 0 respectively.

int IndexedList::size() const {
    return this->sizeSLL;
}
// WC = BC = TC = Theta(1) since we only return the sizeSLL variable.

bool IndexedList::isEmpty() const {
    return this->head == NULL;
}
// WC = BC = TC = Theta(1) since we only check if the head is NULL.

TElem IndexedList::getElement(int pos) const {
    int current_pos = 0;
    Node* currentNode = this->head;

    if (pos > this->sizeSLL - 1 || pos < 0) {
        throw std::exception();
    }

    if (pos == this->sizeSLL - 1)
        return this->tail->info;

    while (currentNode->next!= NULL && current_pos != pos) {
        currentNode = currentNode->next;
        current_pos++;
    }

    return currentNode->info;
}
// WC = Theta(n), when pos is the second to last valid position in the SLL
// BC = Theta(1), when pos is the first position in the SLL or it is not a valid position in the SLL
// TC = O(n), since we have to iterate through the SLL to find the element at the given position but it can be that pos is on a lower 
// position thus not requiring us to iterate through all of the SLL

TElem IndexedList::setElement(int pos, TElem e) {
    int current_pos = 0;
    Node* currentNode = this->head;
    TElem old_value;

    if (pos > this->sizeSLL - 1 || pos < 0) {
        throw std::exception();
    }

    while (currentNode->next != NULL && current_pos != pos) {
        currentNode = currentNode->next;
        current_pos++;
    }

    old_value = currentNode->info;
	currentNode->info = e;

	return old_value;
}
// WC = Theta(n), when pos is the second to last valid position in the SLL
// BC = Theta(1), when pos is the first position in the SLL or it is not a valid position in the SLL
// TC = O(n), since we have to iterate through the SLL to find the element at the given position but it can be that pos is on a lower
// position thus not requiring us to iterate through all of the SLL

void IndexedList::addToEnd(TElem e) {
    if (this->head == NULL) {
        this->head = new Node();
        this->head->info = e;
        this->head->next = NULL;
        this->tail = this->head;
    }
    else {
        this->tail->next = new Node();
        this->tail = this->tail->next;
        this->tail->info = e;
        this->tail->next = NULL;
    }
    this->sizeSLL++;
}
// WC = BC = TC = Theta(1) since we only add a new node at the end of the SLL and update the tail pointer, and new is considered to be of
// constant time complexity

void IndexedList::addToPosition(int pos, TElem e) {
    int current_pos = 0;
    Node* currentNode = this->head;
    Node* newNode = new Node();
	newNode->info = e;

    if (pos > this->sizeSLL - 1 || pos < 0) {
        throw std::exception();
    }

    while (currentNode->next != NULL && current_pos != pos) {
		currentNode = currentNode->next;
        current_pos++;
	}

    if (pos == 0) {
        newNode->next = this->head;
        this->head = newNode;
    }
    else {
        newNode->next = currentNode->next;
        currentNode->next = newNode;
        if (newNode->next == NULL)
            this->tail = newNode;
    }

    this->sizeSLL++;
}
// WC = Theta(n), when pos is the second to last valid position in the SLL
// BC = Theta(1), when pos is the first position in the SLL or it is not a valid position in the SLL
// TC = O(n), since we have to iterate through the SLL to find the element at the given position but it can be that pos is on a lower
// position thus not requiring us to iterate through all of the SLL

TElem IndexedList::remove(int pos) {
    int current_pos = 0;
    Node* currentNode = this->head;
    Node* prevNode = NULL;
    TElem removedValue;

    if (pos > this->sizeSLL - 1 || pos < 0)
        throw std::exception();

    while (currentNode != NULL && current_pos != pos) {
        prevNode = currentNode;
        currentNode = currentNode->next;
        current_pos++;
    }

    if (currentNode->next == NULL)
        this->tail = prevNode;

    if (currentNode != NULL && prevNode == NULL) {
        this->head = this->head->next;
        this->tail = this->head;
    }
    else if (currentNode != NULL) {
        prevNode->next = currentNode->next;
        currentNode->next = NULL;
    }
    removedValue = currentNode->info;
    delete currentNode;

    this->sizeSLL--;
    return removedValue;
}
// WC = Theta(n), when pos is the second to last valid position in the SLL
// BC = Theta(1), when pos is the first position in the SLL or it is not a valid position in the SLL
// TC = O(n), since we have to iterate through the SLL to find the element at the given position but it can be that pos is on a lower
// position thus not requiring us to iterate through all of the SLL

int IndexedList::search(TElem e) const{
    Node* currentNode = this->head;
    int current_pos = 0;

    while (currentNode->next != NULL) {
		if (currentNode->info == e)
			return current_pos;
		currentNode = currentNode->next;
        current_pos++;
	}

    if (currentNode->info == e)
        return current_pos;
    return -1;
}
// WC = Theta(n), when the element is the last element in the SLL
// BC = Theta(1), when the element is the first element in the SLL or it is not in the SLL
// TC = O(n), since we have to iterate through the SLL to find the element but it can be that the element is on a lower position thus
// not requiring us to iterate through all of the SLL

ListIterator IndexedList::iterator() const {
    return ListIterator(*this);        
}
// WC = BC = TC = Theta(1) since all the operations of the iterator are of constant time complexity Theta(1)

IndexedList::~IndexedList() {
	Node* currentNode = this->head;
    Node* current;

    while (currentNode != NULL) {
		current = currentNode;
		currentNode = currentNode->next;
		delete current;
	}
}
// WC = BC = TC = Theta(n) since we have to delete all the nodes in the SLL, thus we have to iterate through all of the SLL
