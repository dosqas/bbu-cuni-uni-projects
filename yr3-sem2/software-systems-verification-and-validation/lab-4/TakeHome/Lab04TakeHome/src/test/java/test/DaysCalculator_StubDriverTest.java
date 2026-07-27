package test;

import datecalc.DaysCalculator;
import org.junit.Before;
import org.junit.Test;
import static org.junit.Assert.assertEquals;

public class DaysCalculator_StubDriverTest {
    private DaysCalculator calculator;

    @Before
    public void setup() {
        // Injecting the stub instead of the real implementation
        LeapYearCheckerStub stub = new LeapYearCheckerStub();
        calculator = new DaysCalculator(stub);
    }

    @Test
    public void testDaysBetweenUsingStub() {
        // 2024 is a leap year (handled by stub)
        // From Feb 28, 2024 to Mar 1, 2024 should be 2 days (Feb 29 exists)
        int days = calculator.daysBetween2Dates(28, 2, 2024, 1, 3, 2024);
        assertEquals(2, days);
    }
}