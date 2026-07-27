import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

public class SetTest {

    // TC 1.0: Set(5), IsInTheSet(10) -> Expected: false
    @Test
    public void testIsInTheSet_EmptySet() {
        Set set = new Set(5);
        assertFalse(set.IsInTheSet(10), "TC 1.0: 10 should not be in an empty set.");
    }

    // TC 2.0: Set(5) with {10}, IsInTheSet(10) -> Expected: true
    @Test
    public void testIsInTheSet_FirstElementMatch() {
        Set set = new Set(5);
        set.AddAValue(10);
        assertTrue(set.IsInTheSet(10), "TC 2.0: 10 should be found in the set at the first position.");
    }

    // TC 3.0: Set(5) with {5}, IsInTheSet(10) -> Expected: false
    @Test
    public void testIsInTheSet_OneElementNoMatch() {
        Set set = new Set(5);
        set.AddAValue(5);
        assertFalse(set.IsInTheSet(10), "TC 3.0: 10 should not be found in a set containing only 5.");
    }

    // TC 4.0: Set(5) with {5, 10}, IsInTheSet(10) -> Expected: true
    @Test
    public void testIsInTheSet_SecondElementMatch() {
        Set set = new Set(5);
        set.AddAValue(5);
        set.AddAValue(10);
        assertTrue(set.IsInTheSet(10), "TC 4.0: 10 should be found in the set at the second position.");
    }

    // TC 5.0: Set(5) with {1, 2, 10}, IsInTheSet(10) -> Expected: true
    @Test
    public void testIsInTheSet_MiddleElementMatch() {
        Set set = new Set(5);
        set.AddAValue(1);
        set.AddAValue(2);
        set.AddAValue(10);
        assertTrue(set.IsInTheSet(10), "TC 5.0: 10 should be found in the middle of the set.");
    }

    // TC 6.0: Set(5) with {1, 2, 3, 4, 5}, IsInTheSet(10) -> Expected: false
    @Test
    public void testIsInTheSet_FullSetNoMatch() {
        Set set = new Set(5);
        set.AddAValue(1);
        set.AddAValue(2);
        set.AddAValue(3);
        set.AddAValue(4);
        set.AddAValue(5);
        assertFalse(set.IsInTheSet(10), "TC 6.0: 10 should not be found in a full set of different numbers.");
    }

    // TC 7.0: Set(5), AddAValue(10) -> Expected: true
    @Test
    public void testAddAValue_ToEmptySet() {
        Set set = new Set(5);
        assertTrue(set.AddAValue(10), "TC 7.0: Adding a value to an empty set should return true.");
    }

    // TC 8.0: Set(5) with {10}, AddAValue(10) -> Expected: false
    @Test
    public void testAddAValue_DuplicateValue() {
        Set set = new Set(5);
        set.AddAValue(10);
        assertFalse(set.AddAValue(10), "TC 8.0: Adding a duplicate value should return false.");
    }

    // TC 9.0: Set(5) with {5}, AddAValue(10) -> Expected: true
    @Test
    public void testAddAValue_UniqueValueToNonEmptySet() {
        Set set = new Set(5);
        set.AddAValue(5);
        assertTrue(set.AddAValue(10), "TC 9.0: Adding a unique value should return true.");
    }

    // TC 10.0: Set(5) with {1, 2, 3, 4, 5}, AddAValue(10) -> Expected: Exception
    @Test
    public void testAddAValue_BeyondCapacityThrowsException() {
        Set set = new Set(5);
        set.AddAValue(1);
        set.AddAValue(2);
        set.AddAValue(3);
        set.AddAValue(4);
        set.AddAValue(5);

        // The array capacity is 5, adding a 6th element should throw an IndexOutOfBoundsException
        assertThrows(ArrayIndexOutOfBoundsException.class, () -> {
            set.AddAValue(10);
        }, "TC 10.0: Adding beyond capacity should throw ArrayIndexOutOfBoundsException.");
    }
}