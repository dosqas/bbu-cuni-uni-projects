package cz.cuni.mff.java.hw.hashtable;

/**
 * DynamicArray is a resizable array of SinglyLinkedLists. Used for the internal representation of the HashTable.
 */
interface IDynamicArray extends Iterable<SinglyLinkedList> {
    /**
     * Adds a SinglyLinkedList to the DynamicArray.
     *
     * @param value the SinglyLinkedList to add
     */
    void add(SinglyLinkedList value);

    /**
     * Returns the SinglyLinkedList at the specified index.
     *
     * @param index the index of the SinglyLinkedList to return
     * @return the SinglyLinkedList at the specified index
     */
    Object get(int index);

    /**
     * Returns the number of SinglyLinkedLists in the DynamicArray.
     *
     * @return the number of SinglyLinkedLists in the DynamicArray
     */
    int size();

    /**
     * Returns the capacity of the DynamicArray.
     *
     * @return the capacity of the DynamicArray
     */
    int getCapacity();
}
