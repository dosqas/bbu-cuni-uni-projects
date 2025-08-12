import org.junit.jupiter.api.*;
import cz.cuni.mff.sopteles.util.Arrays;

public class MaxTests {
    private int[] array;

    @Test
    public void emptyArray() {
        array = new int[]{};

        int max = Arrays.max(array);

        Assertions.assertEquals(array.length, 0);
    }

    @Test
    public void oneElementArray() {
        array = new int[]{1};

        int max = Arrays.max(array);

        Assertions.assertEquals(max, 1);
    }

    @Test
    public void randomArray() {
        array = new int[]{5, 7, 1, 5, 3, 8, 9, 11, 1, 2, 4};

        int max = Arrays.max(array);

        Assertions.assertEquals(max, 11);
    }

    @Test
    public void descendingArray() {
        array = new int[]{5356, 4, 3, 2, -100};

        int max = Arrays.max(array);

        Assertions.assertEquals(max, 5356);
    }
}
