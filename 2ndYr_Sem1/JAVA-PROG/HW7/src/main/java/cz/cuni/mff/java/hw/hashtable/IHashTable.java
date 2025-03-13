package cz.cuni.mff.java.hw.hashtable;

/**
 * HashTable is a data structure that stores key-value pairs. It uses a DynamicArray to store the key-value pairs.
 */
public interface IHashTable extends Iterable<String> {
    /**
     * Returns the value associated with a key from the hash table.
     *
     * @param key the key to get
     * @return the value associated with the key
     */
    Object get(String key);

    /**
     * Sets the value associated with a key in the hash table.
     *
     * @param key the key to set
     * @param value the value to set
     */
    void set(String key, Object value);
}