package cz.cuni.mff.sopteles.util;

import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveAction;

public class Arrays {
    public static void paraMergeSort(int[] arr) {
        if (arr == null || arr.length < 2)
            return;

        try (ForkJoinPool pool = new ForkJoinPool()) {
            pool.invoke(new MergeSortTask(arr, 0, arr.length - 1));
            pool.shutdown();
        }
    }

    private static class MergeSortTask extends RecursiveAction {
        private final int[] _array;
        private final int _left;
        private final int _right;

        public MergeSortTask(int[] array, int left, int right) {
            _array = array;
            _left = left;
            _right = right;
        }

        @Override
        protected void compute() {
            if (_right - _left != 0) {
                int mid = (_left + _right) / 2;

                MergeSortTask leftTask = new MergeSortTask(_array, _left, mid);
                MergeSortTask rightTask = new MergeSortTask(_array, mid + 1, _right);

                invokeAll(leftTask, rightTask);

                merge(_array, _left, mid, _right);
            }
        }

        private void merge(int[] array, int left, int mid, int right) {
            int[] temp = new int[right - left + 1];
            int i = left, j = mid + 1, k = 0;

            while (i <= mid && j <= right) {
                if (array[i] <= array[j]) {
                    temp[k++] = array[i++];
                } else {
                    temp[k++] = array[j++];
                }
            }

            while (i <= mid) {
                temp[k++] = array[i++];
            }

            while (j <= right) {
                temp[k++] = array[j++];
            }

            System.arraycopy(temp, 0, array, left, temp.length);
        }
    }
}
