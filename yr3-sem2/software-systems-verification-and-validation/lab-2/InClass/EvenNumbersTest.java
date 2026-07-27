package org.example;

import io.qameta.allure.Description;
import io.qameta.allure.Epic;
import io.qameta.allure.Feature;
import io.qameta.allure.Story;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

@Epic("Array Processing Tests")
@Feature("Even Numbers Counter")
public class EvenNumbersTest {

    @Test
    @Story("Boundary Value Analysis")
    @Description("Test an empty array. Expected result is 0.")
    public void testTC1_EmptyArray_BVA() {
        int[] input = new int[]{};
        assertEquals(0, EvenNumbers.countEvens(0, input), "Empty array should return 0 evens");
    }

    @Test
    @Story("Boundary Value Analysis")
    @Description("Test an array with exactly one even element. Expected result is 1.")
    public void testTC2_SingleEvenElement_BVA() {
        int[] input = new int[]{8};
        assertEquals(1, EvenNumbers.countEvens(1, input), "Array with one even element should return 1");
    }

    @Test
    @Story("Boundary Value Analysis")
    @Description("Test an array with exactly one odd element. Expected result is 0.")
    public void testTC3_SingleOddElement_BVA() {
        int[] input = new int[]{7};
        assertEquals(0, EvenNumbers.countEvens(1, input), "Array with one odd element should return 0");
    }

    @Test
    @Story("Equivalence Partitioning")
    @Description("Test an array containing only odd numbers. Expected result is 0.")
    public void testTC4_AllOdds_EP() {
        int[] input = new int[]{1, 3, 5, 7, 9};
        assertEquals(0, EvenNumbers.countEvens(5, input), "Array with all odds should return 0");
    }

    @Test
    @Story("Equivalence Partitioning")
    @Description("Test an array containing only even numbers. Expected result is the length of the array.")
    public void testTC5_AllEvens_EP() {
        int[] input = new int[]{2, 4, 6, 8};
        assertEquals(4, EvenNumbers.countEvens(4, input), "Array with all evens should return 4");
    }

    @Test
    @Story("Equivalence Partitioning")
    @Description("Test a standard mixed array of even and odd numbers.")
    public void testTC6_MixedElements_EP() {
        int[] input = new int[]{10, 15, 22, 33, 40, 50};
        assertEquals(4, EvenNumbers.countEvens(6, input), "Mixed array should count exactly 4 evens");
    }

    @Test
    @Story("Error Handling")
    @Description("Test throwing an exception when N does not match the actual array length.")
    public void testError_SizeMismatch() {
        int[] input = new int[]{10, 20, 30};
        Exception exception = assertThrows(IllegalArgumentException.class, () -> {
            EvenNumbers.countEvens(5, input);
        });
        assertEquals("Provided size N does not match the actual array length.", exception.getMessage());
    }

    @Test
    @Story("Error Handling")
    @Description("Test throwing an exception when N is negative.")
    public void testError_NegativeN() {
        int[] input = new int[]{10, 20};
        Exception exception = assertThrows(IllegalArgumentException.class, () -> {
            EvenNumbers.countEvens(-2, input);
        });
        assertEquals("Array size N cannot be negative.", exception.getMessage());
    }

    @Test
    @Story("Error Handling")
    @Description("Test throwing an exception when the array is null.")
    public void testError_NullArray() {
        Exception exception = assertThrows(IllegalArgumentException.class, () -> {
            EvenNumbers.countEvens(2, null);
        });
        assertEquals("Array cannot be null.", exception.getMessage());
    }
}