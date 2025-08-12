package cz.cuni.mff.java.hw.hashtable;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import static java.lang.Math.abs;

/**
 * This class implements a hash table that uses separate chaining to handle collisions.
 */
public class HashTable implements IHashTable {
    private DynamicArray hashTable;
    private int size;
    private static final double LOAD_FACTOR = 0.75;

    /**
     * Constructs a new hash table with 10 buckets.
     */
    public HashTable() {
        hashTable = new DynamicArray();
        for (int i = 0; i < hashTable.getCapacity(); i++) {
            hashTable.add(new SinglyLinkedList());
        }
        size = 0;
    }

    /**
     * Returns the hash value of a key.
     *
     * @param key the key to hash
     * @return the hash value of the key
     */
    private int hash(String key) {
        int hash = 5381;
        for (int i = 0; i < key.length(); i++) {
            hash = ((hash << 5) + hash) + key.charAt(i);
        }
        return abs(hash) % hashTable.getCapacity();
    }

    /**
     * Rehashes the hash table when the load factor exceeds 0.75.
     */
    private void rehash() {
        DynamicArray newHashTable = new DynamicArray(hashTable.getCapacity() * 2);
        for (int i = 0; i < hashTable.getCapacity() * 2; i++) {
            newHashTable.add(new SinglyLinkedList());
        }

        DynamicArray copyHashTable = new DynamicArray(hashTable.getCapacity());
        for (int i = 0; i < hashTable.getCapacity(); i++) {
            copyHashTable.add(hashTable.get(i));
        }

        hashTable = newHashTable;
        var copyOfSize = size;

        for (int i = 0; i < copyHashTable.getCapacity(); i++) {
            if (copyHashTable.get(i) == null) {
                continue;
            }
            for (Node entry : copyHashTable.get(i)) {
                set(entry.value.key, entry.value.value);
            }
        }
        size = copyOfSize;
    }

    /**
     * Returns the value associated with a key from the hash table.
     *
     * @param key the key to get
     * @return the value associated with the key
     */
    public Object get(String key) {
        int hashValue = hash(key);
        SinglyLinkedList bucket = hashTable.get(hashValue);
        if (bucket == null) {
            return 0;
        }
        for (Node entry : bucket) {
            if (entry.value.key.equals(key)) {
                return entry.value.value;
            }
        }

        return 0;
    }

    /**
     * Sets the value associated with a key in the hash table.
     * If the load factor exceeds 0.75, the hash table is rehashed.
     * If the key already exists in the hash table, the value is updated.
     *
     * @param key the key to set
     * @param value the value to set
     */
    public void set(String key, Object value) {
        if ((double) size / hashTable.getCapacity() > LOAD_FACTOR) {
            rehash();
        }

        int hashValue = hash(key);
        SinglyLinkedList bucket = hashTable.get(hashValue);
        if (bucket == null) {
            bucket = new SinglyLinkedList();
            bucket.add(new Pair(key, value));
            size++;
            return;
        }
        for (Node entry : bucket) {
            if (entry.value.key.equals(key)) {
                bucket.remove(entry.value.key);
                size--;
                break;
            }
        }
        bucket.add(new Pair(key, value));
        size++;
    }

    /**
     * Applies a lambda function to each value in the hash table.
     *
     * @param valOp the lambda function applied to each value
     */
    public void forEachValue(IValueOperation valOp) {
        for (int i = 0; i < hashTable.getCapacity(); i++) {
            if (hashTable.get(i) == null) {
                continue;
            }
            for (Node entry : hashTable.get(i)) {
                valOp.apply(entry.value);
            }
        }
    }

    /**
     * Returns an iterator over the keys in the hash table.
     *
     * @return an iterator over the keys in the hash table
     */
    @Override
    public Iterator<String> iterator() {
        return new Iterator<>() {
            private int currentIndex = 0;
            private final List<String> keys = new ArrayList<>();
            {
                for (int j = 0; j < hashTable.getCapacity(); j++) {
                    if (hashTable.get(j) == null) {
                        continue;
                    }
                    for (Node entry : hashTable.get(j)) {
                        keys.add(entry.value.key);
                    }
                }

                    keys.sort(String::compareTo);
            }

            /**
             * Returns true if there are more keys to iterate over.
             *
             * @return true if there are more keys to iterate over
             */
            @Override
            public boolean hasNext() {
                return currentIndex < size;
            }

            /**
             * Returns the next key in the iteration.
             *
             * @return the next key in the iteration
             */
            @Override
            public String next() {
                if (!hasNext()) {
                    return null;
                }
                return keys.get(currentIndex++);
            }
        };
    }
}