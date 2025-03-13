package cz.cuni.mff.sopteles.util;

import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveTask;

public class Arrays {
    public static int max(int[] arr) {
        try (ForkJoinPool pool = new ForkJoinPool()) {
            return pool.invoke(new MaxTask(arr, 0, arr.length));
        }
        catch (Exception e) {
            return Integer.MIN_VALUE;
        }
    }

    private static class MaxTask extends RecursiveTask<Integer> {
        private final int[] arr;
        private final int start;
        private final int end;

        MaxTask(int[] arr, int start, int end) {
            this.arr = arr;
            this.start = start;
            this.end = end;
        }

        @Override
        protected Integer compute() {
            if (end - start <= 1) {
                return arr[start];
            }
            int mid = (start + end) / 2;

            MaxTask left = new MaxTask(arr, start, mid);
            MaxTask right = new MaxTask(arr, mid, end);

            invokeAll(left, right);
            return Math.max(left.join(), right.join());
        }
    }
}
