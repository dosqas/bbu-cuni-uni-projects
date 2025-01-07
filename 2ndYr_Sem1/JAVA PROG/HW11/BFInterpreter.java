package cz.cuni.mff.sopteles.util;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Deque;
import java.util.List;

public class BFInterpreter {
    private static char[] memory;

    public static void setMemory(int size) {
        memory = new char[size];
    }

    public static String validateProgram(String fileName) {
        try (BufferedReader reader = new BufferedReader(new FileReader(fileName))) {
            StringBuilder programBuilder = new StringBuilder();
            String line;
            int lineNumber = 1;

            List<Integer> lineIndices = new ArrayList<>();
            Deque<IntPair> openBracketStack = new ArrayDeque<>();

            while ((line = reader.readLine()) != null) {
                programBuilder.append(line);
                for (int i = 0; i < line.length(); i++) {
                    lineIndices.add(lineNumber);
                }
                lineNumber++;
            }

            String program = programBuilder.toString();
            int lastLineNumber = 1;
            int currentCharacterIndex = 0;

            for (int index = 0; index < program.length(); index++) {
                char ch = program.charAt(index);
                int currentLineNumber = lineIndices.get(index);
                currentCharacterIndex++;

                if (currentLineNumber != lastLineNumber) {
                    currentCharacterIndex = 1;
                    lastLineNumber = currentLineNumber;
                }

                if (ch == '[') {
                    openBracketStack.addLast(new IntPair(currentCharacterIndex, currentLineNumber));
                } else if (ch == ']') {
                    if (openBracketStack.isEmpty()) {
                        return String.format("Unopened cycle - line %d character %d", currentLineNumber, currentCharacterIndex);
                    }
                    openBracketStack.removeLast();
                }
            }

            if (!openBracketStack.isEmpty()) {
                IntPair unmatched = openBracketStack.removeFirst();
                return String.format("Unclosed cycle - line %d character %d", unmatched.second, unmatched.first);
            }
        } catch (Exception e) {
            System.err.println("File error: " + e.getMessage());
        }

        return "";
    }

    public static void runProgram(String fileName) {
        try (BufferedReader programReader = new BufferedReader(new FileReader(fileName))) {
            StringBuilder programBuilder = new StringBuilder();
            String programLine;

            while ((programLine = programReader.readLine()) != null) {
                programBuilder.append(programLine);
            }

            String program = programBuilder.toString();
            int currentMemoryPointer = 0;
            int programLength = program.length();

            for (int index = 0; index < programLength; index++) {
                char ch = program.charAt(index);

                switch (ch) {
                    case '>' -> {
                        currentMemoryPointer++;
                        if (currentMemoryPointer >= memory.length) {
                            System.out.print("Memory overrun");
                            return;
                        }
                    }
                    case '<' -> {
                        currentMemoryPointer--;
                        if (currentMemoryPointer < 0) {
                            System.out.print("Memory underrun");
                            return;
                        }
                    }
                    case '+' -> memory[currentMemoryPointer]++;
                    case '-' -> memory[currentMemoryPointer]--;
                    case '.' -> System.out.print(memory[currentMemoryPointer]);
                    case ',' -> {
                        try {
                            char input = (char) System.in.read();
                            memory[currentMemoryPointer] = input;
                        } catch (IOException e) {
                            System.err.println("Error reading input: " + e.getMessage());
                        }
                    }
                    case '[' -> {
                        if (memory[currentMemoryPointer] == 0) {
                            int bracketCounter = 1;
                            while (bracketCounter != 0) {
                                index++;
                                if (program.charAt(index) == '[') {
                                    bracketCounter++;
                                } else if (program.charAt(index) == ']') {
                                    bracketCounter--;
                                }
                            }
                        }
                    }
                    case ']' -> {
                        if (memory[currentMemoryPointer] != 0) {
                            int bracketCounter = 1;
                            while (bracketCounter != 0) {
                                index--;
                                if (program.charAt(index) == '[') {
                                    bracketCounter--;
                                } else if (program.charAt(index) == ']') {
                                    bracketCounter++;
                                }
                            }
                        }
                    }
                }
            }
        } catch (Exception e) {
            System.err.println("File error: " + e.getMessage());
        }
    }
}