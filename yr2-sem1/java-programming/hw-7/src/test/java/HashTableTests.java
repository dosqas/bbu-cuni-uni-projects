import org.junit.jupiter.api.*;
import cz.cuni.mff.java.hw.hashtable.HashTable;

public class HashTableTests {
    private HashTable hashTable;

    @BeforeEach
    public void setUpEach() {
        hashTable = new HashTable();
    }

    @AfterEach
    public void tearDownEach() {
        hashTable = null;
    }

    @Test
    public void testGet() {
        hashTable.set("key", 1);
        Assertions.assertEquals(1, (int) hashTable.get("key"));
    }

    @Test
    public void testSet() {
        hashTable.set("key1", 1);
        Assertions.assertEquals(1, (int) hashTable.get("key1"));
        hashTable.set("key1", 2);
        Assertions.assertEquals(2, (int) hashTable.get("key1"));
    }

    @Test
    public void testForEachValue() {
        hashTable.set("key1", 1);
        hashTable.set("key2", 2);
        hashTable.set("key3", 3);

        hashTable.forEachValue((value) -> {
            int intValue = (int)value.value;
            intValue += 1;
            hashTable.set(value.key, intValue);
        });

        int sum = 0;
        for (var key : hashTable) {
            sum += (int)hashTable.get(key);
        }

        Assertions.assertEquals(9, sum);
    }

    @Test
    public void testIterator() {
        hashTable.set("key1", 1);
        hashTable.set("key2", 2);
        hashTable.set("key3", 3);

        int i = 0;
        for (var value : hashTable) {
            switch (i){
                case 0 -> Assertions.assertEquals("key1", value);
                case 1 -> Assertions.assertEquals("key2", value);
                case 2 -> Assertions.assertEquals("key3", value);
            }
            i++;
        }
    }
}
