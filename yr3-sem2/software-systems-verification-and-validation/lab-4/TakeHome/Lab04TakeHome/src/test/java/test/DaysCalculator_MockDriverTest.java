package test;

import datecalc.DaysCalculator;
import datecalc.LeapYearChecker;
import org.junit.Before;
import org.junit.Test;
import static org.mockito.Mockito.*;
import static org.junit.Assert.assertEquals;

public class DaysCalculator_MockDriverTest {
    private DaysCalculator calculator;
    private LeapYearChecker mockLeapYearChecker;

    @Before
    public void setup() {
        // Use Mockito to create a mock object
        mockLeapYearChecker = mock(LeapYearChecker.class);
        calculator = new DaysCalculator(mockLeapYearChecker);
    }

    @Test
    public void testDaysBetweenUsingMock() {
        // Configure mock behavior
        when(mockLeapYearChecker.isLeapYear(2023)).thenReturn(false);
        when(mockLeapYearChecker.isLeapYear(2024)).thenReturn(true);
        when(mockLeapYearChecker.isLeapYear(2025)).thenReturn(false);

        // From Dec 30, 2023 to Jan 2, 2025
        int days = calculator.daysBetween2Dates(30, 12, 2023, 2, 1, 2025);

        // 1 day in 2023 + 366 days in 2024 + 2 days in 2025 = 369 days
        assertEquals(369, days);

        // Verify the mock was interacted with
        verify(mockLeapYearChecker, atLeastOnce()).isLeapYear(2023);
        verify(mockLeapYearChecker, atLeastOnce()).isLeapYear(2024);
    }
}