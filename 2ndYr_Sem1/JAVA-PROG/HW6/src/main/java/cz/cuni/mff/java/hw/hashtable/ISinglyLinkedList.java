package cz.cuni.mff.java.hw.hashtable;

/**
 * Interface for singly linked list.
 */
public interface ISinglyLinkedList extends Iterable<Node> {
    /**
     * Get the head of the list.
     *
     * @return The head of the list.
     */
    Node getHead();

    /**
     * Add a new pair to the list.
     *
     * @param value The pair to add.
     */
    void add(Pair value);

    /**
     * Remove a pair with the given key from the list.
     *
     * @param key The key of the pair to remove.
     */
    void remove(String key);

    /**
     * Check if the list contains a pair with the given key.
     *
     * @param key The key to search for.
     * @return True if the list contains a pair with the given key, false otherwise.
     */
    Boolean contains(String key);

    /**
     * Find a pair with the given key in the list.
     *
     * @param key The key to search for.
     * @return The pair with the given key, or null if no such pair exists.
     */
    Pair find(String key);

    /**
     * Get the size of the list.
     *
     * @return The size of the list.
     */
    int size();
}
