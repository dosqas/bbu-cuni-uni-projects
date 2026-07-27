package org.example;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class EvenNumbers {

    public static int countEvens(int n, int[] valuesArray) {
        if (valuesArray == null) {
            throw new IllegalArgumentException("Array cannot be null.");
        }
        if (n < 0) {
            throw new IllegalArgumentException("Array size N cannot be negative.");
        }
        if (n != valuesArray.length) {
            throw new IllegalArgumentException("Provided size N does not match the actual array length.");
        }

        int noOfEvenElements = 0;
        for (int i = 0; i < n; i++) {
            if (valuesArray[i] % 2 == 0) {
                noOfEvenElements++;
            }
        }
        return noOfEvenElements;
    }

    public static void main(String[] args) {
        try {
            File file = new File("IN.TXT");
            Scanner scanner = new Scanner(file);

            if (scanner.hasNextInt()) {
                int noElementsArray = scanner.nextInt();
                int[] valuesArray = new int[noElementsArray];

                for (int i = 0; i < noElementsArray; i++) {
                    if (scanner.hasNextInt()) {
                        valuesArray[i] = scanner.nextInt();
                    }
                }

                int result = countEvens(noElementsArray, valuesArray);
                System.out.println("Result: " + result);
            }
            scanner.close();

        } catch (FileNotFoundException e) {
            System.out.println("Error: Could not find IN.TXT. Ensure it is in the execution directory.");
        } catch (IllegalArgumentException e) {
            System.out.println("Validation Error: " + e.getMessage());
        } catch (Exception e) {
            System.out.println("Error reading the file data.");
        }
    }
}