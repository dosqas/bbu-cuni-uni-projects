import org.junit.jupiter.api.*;
import cz.cuni.mff.java.hw.hashtable.SinglyLinkedList;
import cz.cuni.mff.java.hw.hashtable.Pair;
import cz.cuni.mff.java.hw.hashtable.Node;

public class SinglyLinkedListTests {
    private SinglyLinkedList list;

    @BeforeEach
    public void setUpEach() {
        list = new SinglyLinkedList();
    }

    @AfterEach
    public void tearDownEach() {
        list.clear();
    }

    @Test
    public void testEmptySLL() {
        Assertions.assertNull(list.getHead());
    }

    @Test
    public void testAddOne() {
        list.add(new Pair("key", "value"));

        Assertions.assertNotNull(list.getHead());

        Assertions.assertEquals("key", list.getHead().value.key);
        Assertions.assertEquals("value", list.getHead().value.value);

        Assertions.assertNull(list.getHead().next);
    }

    @Test
    public void testAddMultiple() {
        list.add(new Pair("key1", "value1"));
        list.add(new Pair("key2", "value2"));
        list.add(new Pair("key3", "value3"));

        Assertions.assertNotNull(list.getHead());

        Assertions.assertEquals("key1", list.getHead().value.key);
        Assertions.assertEquals("value1", list.getHead().value.value);

        Assertions.assertEquals("key2", list.getHead().next.value.key);
        Assertions.assertEquals("value2", list.getHead().next.value.value);

        Assertions.assertEquals("key3", list.getHead().next.next.value.key);
        Assertions.assertEquals("value3", list.getHead().next.next.value.value);

        Assertions.assertNull(list.getHead().next.next.next);
    }

    @Test
    public void testContains() {
        list.add(new Pair("key1", "value1"));
        list.add(new Pair("key2", "value2"));
        list.add(new Pair("key3", "value3"));

        Assertions.assertTrue(list.contains("key1"));
        Assertions.assertTrue(list.contains("key2"));
        Assertions.assertTrue(list.contains("key3"));
        Assertions.assertFalse(list.contains("key4"));
    }

    @Test
    public void testFind() {
        Pair pair1 = new Pair("key1", "value1");
        Pair pair2 = new Pair("key2", "value2");
        Pair pair3 = new Pair("key3", "value3");
        list.add(pair1);
        list.add(pair2);
        list.add(pair3);

        Assertions.assertEquals(pair1, list.find("key1"));
        Assertions.assertEquals(pair2, list.find("key2"));
        Assertions.assertEquals(pair3, list.find("key3"));
        Assertions.assertNull(list.find("key4"));
    }

    @Test
    public void testRemoveHeadOneNode() {
        list.add(new Pair("key1", "value1"));

        list.remove("key1");

        Assertions.assertNull(list.getHead());
    }

    @Test
    public void testRemoveHeadMultipleNodes() {
        Pair pair1 = new Pair("key1", "value1");
        Pair pair2 = new Pair("key2", "value2");
        Pair pair3 = new Pair("key3", "value3");
        list.add(pair1);
        list.add(pair2);
        list.add(pair3);

        list.remove("key1");

        Assertions.assertEquals(pair2, list.getHead().value);
        Assertions.assertEquals(pair3, list.getHead().next.value);

        Assertions.assertNull(list.getHead().next.next);
    }

    @Test
    public void testRemoveInnerNode() {
        Pair pair1 = new Pair("key1", "value1");
        Pair pair2 = new Pair("key2", "value2");
        Pair pair3 = new Pair("key3", "value3");
        list.add(pair1);
        list.add(pair2);
        list.add(pair3);

        list.remove("key2");

        Assertions.assertEquals(pair1, list.getHead().value);
        Assertions.assertEquals(pair3, list.getHead().next.value);

        Assertions.assertNull(list.getHead().next.next);
    }

    @Test
    public void testRemoveTail() {
        Pair pair1 = new Pair("key1", "value1");
        Pair pair2 = new Pair("key2", "value2");
        Pair pair3 = new Pair("key3", "value3");
        list.add(pair1);
        list.add(pair2);
        list.add(pair3);

        list.remove("key3");

        Assertions.assertEquals(pair1, list.getHead().value);
        Assertions.assertEquals(pair2, list.getHead().next.value);

        Assertions.assertNull(list.getHead().next.next);
    }

    @Test
    public void testRemoveFromEmptySLL() {
        list.remove("key");
        Assertions.assertNull(list.getHead());
    }

    @Test
    public void testSize() {
        Assertions.assertEquals(0, list.size());

        list.add(new Pair("key1", "value1"));
        Assertions.assertEquals(1, list.size());

        list.add(new Pair("key2", "value2"));
        Assertions.assertEquals(2, list.size());

        list.add(new Pair("key3", "value3"));
        Assertions.assertEquals(3, list.size());

        list.remove("key2");
        Assertions.assertEquals(2, list.size());
    }

    @Test
    public void testClear() {
        list.clear();
        Assertions.assertNull(list.getHead());

        list.add(new Pair("key1", "value1"));
        list.add(new Pair("key2", "value2"));
        list.add(new Pair("key3", "value3"));

        list.clear();

        Assertions.assertNull(list.getHead());
        Assertions.assertEquals(0, list.size());
    }

    @Test
    public void testIterator() {
        Pair pair1 = new Pair("key1", "value1");
        Pair pair2 = new Pair("key2", "value2");
        Pair pair3 = new Pair("key3", "value3");
        list.add(pair1);
        list.add(pair2);
        list.add(pair3);

        int i = 0;
        for (Node p : list) {
            switch (i) {
                case 0:
                    Assertions.assertEquals(pair1, p.value);
                    break;
                case 1:
                    Assertions.assertEquals(pair2, p.value);
                    break;
                case 2:
                    Assertions.assertEquals(pair3, p.value);
                    break;
                default:
                    Assertions.fail("Unexpected pair");
            }
            i++;
        }

        Assertions.assertEquals(3, i);
    }
}
