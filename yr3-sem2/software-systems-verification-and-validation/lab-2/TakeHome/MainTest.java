package com.dosqas;

import org.junit.jupiter.api.*;
import static org.junit.jupiter.api.Assertions.*;
import java.util.*;

class MainTest {

    private List<Main.Employee> employees;
    private List<Main.Sale> sales;
    private final String OUT_FILE = "OUT.TXT";

    @BeforeEach
    void setUp() {
        employees = new ArrayList<>();
        sales = new ArrayList<>();
    }

    // --- ERROR HANDLING (RETURN CODES 1 & 2) ---

    @Test
    @DisplayName("TC01: EC1 - Return 1 if numberEmployees is 0")
    void testNoEmployees() {
        sales.add(new Main.Sale("IT", 1000));
        int result = Main.giveBonus(new ArrayList<>(), sales, OUT_FILE);
        assertEquals(1, result);
    }

    @Test
    @DisplayName("TC02: EC2 - Return 1 if numberSales is 0")
    void testNoSales() {
        employees.add(new Main.Employee("Alice", "IT", "Dev", 3000));
        int result = Main.giveBonus(employees, new ArrayList<>(), OUT_FILE);
        assertEquals(1, result);
    }

    @Test
    @DisplayName("TC03: EC3 - Return 2 if winning dept has no employees")
    void testNoEmployeesInWinningDept() {
        sales.add(new Main.Sale("IT", 10000)); // IT is winner
        employees.add(new Main.Employee("Bob", "HR", "Dev", 3000)); // Bob is HR
        int result = Main.giveBonus(employees, sales, OUT_FILE);
        assertEquals(2, result);
    }

    // --- BOUNDARY ANALYSIS (SALARY = 5000) ---

    @Test
    @DisplayName("TC04: BA - Salary exactly 5000 (Boundary) gets 1000 bonus")
    void testSalaryExactly5000() {
        sales.add(new Main.Sale("IT", 5000));
        employees.add(new Main.Employee("John", "IT", "Dev", 5000));
        Main.giveBonus(employees, sales, OUT_FILE);
        assertEquals(6000, employees.get(0).salary);
    }

    @Test
    @DisplayName("TC05: BA - Salary 5001 (Boundary) gets 500 bonus")
    void testSalaryJustOver5000() {
        sales.add(new Main.Sale("IT", 5000));
        employees.add(new Main.Employee("Jane", "IT", "Dev", 5001));
        Main.giveBonus(employees, sales, OUT_FILE);
        assertEquals(5501, employees.get(0).salary);
    }

    @Test
    @DisplayName("TC06: BA - Salary 4999 (Boundary) gets 1000 bonus")
    void testSalaryJustUnder5000() {
        sales.add(new Main.Sale("IT", 5000));
        employees.add(new Main.Employee("Jack", "IT", "Dev", 4999));
        Main.giveBonus(employees, sales, OUT_FILE);
        assertEquals(5999, employees.get(0).salary);
    }

    // --- FUNCTION/ROLE LOGIC ---

    @Test
    @DisplayName("TC07: EC5 - Manager gets 500 bonus even with low salary")
    void testManagerLowSalary() {
        sales.add(new Main.Sale("IT", 5000));
        employees.add(new Main.Employee("Boss", "IT", "Manager", 2000));
        Main.giveBonus(employees, sales, OUT_FILE);
        assertEquals(2500, employees.get(0).salary);
    }

    @Test
    @DisplayName("TC08: EC5 - Manager (Case Insensitive) gets 500 bonus")
    void testManagerCaseInsensitive() {
        sales.add(new Main.Sale("IT", 5000));
        employees.add(new Main.Employee("Boss", "IT", "mAnAgEr", 2000));
        Main.giveBonus(employees, sales, OUT_FILE);
        assertEquals(2500, employees.get(0).salary);
    }

    // --- COMPLEX LOGIC / ROBUSTNESS ---

    @Test
    @DisplayName("TC09: Multiple Sales - Highest department wins")
    void testCorrectWinningDept() {
        sales.add(new Main.Sale("IT", 1000));
        sales.add(new Main.Sale("HR", 5000)); // HR wins
        employees.add(new Main.Employee("Alice", "IT", "Dev", 3000));
        employees.add(new Main.Employee("Bob", "HR", "Dev", 3000));

        Main.giveBonus(employees, sales, OUT_FILE);

        assertEquals(3000, employees.get(0).salary); // IT gets nothing
        assertEquals(4000, employees.get(1).salary); // HR gets 1000
    }

    @Test
    @DisplayName("TC10: Large Salary + Manager - Stays at 500 bonus")
    void testHighSalaryManager() {
        sales.add(new Main.Sale("IT", 5000));
        employees.add(new Main.Employee("Director", "IT", "Manager", 10000));
        Main.giveBonus(employees, sales, OUT_FILE);
        assertEquals(10500, employees.get(0).salary); // Not double bonus
    }

    // --- ADDITIONAL ROBUSTNESS & EDGE CASES ---

    @Test
    @DisplayName("TC11: TIE - Multiple departments with same max sale")
    void testTiedSalesDepartments() {
        // Both IT and HR have 5000. Current logic usually picks the first one found.
        sales.add(new Main.Sale("IT", 5000));
        sales.add(new Main.Sale("HR", 5000));
        employees.add(new Main.Employee("Alice", "IT", "Dev", 3000));
        employees.add(new Main.Employee("Bob", "HR", "Dev", 3000));

        Main.giveBonus(employees, sales, OUT_FILE);

        // Note: Check if your requirement says ALL winners or just THE FIRST winner.
        // If your current code only awards one, this test helps document that behavior.
        assertTrue(employees.get(0).salary > 3000 || employees.get(1).salary > 3000);
    }

    @Test
    @DisplayName("TC12: Zero Salary - Should still receive full bonus")
    void testZeroSalaryBonus() {
        sales.add(new Main.Sale("IT", 5000));
        employees.add(new Main.Employee("Intern", "IT", "Intern", 0));

        Main.giveBonus(employees, sales, OUT_FILE);
        assertEquals(1000, employees.get(0).salary);
    }

    @Test
    @DisplayName("TC13: Multiple sales for the same department")
    void testAggregatedSales() {
        // IT: 2000 + 4000 = 6000. HR: 5000. IT should win.
        sales.add(new Main.Sale("IT", 2000));
        sales.add(new Main.Sale("IT", 4000));
        sales.add(new Main.Sale("HR", 5000));

        employees.add(new Main.Employee("Alice", "IT", "Dev", 3000));

        int result = Main.giveBonus(employees, sales, OUT_FILE);
        assertEquals(0, result);
        assertEquals(4000, employees.get(0).salary);
    }

    @Test
    @DisplayName("TC14: Non-Manager with exactly 5001 salary")
    void testNonManagerBoundaryHigh() {
        sales.add(new Main.Sale("IT", 5000));
        employees.add(new Main.Employee("Expert", "IT", "Specialist", 5001));

        Main.giveBonus(employees, sales, OUT_FILE);
        assertEquals(5501, employees.get(0).salary);
    }

    @Test
    @DisplayName("TC15: Very Large Sale and Salary")
    void testLargeValues() {
        sales.add(new Main.Sale("IT", 1000000));
        employees.add(new Main.Employee("Rich", "IT", "Dev", 1000000));

        int result = Main.giveBonus(employees, sales, OUT_FILE);
        assertEquals(0, result);
        assertEquals(1000500, employees.get(0).salary);
    }
}