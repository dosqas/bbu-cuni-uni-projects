package cz.cuni.mff.sopteles.util;

import java.lang.Comparable;

@SuppressWarnings("ALL")
public class DynamicArray implements SimpleCollection {
    private Object[] array;
    private int capacity;
    private int size;

    public DynamicArray() {
        this.array = new Object[10];
        this.capacity = 10;
        this.size = 0;
    }

    public void add(Object o) {
        if (this.size == this.capacity)
            this.resize();

        this.size++;
        this.array[this.size - 1] = o;
    }

    public Object get(int i) {
        if (i >= this.size)
            return null;
        return array[i];
    }

    public void remove(Object o) {
        for (int ind = 0; ind < this.size; ind++) {
            if (array[ind] == o) {
                this.array[ind] = this.array[this.size - 1];
                this.array[this.size - 1] = null;
            }
        }
    }

    public void remove(int i) {
        for (int ind = 0; ind < this.size; ind++) {
            if (ind == i) {
                this.array[ind] = this.array[this.size - 1];
                this.array[this.size - 1] = null;
            }
        }
    }

    private void resize() {
        this.capacity *= 2;
        Object[] auxArr = this.array;
        this.array = new Object[this.capacity];
        System.arraycopy(auxArr, 0, this.array, 0, auxArr.length);
    }

    public static DynamicArray of(Object... args) {
        DynamicArray newDA = new DynamicArray();
        for (Object obj : args) {
            newDA.add(obj);
        }

        return newDA;
    }

    private int partition(int low, int high) {
        Comparable<Object> pivot = (Comparable<Object>) this.get(high);
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (((Comparable<Object>) this.array[j]).compareTo(pivot) <= 0) {
                i++;
                Object aux = this.array[j];
                this.array[j] = this.array[i];
                this.array[i] = aux;
            }
        }

        Object aux = this.array[i + 1];
        this.array[i + 1] = this.array[high];
        this.array[high] = aux;

        return i + 1;
    }

    private void quicksort(int low, int high) {
        if (low < high) {
            int pivot_index = partition(low, high);
            quicksort(low, pivot_index - 1);
            quicksort(pivot_index + 1, high);
        }
    }

    public void sort() {
        quicksort(0, this.size - 1);
    }
}
