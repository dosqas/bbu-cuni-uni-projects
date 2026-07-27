package datecalc;

public class DaysCalculator {
    private final LeapYearChecker leapYearChecker;
    private final int[] monthDays = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    public DaysCalculator(LeapYearChecker leapYearChecker) {
        this.leapYearChecker = leapYearChecker;
    }

    public int daysBetween2Dates(int d1, int m1, int y1, int d2, int m2, int y2) {
        int days = 0;

        // If dates are in the same year
        if (y1 == y2) {
            return daysPassedInYear(d2, m2, y2) - daysPassedInYear(d1, m1, y1);
        }

        // Days left in the start year
        days += daysLeftInYear(d1, m1, y1);

        // Days in full intermediate years
        for (int y = y1 + 1; y < y2; y++) {
            days += leapYearChecker.isLeapYear(y) ? 366 : 365;
        }

        // Days passed in the end year
        days += daysPassedInYear(d2, m2, y2);

        return days;
    }

    private int daysPassedInYear(int d, int m, int y) {
        int days = d;
        for (int i = 0; i < m - 1; i++) {
            days += monthDays[i];
        }
        // Add leap day if past February in a leap year
        if (m > 2 && leapYearChecker.isLeapYear(y)) {
            days++;
        }
        return days;
    }

    private int daysLeftInYear(int d, int m, int y) {
        int totalDays = leapYearChecker.isLeapYear(y) ? 366 : 365;
        return totalDays - daysPassedInYear(d, m, y);
    }
}