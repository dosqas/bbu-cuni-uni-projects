package datecalc;

public class LeapYearChecker {
    public boolean isLeapYear(int year) {
        // A year is leap if it is divisible by 4, but not by 100, unless it is also divisible by 400.
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }
}