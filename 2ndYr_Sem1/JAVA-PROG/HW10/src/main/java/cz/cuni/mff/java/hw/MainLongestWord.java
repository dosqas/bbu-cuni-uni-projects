package cz.cuni.mff.java.hw;

import cz.cuni.mff.sopteles.util.FileUtils;

import java.nio.file.Paths;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;

public class MainLongestWord {
    public static void main(String[] args) {
        if (args.length == 0) {
            System.err.println("No file name provided.");
            return;
        }

        String fileName = args[0];
        Future<String> future = FileUtils.longestWordInFile(Paths.get(fileName));

        try {
            String longestWord = future.get();
            System.out.println(longestWord);
        }
        catch (InterruptedException | ExecutionException e) {
            System.err.println("Error processing file: " + e.getMessage());
        }
    }
}
