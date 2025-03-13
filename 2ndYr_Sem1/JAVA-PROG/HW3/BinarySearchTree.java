package cz.cuni.mff.sopteles.utils;

import java.util.NoSuchElementException;

public class BinarySearchTree {
    public static class Node {
        int value = 0;
        Node leftChild = null;
        Node rightChild = null;
        Node parent = null;
    }
    public Node root = null;

    public Iterator getIterator() {
        return new BSTIterator();
    }

    public void add(int value) {
        if (this.root == null) {
            this.root = new Node();
            this.root.value = value;

            return;
        }

        Node currentNode = this.root;

        while (true) {
            if (currentNode.value > value) {
                if (currentNode.leftChild == null) {
                    Node newNode = new Node();
                    newNode.value = value;
                    newNode.parent = currentNode;

                    currentNode.leftChild = newNode;

                    return;
                }
                currentNode = currentNode.leftChild;
            }
            else {
                if (currentNode.rightChild == null) {
                    Node newNode = new Node();
                    newNode.value = value;
                    newNode.parent = currentNode;

                    currentNode.rightChild = newNode;

                    return;
                }
                currentNode = currentNode.rightChild;
            }
        }
    }

    private class BSTIterator implements Iterator {
        private Node currentNode;

        public BSTIterator() {
            this.currentNode = root;
            if (this.currentNode != null) {
                while (this.currentNode.leftChild != null) {
                    this.currentNode = this.currentNode.leftChild;
                }
            }
        }

        @Override
        public boolean hasNext() {
            return currentNode != null;
        }

        @Override
        public Object getValue() {
            return this.currentNode.value;
        }

        @Override
        public void next() {
            if (!hasNext()) {
                throw new NoSuchElementException();
            }

            if (this.currentNode.rightChild != null) {
                this.currentNode = this.currentNode.rightChild;

                while (this.currentNode.leftChild != null) {
                    this.currentNode = this.currentNode.leftChild;
                }
            }
            else {
                while (true) {
                    if (this.currentNode.parent == null) {
                        this.currentNode = null;
                        break;
                    }

                    if (currentNode.parent.leftChild == currentNode) {
                        currentNode = currentNode.parent;
                        break;
                    }

                    currentNode = currentNode.parent;
                }
            }
        }
    }
}
