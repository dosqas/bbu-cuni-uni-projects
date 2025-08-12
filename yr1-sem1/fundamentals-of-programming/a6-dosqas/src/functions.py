#
# The program's functions are implemented here. There is no user interaction in this file, therefore
# no input/print statements. Functions here
# communicate via function parameters, the return statement and raising of exceptions. 
#
import random


def random_generation():
    # This function generates 10 random values at program startup
    helping_array = []
    expense_type = ["housekeeping", "food", "transport", "clothing", "internet", "others"]
    for cnt in range(0, 11):
        day = random.randint(1, 30)
        summ = random.randint(1, 100)
        for_cat = random.randint(0, 5)
        category = expense_type[for_cat]
        helping_array.append([str(day), str(summ), category])

    return helping_array


def check_list(category_picked):
    # This function checks for a certain string if it is part of the valid list of expenses
    expense_type = ["housekeeping", "food", "transport", "clothing", "internet", "others"]
    for cnt in range(6):
        if expense_type[cnt] == category_picked:
            return True


def check_valid(string):
    # This function checks the validity of the instruction written by the user for operation no. 1
    # against the valid syntax for it
    words = string.split(" ")
    if words[0] == "add":
        if words[1].isdigit() and int(words[1]) > -1 and isinstance(int(words[1]), int):
            if check_list(words[2]):
                return True
            else:
                raise ValueError("Invalid category")
        else:
            raise ValueError("Invalid amount")
    elif words[0] == "insert":
        if words[1].isdigit() and int(words[1]) > -1 and isinstance(int(words[1]), int):
            if 0 < int(words[1]) < 31:
                if words[2].isdigit():
                    if check_list(words[3]):
                        return True
                    else:
                        raise ValueError("Invalid category")
                else:
                    raise ValueError("Invalid amount")
            else:
                raise ValueError("Invalid day")
        else:
            raise ValueError("Invalid day")
    else:
        raise ValueError("Invalid instruction")


def check_valid2(string):
    # This function checks the validity of the instruction written by the user for operation no. 2
    # against the valid syntax for it
    words = string.split(" ")
    if words[0] == "remove":
        if len(words) == 2:
            if words[1].isdigit():
                if 0 < int(words[1]) < 31 and int(words[1]) > -1 and isinstance(int(words[1]), int):
                    return True
                else:
                    raise ValueError("Invalid day")
            elif check_list(words[1]):
                return True
            else:
                raise ValueError("Invalid category")
        elif len(words) == 4:
            if (words[1].isdigit() and words[3].isdigit() and int(words[1]) > -1 and isinstance(int(words[1]), int)
                    and int(words[3]) > -1 and isinstance(int(words[3]), int)):
                if 0 < int(words[1]) < 31 and 0 < int(words[3]) < 31:
                    return True
                else:
                    raise ValueError("Invalid day")
            else:
                raise ValueError("Invalid amount")
        else:
            raise ValueError("Invalid instruction")
    else:
        raise ValueError("Invalid instruction")


def check_valid3(string):
    # This function checks the validity of the instruction written by the user for operation no. 3
    # against the valid syntax for it
    words = string.split(" ")
    if words[0] == "list":
        if len(words) == 1:
            return True
        elif len(words) == 2:
            if check_list(words[1]):
                return True
            else:
                raise ValueError("Invalid category")
        elif len(words) == 4:
            if check_list(words[1]):
                if words[2] == '>' or words[2] == '=' or words[2] == '<':
                    if words[3].isdigit() and int(words[3]) > -1 and isinstance(int(words[3]), int):
                        return True
                    else:
                        raise ValueError("Invalid amount")
                else:
                    raise ValueError("Invalid operand")
            else:
                raise ValueError("Invalid category")
        else:
            raise ValueError("Invalid input")


def check_valid4(string):
    # This function checks the validity of the instruction written by the user for operation no. 4
    # against the valid syntax for it
    words = string.split(" ")
    if words[0] == "filter":
        if len(words) == 2:
            if check_list(words[1]):
                return True
            else:
                raise ValueError("Invalid category")
        elif len(words) == 4:
            if check_list(words[1]):
                if words[2] == '>' or words[2] == '=' or words[2] == '<':
                    if words[3].isdigit() and int(words[3]) > -1 and isinstance(int(words[3]), int):
                        return True
                    else:
                        raise ValueError("Invalid amount")
                else:
                    raise ValueError("Invalid operand")
            else:
                raise ValueError("Invalid category")
        else:
            raise ValueError("Invalid operation")
    else:
        raise ValueError("Invalid operation")


def choice_one(string):
    # This functions adds to the list of lists for the month, the validated operation for point 1.
    words = string.split(" ")
    if words[0] == "add":
        effect = ['1', words[1], words[2]]
    else:
        effect = [words[1], words[2], words[3]]

    return effect


def choice_two(string, month):
    # This functions calls the correct functions for whichever variant of the valid syntax for point 2.
    # the user has chosen.
    words = string.split(" ")
    if words[0] == "remove":
        if len(words) == 2:
            if words[1].isdigit():
                if 0 < int(words[1]) < 31:
                    return choice_two_1(words[1], month)
            elif check_list(words[1]):
                return choice_two_2(words[1], month)
        elif len(words) == 4:
            if words[1].isdigit() and words[3].isdigit() and words[2] == "to":
                if 0 < int(words[1]) < 31 and 0 < int(words[3]) < 31:
                    return choice_two_3(int(words[1]), int(words[3]), month)


def choice_four(string, month):
    # This functions calls the correct functions for whichever variant of the valid syntax for point 4.
    # the user has chosen.
    words = string.split(" ")
    if words[0] == "filter":
        if len(words) == 2:
            if check_list(words[1]):
                return choice_four_1(month, words[1])
        elif len(words) == 4:
            if check_list(words[1]):
                if words[2] == '>' or words[2] == '=' or words[2] == '<':
                    if words[3].isdigit():
                        return choice_four_2(month, words[1], words[2], int(words[3]))


def get_day(month, cnt):
    return month[cnt][0]


def get_price(month, cnt):
    return month[cnt][1]


def get_category(month, cnt):
    return month[cnt][2]


def test_for_one(operation, string):
    # This functions acts as a test for values given in function 1
    words = string.split(" ")
    assert int(operation[1]) >= 0
    assert check_list(operation[2])
    assert 0 < int(operation[0]) < 31
    assert isinstance(int(operation[1]), int)
    return True


def test_for_two(month, string):
    # This functions acts as a test for values given in function 2
    words = string.split(" ")
    if check_list(words[1]):
        for cnt in range(len(month)):
            assert not month[cnt][2] == words[1]
    if words[1].isdigit():
        assert int(words[1]) >= 0 and isinstance(int(words[1]), int)
        if len(words) == 4:
            assert int(words[3]) >= 0 and isinstance(int(words[3]), int)
            assert words[2] == "to"
    return True


def choice_four_1(month, category):
    # This function is the first option for syntax for point 4. - filter <category>
    test_month = []
    for cnt in range(len(month)):
        if get_category(month, cnt) == category:
            test_month.append(month[cnt])

    return test_month


def choice_four_2(month, category, symbol, value):
    # This function is the second option for syntax for point 4. - filter <category> [ < | = | > ] <value>
    test_month = []
    for cnt in range(len(month)):
        if get_category(month, cnt) == category:
            if symbol == ">":
                if int(get_price(month, cnt)) > value:
                    test_month.append(month[cnt])
            elif symbol == "=":
                if int(get_price(month, cnt)) == value:
                    test_month.append(month[cnt])
            elif symbol == "<":
                if int(get_price(month, cnt)) < value:
                    test_month.append(month[cnt])

    return test_month


def choice_two_1(day, month):
    # This function is the first option for syntax for point 2. - remove <day>
    test_month = []
    for cnt in range(len(month)):
        if not get_day(month, cnt) == day:
            test_month.append(month[cnt])

    return test_month


def choice_two_2(category, month):
    # This function is the second option for syntax for point 2. - remove <start day> to <end day>
    test_month = []
    for cnt in range(len(month)):
        if not get_category(month, cnt) == category:
            test_month.append(month[cnt])

    return test_month


def choice_two_3(day1, day2, month):
    # This function is the third option for syntax for point 2. - remove <category>
    test_month = []
    for cnt in range(len(month)):
        if not day1 < int(get_day(month, cnt)) < day2:
            test_month.append(month[cnt])

    return test_month


def undo(list_of_months):
    # This function acts as the undo.
    length = len(list_of_months)-1
    if length > -1:
        list_of_months.pop(length)

    return list_of_months, list_of_months[length-1]


def add_to_month_list(list_of_months, month):
    # This function adds a month to the list of months.
    list_of_months.append(month)


def add_to_month(month, operation):
    # This function adds an operation to the month, for point 1.
    month.append(operation)


def testing():
    testing_month = []
    assert choice_one("insert 10 100 food") == ['10', '100', 'food']
    assert choice_two('remove 5 to 20', testing_month) == []

    assert choice_one("add 10 food") == ['1', '10', 'food']
    assert choice_two('remove 1', testing_month) == []
