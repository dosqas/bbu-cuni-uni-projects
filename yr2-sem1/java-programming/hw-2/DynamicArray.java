package cz.cuni.mff.sopteles.util;

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
        for (int ind = 0; ind < auxArr.length; ind++) {
            this.array[ind] = auxArr[ind];
        }
    }
}
