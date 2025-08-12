[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/MrnA3ZI7)
# 💻 Assignment 07
## Requirements


### 3. Students
Manage a list of students. Each student has an `id` (integer, unique), a `name` (string) and a `group` (positive integer). Provide the following features:
1. Add a student. Student data is read from the console.
2. Display the list of students.
3. Filter the list so that students in a given group (read from the console) are deleted from the list.
4. Undo the last operation that modified program data. This step can be repeated. The user can undo only those operations made during the current run of the program.

## Bonus possibility (0.1p, deadline week 10)
- In addition to the file-based implementations above, implement a repository that uses either a JSON or an XML file for storage (at your choice).

## Bonus possibility (0.1p, deadline week 10)
- Use a `settings.properties` file to decide which of the repository implementations to use. At startup, the program reads this input file and instantiates the correct repository. This allows changing the program's input file format without changing its source code.

## Bonus possibility (0.1p, deadline week 10)
- Implement a database-backed (SQL or NoSQL) repository. Use the database system’s update functionalities properly (don’t rewrite the entire database at each operation).
