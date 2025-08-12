#include "ListIterator.h"
#include "IndexedList.h"
#include <exception>

ListIterator::ListIterator(const IndexedList& list) : list(list){
   this->iter_node = list.head;
}
// WC = BC = TC = Theta(1), since we just initialize the node the iterator points to with the head of the list

void ListIterator::first(){
    this->iter_node = list.head;
}
// WC = BC = TC = Theta(1), since we just set the node the iterator points at to be the head of the list

void ListIterator::next(){
    if (!valid()) {
        throw std::exception();
    }
    this->iter_node = this->iter_node->next;
}
// WC = BC = TC = Theta(1), since we just set the node the iterator points at to point to the next node in the list

bool ListIterator::valid() const{
    return this->iter_node != NULL;
}
// WC = BC = TC = Theta(1), since we just check if the node the iterator points to is not NULL

TElem ListIterator::getCurrent() const{
    if (!valid()) {
        throw std::exception();
    }
   return this->iter_node->info;
}
// WC = BC = TC = Theta(1), since we just return the info from the current node the iterator points to
