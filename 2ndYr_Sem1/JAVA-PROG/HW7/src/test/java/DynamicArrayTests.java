import org.junit.jupiter.api.*;
import cz.cuni.mff.java.hw.hashtable.SinglyLinkedList;
import cz.cuni.mff.java.hw.hashtable.DynamicArray;

public class DynamicArrayTests {
    private DynamicArray array;

    @BeforeEach
    public void setUpEach() {
        array = new DynamicArray();
    }

    @AfterEach
    public void tearDownEach() {
        array = null;
    }

    @Test
    public void testAddOne() {
        array.add(new SinglyLinkedList());

        Assertions.assertNotNull(array.get(0));
        Assertions.assertNull(array.get(1));
    }

    @Test
    public void testAddMultiple() {
        array.add(new SinglyLinkedList());
        array.add(new SinglyLinkedList());
        array.add(new SinglyLinkedList());

        Assertions.assertNotNull(array.get(0));
        Assertions.assertNotNull(array.get(1));
        Assertions.assertNotNull(array.get(2));
        Assertions.assertNull(array.get(3));
    }

    @Test
    public void testGet() {
        SinglyLinkedList list = new SinglyLinkedList();
        array.add(list);

        Assertions.assertEquals(list, array.get(0));
    }

    @Test
    public void testSize() {
        array.add(new SinglyLinkedList());
        array.add(new SinglyLinkedList());
        array.add(new SinglyLinkedList());

        Assertions.assertEquals(3, array.size());
    }

    @Test
    public void testCapacity() {
        Assertions.assertEquals(10, array.getCapacity());
    }

    @Test
    public void testCapacityConstructor() {
        DynamicArray array = new DynamicArray(20);
        Assertions.assertEquals(20, array.getCapacity());
    }

    @Test
    public void testIterator() {
        array.add(new SinglyLinkedList());
        array.add(new SinglyLinkedList());
        array.add(new SinglyLinkedList());

        int count = 0;
        for (int i = 0; i < array.size(); i++) {
            if (array.get(i) != null) {
                count++;
            }
        }

        Assertions.assertEquals(3, count);
    }

}
