[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/0NiWzAn2)
# :computer: Assignment 06

## Requirements
- You will be given one of the problems below to solve
- In addition to **procedural programming**, also use **modular programming** by having a **UI** module, a **Functions** module and a **Start** module
- The **UI** module provides a command-based console user interface that accepts given commands **exactly** as stated
- Handle the case of incorrect user input by displaying error messages. The program must not crash regardless of user input.
- Use the built-in `list` or `dict` compound types to represent entities in the problem domain and access/modify them using *getter* and *setter* functions
- Use Python's [exception mechanism](https://docs.python.org/3/tutorial/errors.html) so that functions can signal that an exceptional situation, or error, has happened.
- Provide **specifications** for all non-UI functions (except getters and setters), and tests for all non-UI functions related to functionalities **(A)** and **(B)**
- Have at least 10 randomly generated items in your application at program startup
- Deadline for maximum grade is **week 9**, hard deadline is **week 11**.

## Problem Statements
### 3. Family Expenses
A family wants to manage their monthly expenses. They need an application to store, for a given month, all their expenses. Each expense will be stored using the following elements: `day` (*of the month in which it was made, between 1 and 30, for simplicity*), `amount of money` (positive integer) and `expense type` (one of: `housekeeping`, `food`, `transport`, `clothing`, `internet`, `others`). Write a program that implements the functionalities exemplified below:

**(A) Add a new expense**\
`add <sum> <category>`\
`insert <day> <sum> <category>`\
e.g.\
`add 10 internet` – add to the current day an expense of `10 RON` for internet\
`insert 25 100 food` – insert to day 25 an expense of `100 RON` for food

**(B) Modify expenses**\
`remove <day>`\
`remove <start day> to <end day>`\
`remove <category>`\
e.g.\
`remove 15` – remove all expenses for day 15\
`remove 2 to 9` – remove all expenses between days 2 and 9\
`remove food` – remove all expenses for food

**(C) Display expenses with different properties**\
`list`\
`list <category>`\
`list <category> [ < | = | > ] <value>`\
e.g.\
`list` – display all expenses\
`list food` – display all the expenses for `food`\
`list food > 5` - display all `food` expenses with an amount of money `>5`\
`list internet = 44` - display all `internet` expenses with an amount of money `=44`

**(D) Filter expenses**\
`filter <category>`\
`filter <category> [ < | = | > ] <value>`\
e.g.\
`filter food` – keep only expenses in category `food`\
`filter books < 100` – keep only expenses in category books with amount of money `<100 RON`\
`filter clothing = 59` – keep only expenses for clothing with amount of money `=59 RON`

**(E) Undo**\
`undo` – the last operation that modified program data is reversed. The user can undo all operations performed since program start by repeatedly calling this function.