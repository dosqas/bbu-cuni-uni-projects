import cz.cuni.mff.sopteles.util.FileUtils;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.concurrent.ExecutionException;

import static org.junit.jupiter.api.Assertions.fail;

public class LongestWordTests {

    @Test
    public void testEmptyFile() {
        try {
            Path tempFile = Files.createTempFile("empty", ".txt");
            Files.write(tempFile, "".getBytes());

            String longestWord = FileUtils.longestWordInFile(tempFile).get();

            assert(longestWord.equals("Nothing found"));
        }
        catch (IOException | InterruptedException | ExecutionException e) {
            fail("Fail: " + e.getMessage());
        }
    }

    @Test
    public void testFileWithOneWord() {
        try {
            Path tempFile = Files.createTempFile("one", ".txt");
            Files.write(tempFile, "word".getBytes());

            String longestWord = FileUtils.longestWordInFile(tempFile).get();

            assert(longestWord.equals("word"));
        }
        catch (IOException | InterruptedException | ExecutionException e) {
            fail("Fail: " + e.getMessage());
        }
    }

    @Test
    public void testMultipleWordsThatAreEqual() {
        try {
            Path tempFile = Files.createTempFile("multiple", ".txt");
            Files.write(tempFile, "word1 word2 word3".getBytes());

            String longestWord = FileUtils.longestWordInFile(tempFile).get();

            assert(longestWord.equals("word1"));
        }
        catch (IOException | InterruptedException | ExecutionException e) {
            fail("Fail: " + e.getMessage());
        }
    }

    @Test
    public void testDifferentWordLengths() {
        try {
            Path tempFile = Files.createTempFile("different", ".txt");
            Files.write(tempFile, "word11 word222 word33".getBytes());

            String longestWord = FileUtils.longestWordInFile(tempFile).get();

            assert(longestWord.equals("word222"));
        }
        catch (IOException | InterruptedException | ExecutionException e) {
            fail("Fail: " + e.getMessage());
        }
    }
}
