package cz.cuni.mff.java.hw.hashtable;

/**
 * A simple key-value pair.
 */
public class Pair {
    public String key;
    public Object value;

    /**
     * Creates a new key-value pair.
     * @param key The key.
     * @param value The value.
     */
    public Pair(String key, Object value) {
        this.key = key;
        this.value = value;
    }
}
