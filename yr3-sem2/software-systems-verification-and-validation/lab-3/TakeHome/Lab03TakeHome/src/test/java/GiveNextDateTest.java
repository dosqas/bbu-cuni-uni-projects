import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.assertEquals;

public class GiveNextDateTest {

    @Test
    public void testGlobalInvalidDate() {
        GiveNextDate date = new GiveNextDate(0, 5, 2000);
        assertEquals("invalid Input Date", date.run());
    }

    @Test
    public void test31DayMonthStandard() {
        GiveNextDate date = new GiveNextDate(1, 15, 2000);
        assertEquals("1/16/2000", date.run());
    }

    @Test
    public void test31DayMonthRollover() {
        GiveNextDate date = new GiveNextDate(1, 31, 2000);
        assertEquals("2/1/2000", date.run());
    }

    @Test
    public void test30DayMonthStandard() {
        GiveNextDate date = new GiveNextDate(4, 15, 2000);
        assertEquals("4/16/2000", date.run());
    }

    @Test
    public void test30DayMonthRollover() {
        GiveNextDate date = new GiveNextDate(4, 30, 2000);
        assertEquals("5/1/2000", date.run());
    }

    @Test
    public void test30DayMonthInvalidDay() {
        GiveNextDate date = new GiveNextDate(4, 31, 2000);
        assertEquals("Invalid Input Date", date.run());
    }

    @Test
    public void testDecemberStandard() {
        GiveNextDate date = new GiveNextDate(12, 15, 2000);
        assertEquals("12/16/2000", date.run());
    }

    @Test
    public void testDecemberRolloverBug() {
        GiveNextDate date = new GiveNextDate(12, 31, 2000);
        assertEquals("1/1/2001", date.run(), "December 31st should roll over to January 1st of the next year");
    }

    @Test
    public void testDecemberInvalidNextYear() {
        GiveNextDate date = new GiveNextDate(12, 32, 2021);
        assertEquals("Invalid Input Date", date.run(), "Day 32 does not exist, so it should be an invalid input date");
    }

    @Test
    public void testFebruaryStandard() {
        GiveNextDate date = new GiveNextDate(2, 15, 2000);
        assertEquals("2/16/2000", date.run());
    }

    @Test
    public void testFebruary28LeapYear() {
        GiveNextDate date = new GiveNextDate(2, 28, 2000);
        assertEquals("2/29/2000", date.run());
    }

    @Test
    public void testFebruary28NonLeapYear() {
        GiveNextDate date = new GiveNextDate(2, 28, 2001);
        assertEquals("3/1/2001", date.run());
    }

    @Test
    public void testFebruary29LeapYear() {
        GiveNextDate date = new GiveNextDate(2, 29, 2000);
        assertEquals("3/1/2000", date.run());
    }

    @Test
    public void testFebruary29NonLeapYear() {
        GiveNextDate date = new GiveNextDate(2, 29, 2001);
        assertEquals("Invalid Input Date", date.run());
    }

    @Test
    public void testFebruaryInvalidDay() {
        GiveNextDate date = new GiveNextDate(2, 30, 2000);
        assertEquals("Invalid Input Date", date.run());
    }
}