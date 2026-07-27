package test;
import datecalc.LeapYearChecker;

public class LeapYearCheckerStub extends LeapYearChecker {
    @Override
    public boolean isLeapYear(int year) {
        // Hardcoded responses for specific years used in tests
        if (year == 2024 || year == 2020) return true;
        if (year == 2023 || year == 2025) return false;
        return false;
    }
}