package com.dosqas;

import java.io.*;
import java.util.*;

public class Main {
    static class Employee {
        String name;
        String department;
        String function;
        int salary;

        public Employee(String name, String department, String function, int salary) {
            this.name = name;
            this.department = department;
            this.function = function;
            this.salary = salary;
        }
    }

    static class Sale {
        String department;
        int sumSale;

        public Sale(String department, int sumSale) {
            this.department = department;
            this.sumSale = sumSale;
        }
    }

    public static int giveBonus(List<Employee> employees, List<Sale> sales, String outputFile) {
        if (employees.isEmpty() || sales.isEmpty()) {
            return 1;
        }

        Map<String, Integer> departmentSales = new HashMap<>();
        for (Sale sale : sales) {
            departmentSales.put(sale.department, departmentSales.getOrDefault(sale.department, 0) + sale.sumSale);
        }

        String bestDepartment = null;
        int maxSales = -1;
        for (Map.Entry<String, Integer> entry : departmentSales.entrySet()) {
            if (entry.getValue() > maxSales) {
                maxSales = entry.getValue();
                bestDepartment = entry.getKey();
            }
        }

        boolean hasEmployeesInBestDepartment = false;
        for (Employee emp : employees) {
            if (emp.department.equals(bestDepartment)) {
                hasEmployeesInBestDepartment = true;
                if (emp.salary > 5000 || emp.function.equalsIgnoreCase("manager")) {
                    emp.salary += 500;
                } else {
                    emp.salary += 1000;
                }
            }
        }

        if (!hasEmployeesInBestDepartment) {
            return 2;
        }

        try (PrintWriter writer = new PrintWriter(new FileWriter(outputFile))) {
            for (Employee emp : employees) {
                writer.println(emp.name + "," + emp.department + "," + emp.function + "," + emp.salary);
            }
        } catch (IOException e) {
            e.printStackTrace();
            return -1;
        }

        return 0;
    }

    public static void main(String[] args) {
        String inputFile = "IN.TXT";
        String outputFile = "OUT.TXT";

        List<Employee> employees = new ArrayList<>();
        List<Sale> sales = new ArrayList<>();

        try (Scanner scanner = new Scanner(new File(inputFile))) {
            if (scanner.hasNextInt()) {
                int numEmployees = scanner.nextInt();
                scanner.nextLine();
                for (int i = 0; i < numEmployees; i++) {
                    String[] parts = scanner.nextLine().split(",");
                    if (parts.length == 4) {
                        employees.add(new Employee(parts[0].trim(), parts[1].trim(), parts[2].trim(), Integer.parseInt(parts[3].trim())));
                    }
                }
            }

            if (scanner.hasNextInt()) {
                int numSales = scanner.nextInt();
                scanner.nextLine();
                for (int i = 0; i < numSales; i++) {
                    String[] parts = scanner.nextLine().split(",");
                    if (parts.length == 2) {
                        sales.add(new Sale(parts[0].trim(), Integer.parseInt(parts[1].trim())));
                    }
                }
            }
        } catch (FileNotFoundException e) {
            System.err.println("Input file not found: " + inputFile);
            System.exit(1);
        }

        int returnCode = giveBonus(employees, sales, outputFile);
        System.exit(returnCode);
    }
}
