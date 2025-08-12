#include "SortedBag.h"
#include "SortedBagIterator.h"
#include <iostream>

SortedBag::SortedBag(Relation r) {
	this->root = nullptr;
	this->relation = r;
	this->sizeOfBag = 0;
}
// WC = BC = TC = Theta(1), since we only initialize the root, the relation and the size of the bag

void SortedBag::add(TComp e) {
	BSTNode* current = this->root;
	
	if (this->root == nullptr) {
		// if the tree is empty, we create a new node and set it as the root
		BSTNode* newNode = new BSTNode();
		newNode->element = e;
		newNode->frequency = 1;
		newNode->left = nullptr;
		newNode->right = nullptr;
		this->root = newNode;
		this->sizeOfBag++;
		return;
	}

	while (current) {
		if (current->element == e) {
			// if the element is already in the tree, we just increment the frequency
			current->frequency++;
			this->sizeOfBag++;
			return;
		}
		else if (this->relation(current->element, e)) {
			// if the relation is true between the current element and the element to be added, we go to the right child
			if (current->right == nullptr) {
				// if there is no right child, we create a new node and set it as the right child
				BSTNode* newNode = new BSTNode();
				newNode->element = e;
				newNode->frequency = 1;
				newNode->left = nullptr;
				newNode->right = nullptr;
				current->right = newNode;
				this->sizeOfBag++;
				return;
			}
			else
				// if there is a right child, we go to the right child
				current = current->right;
		}
		else {
			// if the relation is false between the current element and the element to be added, we go to the left child
			if (current->left == nullptr) {
				// if there is no left child, we create a new node and set it as the left child
				BSTNode* newNode = new BSTNode();
				newNode->element = e;
				newNode->frequency = 1;
				newNode->left = nullptr;
				newNode->right = nullptr;
				current->left = newNode;
				this->sizeOfBag++;
				return;
			}
			else
				// if there is a left child, we go to the left child
				current = current->left;
		}
	}
}
// WC = Theta(n), when the element is added as a leaf and the BST is degenerate
// BC = Theta(1), when we add the element to the root's frequency
// TC = O(n), since the BST is not always degenerate

bool SortedBag::remove(TComp e) {
	if (this->root == nullptr)
		return false;

	BSTNode* current = this->root;
	BSTNode* parent = nullptr;

	while (current != nullptr) {
		if (current->element == e) {
			// if the element is found
			if (current->frequency > 1) {
				// if the element has a frequency greater than 1, we just decrement the frequency
				current->frequency--;
				this->sizeOfBag--;
				return true;
			}
			else {
				// node with no children
				if (current->left == nullptr && current->right == nullptr) {
					if (parent == nullptr) {
						// if it's the root
						delete current;
						this->root = nullptr;
					}
					else {
						if (parent->left == current)
							// if it's a left child, we set the left child to nullptr
							parent->left = nullptr;
						else
							// if it's a right child, we set the right child to nullptr
							parent->right = nullptr;
						delete current;
					}
				}
				// node with one child on the right
				else if (current->left == nullptr) {
					// if it's the root, we set the right child as the new root
					if (parent == nullptr) {
						this->root = current->right;
					}
					else {
						// if it's a left child, we set the parent's left child to the right child of the current node, since the 
						// current node is going to be deleted and it only has a right child
						if (parent->left == current)
							parent->left = current->right;
						else
							// if it's a right child, we set the right child of the parent to the right child of the current node, 
							// since the current node is going to be deleted
							parent->right = current->right;
					}
					delete current;
				}
				// node with one child on the left
				else if (current->right == nullptr) {
					// if it's the root
					if (parent == nullptr) {
						this->root = current->left;
					}
					else {
						if (parent->left == current)
							// if it's a left child, we set the left child of the parent to the left child of the current node, since 
							// the current node is going to be deleted and it only has a left child
							parent->left = current->left;
						else
							// if it's a right child, we set the right child of the parent to the left child of the current node,
							// since the current node is going to be deleted and it only has a left child
							parent->right = current->left;
					}
					delete current;
				}
				else {
					// node with two children
					BSTNode* successorParent = current;
					BSTNode* successor = current->right;

					// we find the inorder successor of the current node. The inorder successor is the leftmost node in the right 
					// subtree
					while (successor->left != nullptr) {
						successorParent = successor;
						successor = successor->left;
					}

					if (successorParent != current) {
						// if the successor's parent is not the current node, meaning that we have found a successor that has at least
						// one left child, we set the left child of the successor's parent as the right child of the successor
						successorParent->left = successor->right;
					}
					else {
						// if the successor's parent is the current node, meaning that the successor is the right child of the current
						// node, aka the successor has no left child, so we set the right child of the current node as the right child
						// of the successor
						successorParent->right = successor->right;
					}

					// we put the found successor in the place of the current node we want to delete
					current->element = successor->element;
					current->frequency = successor->frequency;
					delete successor;
				}
				this->sizeOfBag--;
				return true;
			}
		}
		// if the element is not found yet, we go to the left or right child, based on the relation
		else if (this->relation(current->element, e)) {
			parent = current;
			current = current->right;
		}
		else {
			parent = current;
			current = current->left;
		}
	}
	// if the element is not found
	return false;
}
// WC = Theta(n), when the BST is degenerate and the element is not in the tree or the last element is removed
// BC = Theta(1), when the element is the root and has a frequency greater than 1
// TC = O(n), since the BST is not always degenerate

bool SortedBag::search(TComp elem) const {
	if (this->root == nullptr)
		return false;

	BSTNode* current = this->root;
	while (current) {
		if (current->element == elem)
			return true;
		else if (this->relation(current->element, elem))
			current = current->right;
		else
			current = current->left;
	}
	return false;
}
// WC = Theta(n), when the BST is degenerate
// BC = Theta(1), when the element is the root
// TC = O(n), since the BST is not always degenerate

int SortedBag::nrOccurrences(TComp elem) const {
	if (this->root == nullptr)
		return false;

	BSTNode* current = this->root;
	while (current) {
		if (current->element == elem)
			return current->frequency;
		else if (this->relation(current->element, elem))
			current = current->right;
		else
			current = current->left;
	}
	return 0;
}
// WC = Theta(n), when the BST is degenerate
// BC = Theta(1), when the element is the root
// TC = O(n), since the BST is not always degenerate

int SortedBag::size() const {
	return this->sizeOfBag;
}
// WC = BC = TC = Theta(1), since we only return the size of the bag

bool SortedBag::isEmpty() const {
	return this->sizeOfBag == 0;
}
// WC = BC = TC = Theta(1), since we only check if the size of the bag is 0

SortedBagIterator SortedBag::iterator() const {
	return SortedBagIterator(*this);
}
// WC = BC = TC = Theta(n), the complexity of the iterator operations added up

void SortedBag::deleteNode(BSTNode* node) {
	if (node == nullptr)
		return;
	deleteNode(node->left);
	deleteNode(node->right);
	delete node;
}
// WC = BC = TC = Theta(1), since we only delete the node, operation considered to be done in constant time

SortedBag::~SortedBag() {
	deleteNode(this->root);
}
// WC = BC = TC = Theta(n), since we call deleteNode for each of the n nodes in the tree
