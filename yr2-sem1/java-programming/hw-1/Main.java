package main;

public class Main {
    public static void main(String[] args) {
        int nr = Integer.parseInt(args[0]);
        int maxResult = nr * 10, nrDigits = 0;
        while (maxResult > 0) {
            maxResult /= 10;
            nrDigits++;
        }
        for (int i = 1; i <= 10; i++) {
            String result;
            int result1 = i * nr;
            if (i < 10) {
                result = " " + i + " * " + nr + " = ";
            } else {
                result = i + " * " + nr + " = ";
            }
            int resultDigits = 0, result1Copy = result1;
            while (result1 > 0) {
                result1 /= 10;
                resultDigits++;
            }
            while (resultDigits < nrDigits) {
                result += " ";
                resultDigits++;
            }
            result += result1Copy;
            System.out.println(result);
        }
    }
}

/*public class Main {
    public static void main(String[] args) {

    }
}*/
