#
# This is the program's UI module. The user interface and all interaction with the user (print and input statements)
# are found here
#
from functions import choice_one
from functions import choice_two
from functions import choice_four
from functions import check_valid
from functions import check_valid2
from functions import check_valid3
from functions import check_valid4
from functions import check_list
from functions import add_to_month
from functions import add_to_month_list
from functions import test_for_one
from functions import test_for_two
from functions import random_generation
from functions import undo
from functions import testing


def choice_three(string, month):
    words = string.split(" ")
    if words[0] == "list":
        if len(words) == 1:
            choice_three1(month)
        elif len(words) == 2:
            if check_list(words[1]):
                choice_three2(month, words[1])
        elif len(words) == 4:
            if check_list(words[1]):
                if words[2] == '>' or words[2] == '=' or words[2] == '<':
                    if words[3].isdigit():
                        choice_three3(month, words[1], words[2], int(words[3]))


def choice_three1(month):
    print("\nThe list is:")
    test_month = month
    test_month.sort()
    for cnt in range(len(month)):
        print(test_month[cnt], end=" ")
    print('\n')


def choice_three2(month, category):
    print("\nThe list is:")
    test_month = month
    test_month.sort()
    for cnt in range(len(month)):
        if test_month[cnt][2] == category:
            print(test_month[cnt], end=" ")
    print('\n')


def choice_three3(month, category, symbol, value):
    print("\nThe list is:")
    test_month = month
    test_month.sort()
    for cnt in range(len(month)):
        if test_month[cnt][2] == category:
            if symbol == ">":
                if int(test_month[cnt][1]) > value:
                    print(test_month[cnt], end=" ")
            elif symbol == "=":
                if int(test_month[cnt][1]) == value:
                    print(test_month[cnt], end=" ")
            elif symbol == "<":
                if int(test_month[cnt][1]) < value:
                    print(test_month[cnt], end=" ")
    print('\n')


def error_one():
    # This function raises a value error for an invalid input for point 1.
    raise ValueError("Invalid input.")


def menu():
    testing()
    month = random_generation()
    list_of_months = []
    add_to_month_list(list_of_months, month.copy())
    while True:
        print("This is the menu for the family expenses (ex. 3). The options provided are:\n"
              "1) Add new expense.\n"
              "2) Modify expenses.\n"
              "3) Display expenses with different properties.\n"
              "4) Filter expenses.\n"
              "5) Undo.\n"
              "6) Exit.")

        while True:
            try:
                choice = int(input("Enter the number for the operation you wish to do: "))
                if 0 < choice < 7:
                    break
            except ValueError:
                print("Oops!  That was not a valid input. Try again...\n")
            else:
                print("Oops!  That was not a valid operation. Try again...\n")

        if choice == 1:
            try:
                print("\nThe syntax is either:\nadd <sum> <category> \ninsert <day> <sum> <category>\n")
                print("Categories: housekeeping, food, transport, clothing, internet, others.\n")
                string = input("Enter input: ")
                if check_valid(string):
                    operation = choice_one(string)
                    add_to_month(month, operation)
                    add_to_month_list(list_of_months, month.copy())
                    test_for_one(operation, string)
                    print("Operation successful.\n")
                else:
                    error_one()
            except ValueError as ve:
                print("Error occurred: ", ve)

        elif choice == 2:
            try:
                print("\nThe syntax is either:\nremove <day>\nremove <start day> to <end day>\nremove <category>\n")
                print("Categories: housekeeping, food, transport, clothing, internet, others.\n")
                string = input("Enter input: ")
                if check_valid2(string):
                    month = choice_two(string, month)
                    test_for_two(month, string)
                    add_to_month_list(list_of_months, month.copy())
                    print("Operation successful.\n")
                else:
                    error_one()
            except ValueError as ve:
                print("Error occurred: ", ve)
        elif choice == 3:
            try:
                print("\nThe syntax is either:\nlist\nlist <category>\nlist <category> [ < | = | > ] <value>\n")
                print("Categories: housekeeping, food, transport, clothing, internet, others.\n")
                string = input("Enter input: ")
                if check_valid3(string):
                    choice_three(string, month)
                    print("Operation successful.\n")
                else:
                    error_one()
            except ValueError as ve:
                print("Error occurred: ", ve)
        elif choice == 4:
            try:
                print("\nThe syntax is either:\nfilter <category>\nfilter <category> [ < | = | > ] <value>\n")
                print("Categories: housekeeping, food, transport, clothing, internet, others.\n")
                string = input("Enter input: ")
                if check_valid4(string):
                    month = choice_four(string, month)
                    add_to_month_list(list_of_months, month.copy())
                    print("Operation successful.\n")
                else:
                    error_one()
            except ValueError as ve:
                print("Error occurred: ", ve)
        elif choice == 5:
            print("\nLast operation (if there was one) has been undone.\n")
            list_of_months, month = undo(list_of_months)
        elif choice == 6:
            print("Goodbye.")
            quit()
