package datecalc;

public class LibraryReturnSystem {
    private final DaysCalculator daysCalculator;
    private static final int MAX_BORROW_DAYS = 14;
    private static final double FINE_PER_DAY = 2.50; // $2.50 per overdue day

    public LibraryReturnSystem(DaysCalculator daysCalculator) {
        this.daysCalculator = daysCalculator;
    }

    public double calculateOverdueFine(int borrowD, int borrowM, int borrowY, int returnD, int returnM, int returnY) {
        int daysBorrowed = daysCalculator.daysBetween2Dates(borrowD, borrowM, borrowY, returnD, returnM, returnY);

        if (daysBorrowed > MAX_BORROW_DAYS) {
            int overdueDays = daysBorrowed - MAX_BORROW_DAYS;
            return overdueDays * FINE_PER_DAY;
        }
        return 0.0; // No fine if returned on time
    }
}