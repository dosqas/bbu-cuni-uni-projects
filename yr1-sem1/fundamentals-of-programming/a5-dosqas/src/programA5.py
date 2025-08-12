#
# Write the implementation for A5 in this file
#

# 
# Write below this comment 
# Functions to deal with complex numbers -- list representation
# -> There should be no print or input statements in this section 
# -> Each function should do one thing only
# -> Functions communicate using input parameters and their return values
#

def get_real_string(complex_num):
    complex_num.strip()
    parts = complex_num.split('+')

    return int(parts[0])


def get_imaginary_string(complex_num):
    complex_num.strip()
    parts = complex_num.split('+')
    imag_part = parts[1].rstrip('i').strip()

    return int(imag_part)


def convert_num(complex_num):
    return [get_real_string(complex_num), get_imaginary_string(complex_num)]


def get_real(complex_num):
    return complex_num[0]


def get_imaginary(complex_num):
    return complex_num[1]


def list_represent(complex_array, counter):
    string = f"{complex_array[counter][0]} + {complex_array[counter][1]}i"

    return string


def appending(intermediate_array, element):
    intermediate_array.append(element)

    return intermediate_array


def frequency(element: list) -> list:
    freq_array = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    real = abs(element[0])
    imaginary = abs(element[1])

    while real > 9:
        digit = real % 10
        freq_array[digit] = 1
        real //= 10
    freq_array[real] = 1
    while imaginary > 9:
        digit = imaginary % 10
        freq_array[digit] = 1
        imaginary //= 10
    freq_array[imaginary] = 1

    return freq_array


#
# Write below this comment
# Functions to deal with complex numbers -- dict representation
# -> There should be no print or input statements in this section 
# -> Each function should do one thing only
# -> Functions communicate using input parameters and their return values
#
# def get_real_string(complex_num):
#     complex_num.strip()
#     parts = complex_num.split('+')
#
#     return int(parts[0])
#
#
# def get_real(complex_num):
#     return complex_num["real_part"]
#
#
# def get_imaginary(complex_num):
#     return complex_num["imaginary_part"]
#
#
# def get_imaginary_string(complex_num):
#     complex_num.strip()
#     parts = complex_num.split('+')
#     imag_part = parts[1].rstrip('i').strip()
#
#     return int(imag_part)
#
#
# def convert_num(complex_num):
#     complex_dict = {
#         "real_part": get_real_string(complex_num),
#         "imaginary_part": get_imaginary_string(complex_num)
#     }
#
#     return complex_dict
#
#
# def list_represent(complex_array, counter):
#     real_part = complex_array[counter]["real_part"]
#     imaginary_part = complex_array[counter]["imaginary_part"]
#
#     string = f"{real_part} + {imaginary_part}i"
#
#     return string
#
#
# def appending(intermediate_array, element):
#     intermediate_array.append(element)
#
#     return intermediate_array
#
#
# def frequency(element):
#     freq_array = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
#     real = abs(get_real(element))
#     imaginary = abs(get_imaginary(element))
#
#     while real > 9:
#         digit = real % 10
#         freq_array[digit] = 1
#         real //= 10
#     freq_array[real] = 1
#     while imaginary > 9:
#         digit = imaginary % 10
#         freq_array[digit] = 1
#         imaginary //= 10
#     freq_array[imaginary] = 1
#
#     return freq_array


#
# Write below this comment 
# Functions that deal with subarray/subsequence properties
# -> There should be no print or input statements in this section 
# -> Each function should do one thing only
# -> Functions communicate using input parameters and their return values
#

def compare(element_one, element_two):
    freq_elem1 = frequency(element_one)
    freq_elem2 = frequency(element_two)

    for i in range(0, 10, 1):
        if freq_elem1[i] < freq_elem2[i]:
            return False

    return True


def naive(complex_array):
    mx = 0
    cnt = 0
    best_array = []
    for i in range(0, len(complex_array)):
        cnt = 1
        intermediate_array = []
        appending(intermediate_array, complex_array[i])
        for j in range(i + 1, len(complex_array)):
            if compare(complex_array[i], complex_array[j]) is True:
                cnt += 1
                appending(intermediate_array, complex_array[j])
        if cnt > mx:
            mx = cnt
            best_array = intermediate_array

    return cnt, best_array


def compare_dynamic(j, i, complex_array):
    if get_real(complex_array[i]) < get_real(complex_array[j]):
        return True
    return False


def dynamic(complex_array):
    dp = [[1, 1] for _ in range(len(complex_array))]

    for i in range(1, len(complex_array)):
        for j in range(0, i):
            if compare_dynamic(j, i, complex_array) and dp[i][0] < dp[j][1] + 1:
                dp[i][0] = dp[j][1] + 1
            if not compare_dynamic(j, i, complex_array) and dp[i][1] < dp[j][0] + 1:
                dp[i][1] = dp[j][0] + 1

    res = max(max(dp))
    longest_subsequence = []
    for i in range(len(complex_array) - 1, -1, -1):
        if max(dp[i]) == res:
            longest_subsequence.append(complex_array[i])
            res -= 1
    res = max(max(dp))

    return res, longest_subsequence[::-1]


#
# Write below this comment 
# UI section
# Write all functions that have input or print statements here
# Ideally, this section should not contain any calculations relevant to program functionalities
#
def menu():
    complex_array = []
    while True:
        print("Hello! This is the menu. Input a number corresponding to the operation you want to do.\n"
              "1. Read a list of complex numbers.\n"
              "2. Print the list of complex numbers.\n"
              "3. Select the problem you wish to be solved.\n"
              "4. Exit.\n")
        input_menu = int(input("Enter here: "))
        if input_menu == 1:
            complex_array = read()
        elif input_menu == 2:
            print("The current list of complex numbers is: ")
            print_list(complex_array)
        elif input_menu == 3:
            choose_option(complex_array)
        elif input_menu == 4:
            exit_program()


def read():
    # 4 + 3i 8 + 10i 0 + 0i 1 + 4i 10 + 6i 13 + 0i 7 + 8i 1 + 1i 6 + 2i 9 + 0i
    print("Input the desired amount of complex numbers to enter.")
    amount = int(input("Enter here: "))

    print("Input the ", amount, " complex numbers below.", sep='')
    complex_array = []
    for counter in range(amount):
        complex_number = input(">>>")
        complex_array.append(convert_num(complex_number))

    return complex_array


def print_list(complex_array):
    for counter in range(len(complex_array) - 1):
        string = list_represent(complex_array, counter)
        print(string, sep='', end=", ")

    string = list_represent(complex_array, len(complex_array) - 1)
    print(string, "\n", sep='')


def choose_option(complex_array):
    print("Type 1 for naive implementation (problem 8) "
          "or type 2 for the dynamic programming implementation (problem 12).\n")
    input_choice = int(input("Enter here: "))
    if input_choice == 1:
        amount, best_array = naive(complex_array)
        print("The longest subarray of numbers where both their real and imaginary parts can be written using "
              "the same base 10 digits has length ", amount, " and is: ", sep='')
        print_list(best_array)
    elif input_choice == 2:
        amount_dynamic, best_array_dynamic = dynamic(complex_array)
        print("The longest alternating subsequence, when considering each number's real part, has length ",
              amount_dynamic, " and is: ", sep='')
        print_list(best_array_dynamic)


def exit_program():
    print("Goodbye.")
    quit()


if __name__ == "__main__":
    menu()
