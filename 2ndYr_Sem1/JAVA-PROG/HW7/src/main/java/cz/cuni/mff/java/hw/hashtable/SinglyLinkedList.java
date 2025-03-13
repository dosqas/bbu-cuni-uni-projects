package cz.cuni.mff.java.hw.hashtable;

import java.util.Iterator;

/**
 * Singly linked list implementation. Used for chaining in the hash table.
 */
public class SinglyLinkedList implements ISinglyLinkedList{
    private Node head;

    /**
     * Constructor.
     */
    public SinglyLinkedList() {
        head = null;
    }

    /**
     * Get the head of the list.
     *
     * @return The head of the list.
     */
    public Node getHead() {
        return head;
    }

    /**
     * Add a new pair to the list.
     *
     * @param value The pair to add.
     */
    public void add(Pair value) {
        if (head == null) {
            head = new Node(value);
            return;
        }

        Node current = head;
        while (current.next != null) {
            current = current.next;
        }
        current.next = new Node(value);
    }

    /**
     * Check if the list contains a pair with the given key.
     *
     * @param key The key to search for.
     * @return True if the list contains a pair with the given key, false otherwise.
     */
    public Boolean contains(String key) {
        Node current = head;
        while (current != null) {
            if (current.value.key.equals(key)) {
                return true;
            }
            current = current.next;
        }
        return false;
    }

    /**
     * Find a pair with the given key.
     *
     * @param key The key to search for.
     * @return The pair with the given key, or null if no such pair exists.
     */
    public Pair find(String key) {
        Node current = head;
        while (current != null) {
            if (current.value.key.equals(key)) {
                return current.value;
            }
            current = current.next;
        }
        return null;
    }

    /**
     * Remove a pair with the given key.
     *
     * @param key The key of the pair to remove.
     */
    public void remove(String key) {
        if (head == null) return;

        if (head.value.key.equals(key)) {
            head = head.next;
            return;
        }

        Node current = head;
        while (current.next != null) {
            if (current.next.value.key.equals(key)) {
                current.next = current.next.next;
                return;
            }
            current = current.next;
        }
    }

    /**
     * Get the number of pairs in the list.
     *
     * @return The number of pairs in the list.
     */
    public int size() {
        int count = 0;
        Node current = head;
        while (current != null) {
            count++;
            current = current.next;
        }
        return count;
    }

    /**
     * Clear the list.
     */
    public void clear() {
        head = null;
    }

    /**
     * Get an iterator over the pairs in the list.
     *
     * @return An iterator over the pairs in the list.
     */
    @Override
    public Iterator<Node> iterator() {
        return new Iterator<>() {
            private Node current = head;

            /**
             * Check if there is another pair in the list.
             *
             * @return True if there is another pair in the list, false otherwise.
             */
            @Override
            public boolean hasNext() {
                return current != null;
            }

            /**
             * Get the next pair in the list.
             *
             * @return The next pair in the list.
             */
            @Override
            public Node next() {
                if (!hasNext()) {
                    return null;
                }
                Node result = current;
                current = current.next;
                return result;
            }
        };
    }
}
