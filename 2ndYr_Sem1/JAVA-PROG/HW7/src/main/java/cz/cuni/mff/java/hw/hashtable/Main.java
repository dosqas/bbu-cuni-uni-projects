package cz.cuni.mff.java.hw.hashtable;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<String> words = new ArrayList<>();
        HashTable hashTable = new HashTable();

        try (BufferedReader input = new BufferedReader(new InputStreamReader(System.in))) {
            String str;
            str = input.readLine();
            while (str != null) {
                Collections.addAll(words, str.split("\\s+"));
                words.removeIf(String::isEmpty);
                str = input.readLine();

                for (var word : words) {
                    hashTable.set(word, ((int)hashTable.get(word) + 1));
                }

                words.clear();
            }
        } catch (IOException e) {
            System.out.printf("Error reading input: %s\n", e.getMessage());
        }

        for (var word : hashTable) {
            System.out.printf("%s: %d\n", word, (int)hashTable.get(word));
        }
    }
}