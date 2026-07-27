package test;

import datecalc.DaysCalculator;
import datecalc.LeapYearChecker;
import datecalc.LibraryReturnSystem;
import org.junit.Before;
import org.junit.Test;
import static org.mockito.Mockito.*;
import static org.junit.Assert.assertEquals;

public class LibrarySimulation_IntegrationTest {
    private LibraryReturnSystem librarySystem;
    private LeapYearChecker mockChecker;

    @Before
    public void setup() {
        mockChecker = mock(LeapYearChecker.class);
        DaysCalculator calculator = new DaysCalculator(mockChecker);
        librarySystem = new LibraryReturnSystem(calculator);
    }

    @Test
    public void testOverdueFineCalculation_Integration() {
        // Setup mock
        when(mockChecker.isLeapYear(2024)).thenReturn(true);

        // Borrowed on Feb 20, 2024. Returned on March 10, 2024
        // Feb 20 to Feb 29 = 9 days. March 1 to March 10 = 10 days. Total = 19 days.
        // Max borrow = 14 days. Overdue = 5 days. Fine = 5 * $2.50 = $12.50.

        double fine = librarySystem.calculateOverdueFine(20, 2, 2024, 10, 3, 2024);

        assertEquals(12.50, fine, 0.001);
    }
}