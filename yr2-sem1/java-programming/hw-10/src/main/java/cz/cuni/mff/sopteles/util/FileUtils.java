package cz.cuni.mff.sopteles.util;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Comparator;
import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.Future;
import java.util.stream.Stream;

public class FileUtils {
    public static Future<String> longestWordInFile(Path path) {
        return CompletableFuture.supplyAsync(() -> {
            try {
                List<String> lines = Files.readAllLines(path);

                return lines.stream()
                        .flatMap(line -> Stream.of(line.split("\\s+")))
                        .max(Comparator.comparingInt(String::length))
                        .orElse("Nothing found");
            }
            catch (IOException e) {
                return "Nothing found";
            }
        });
    }
}
