import org.junit.jupiter.api.*;
import cz.cuni.mff.sopteles.util.Arrays;

public class ParaMergeSortTests {
    private int[] array;

    @Test
    public void emptyArray() {
        array = new int[]{};

        Arrays.paraMergeSort(array);

        Assertions.assertEquals(array.length, 0);
    }

    @Test
    public void oneElementArray() {
        array = new int[]{1};

        Arrays.paraMergeSort(array);

        Assertions.assertEquals(array[0], 1);
    }

    @Test
    public void randomArray() {
        array = new int[]{5, 7, 1, 5, 3, 8, 9, 11, 1, 2, 4};

        Arrays.paraMergeSort(array);

        Assertions.assertEquals(array[0], 1);
        Assertions.assertEquals(array[1], 1);
        Assertions.assertEquals(array[2], 2);
        Assertions.assertEquals(array[3], 3);
        Assertions.assertEquals(array[4], 4);
        Assertions.assertEquals(array[5], 5);
        Assertions.assertEquals(array[6], 5);
        Assertions.assertEquals(array[7], 7);
        Assertions.assertEquals(array[8], 8);
        Assertions.assertEquals(array[9], 9);
        Assertions.assertEquals(array[10], 11);
    }

    @Test
    public void descendingArray() {
        array = new int[]{5, 4, 3, 2, 1};

        Arrays.paraMergeSort(array);

        Assertions.assertEquals(array[0], 1);
        Assertions.assertEquals(array[1], 2);
        Assertions.assertEquals(array[2], 3);
        Assertions.assertEquals(array[3], 4);
        Assertions.assertEquals(array[4], 5);
    }
}
