package cz.cuni.mff.sopteles.util;

import java.util.Iterator;

public class MString implements Iterable<Character> {
    private StringBuilder string;

    public MString() {
        this.string = new StringBuilder();
    }

    public MString(String str) {
        this.string = new StringBuilder(str);
    }

    public void append(String str) {
        this.string.append(str);
    }

    public void append(char ch) {
        this.string.append(ch);
    }

    public void insert(int pos, String str) {
        this.string.insert(pos, str);
    }

    public void insert(int pos, char ch) {
        this.string.insert(pos, ch);
    }

    public void delete(int pos, int length) {
        this.string.delete(pos, pos + length);
    }

    @Override
    public String toString() {
        return this.string.toString();
    }

    public Iterator<Character> iterator() {
        return new Iterator<>() {
            private int currentIndex = 0;

            public boolean hasNext() {
                return currentIndex < string.length();
            }

            public Character next() {
                return string.charAt(currentIndex++);
            }
        };
    }
}
