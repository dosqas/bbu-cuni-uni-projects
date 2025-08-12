# The algorithms I have to use are Permutation Sort and Shell Sort.

from random import randint
import timeit


def is_sorted(ar2):
    l = len(ar2)
    for i in range(0, l - 1, 1):
        if ar2[i] > ar2[i + 1]:
            return False

    return True


def perm(ar2):
    l = len(ar2)
    for i in range(0, l, 1):
        t = randint(0, l - 1)
        ar2[i], ar2[t] = ar2[t], ar2[i]


def sort1(ar1, step1):
    print("Initial list:", ar)
    x1 = len(ar1)
    cnt = 0
    while not is_sorted(ar1):
        perm(ar1)
        cnt += 1
        if cnt == step1:
            print("The list after", step1, "(more) steps is:", ar)
            cnt = 0


def sort1_new(ar1):
    x1 = len(ar1)
    cnt = 0
    while not is_sorted(ar1):
        perm(ar1)

    return 1


def sort2(ar1, step1):
    print("Initial list:", ar)
    gap = 1
    cnt = 0
    while gap < len(ar1) // 3:
        gap = gap * 3 + 1

    while gap > 0:
        for o in range(gap, len(ar1), 1):
            val = ar1[o]
            i = o
            while i > gap - 1 and ar1[i - gap] >= val:
                ar1[i] = ar1[i - gap]
                i = i - gap
            ar1[i] = val
            cnt += 1
            if cnt == step1:
                print("The list after", step1, "(more) steps is:", ar)
                cnt = 0
        gap = (gap - 1) // 3


def sort2_new(ar1):
    cnt = 0
    n = len(ar1)
    interval = n // 2
    while interval > 0:
        for i in range(interval, n):
            temp = ar1[i]
            j = i
            while j >= interval and ar1[j - interval] > temp:
                ar1[j] = ar1[j - interval]
                j -= interval
            ar1[j] = temp
        interval //= 2


def option(x1, ar1):
    while True:
        x1 = int(input("\nHow long should the list of length 'n' be?(min. 1)\n"
                       "Enter the desired length: "))

        if x1 < 1:
            print("Error! Length has to be at least 1 (n>0). Try again.")
        else:
            break

    ar1 = []

    for i in range(1, x1 + 1, 1):
        y = randint(0, 100)
        ar1.append(y)
    return x1, ar1


def menu(ok1, ar):
    if ok1 == 1:
        print("Hello! This is the menu.\n"
              "Choose and type a number for a specific option. The options provided are:\n"
              "1 - Generate a list of 'n' random natural numbers between 0-100. If you are unhappy"
              " with the generated numbers or the chosen 'n', just type 1 again and repeat the process! :)\n"
              "2 - Sort the generated list using Permutation Sort.\n"
              "3 - Sort the generated list using Shell Sort.\n"
              "4 - Exit the program. :(\n"
              "5 - Calculate the best-case complexity for either Permutation or Shell Sort.\n"
              "6 - Calculate the average-case complexity for either Permutation or Shell Sort.\n"
              "7 - Calculate the worst-case complexity for either Permutation or Shell Sort.\n")
    else:
        print("Perfect! Returning to the menu...\n")
        print("Hello! This is the menu.\n"
              "The generated number list is:", ar,
              "Choose and type a number for a specific option. The options provided are:\n"
              "1 - Generate a list of 'n' random natural numbers between 0-100. If you are unhappy"
              " with the generated numbers or the chosen 'n', just type 1 again and repeat the process! :)\n"
              "2 - Sort the generated list using Permutation Sort.\n"
              "3 - Sort the generated list using Shell Sort.\n"
              "4 - Exit the program. :(\n"
              "5 - Calculate the best-case complexity for either Permutation or Shell Sort.\n"
              "6 - Calculate the average-case complexity for either Permutation or Shell Sort.\n"
              "7 - Calculate the worst-case complexity for either Permutation or Shell Sort.\n")


def generating(x1):
    ar1 = []

    for i in range(0, x1, 1):
        y = randint(0, 8000)
        ar1.append(y)
    return ar1


def generatinggood(x1):
    ar1 = []

    for i in range(0, x1, 1):
        ar1.append(i)
    return ar1

def generatingbad(x1):
    ar1 = []
    even = 50000
    odd = 0

    for i in range(x1):
        if i % 2 == 0:
            ar1.append(odd)
            odd += 1
        else:
            ar1.append(even)
            even -= 1

    return ar1

def bestcase(y):
    if y == 1:
        print("We will create 5 lists of length n, 2n, 3n, 4n, 5n, where n = 2")
        for i in range(1, 6, 1):
            i3 = i
            i3 = i3 * 2
            ar1 = generatinggood(i3)
            print(f"[{i}]The time taken is ", timeit.timeit(stmt=f"sort1_new({ar1})", globals=globals(), number=1))

    else:
        print("We will create 5 lists of length n, 2n, 4n, 8n, 16n, where n = 500")
        cnt = 0
        for i in range(1, 6, 1):
            i3 = pow(2, i) * 500
            cnt += 1
            ar1 = generatinggood(i3)
            print(f"[{cnt}]The time taken is ", timeit.timeit(stmt=f"sort2_new({ar1})", globals=globals(), number=1))


def averagecase(y):
    ar1 = []
    if y == 1:
        print("We will create 5 lists of length n, 2n, 3n, 4n, 5n, where n = 2")
        for i in range(1, 6, 1):
            i3 = i
            i3 = i3 * 2
            ar1 = generating(i3)
            print(f"[{i}]The time taken is ", timeit.timeit(stmt=f"sort1_new({ar1})", globals=globals(), number=1))

    else:
        print("We will create 5 lists of length n, 2n, 4n, 8n, 16n, where n = 500")
        cnt = 0
        for i in range(1, 6, 1):
            i3 = pow(2, i) * 500
            cnt += 1
            ar1 = generating(i3)
            print(f"[{cnt}]The time taken is ", timeit.timeit(stmt=f"sort2_new({ar1})", globals=globals(), number=1))


def worstcase(y):
    if y == 1:
        print("We will create 5 lists of length n, 2n, 3n, 4n, 5n, where n = 2")
        for i in range(1, 6, 1):
            i3 = i
            i3 = i3 * 2
            ar1 = generating(i3)
            print(f"[{i}]The time taken is ", timeit.timeit(stmt=f"sort1_new({ar1})", globals=globals(), number=1))

    else:
        print("We will create 5 lists of length n, 2n, 4n, 8n, 16n, where n = 500")
        cnt = 0
        for i in range(1, 6, 1):
            i3 = pow(2, i) * 500
            cnt += 1
            ar1 = generatingbad(i3)
            print(f"[{cnt}]The time taken is ", timeit.timeit(stmt=f"sort2_new({ar1})", globals=globals(), number=1))


def input_one():
    while True:
        x = input("Enter number:")
        x = int(x)
        if x == 2 or x == 3:
            print("Uh oh! You need to generate a list first(option 1). Try again.")
        elif x < 1 or x > 8:
            print("Invalid number for an operation.")
        elif x == 4:
            print("Goodbye.")
            quit()
        elif x == 5 or x == 6 or x == 7:
            y = int(input("Type 1 for Permutation Sort and 2 for Shell Sort: "))
            if x == 5:
                bestcase(y)
            elif x == 6:
                averagecase(y)
            elif x == 7:
                worstcase(y)
        else:
            break
    return 1


ar = []
menu(1, ar)
input_one()

x = 0
x, ar = option(x, ar)
ok = 0

while True:
    if ok == 1:
        menu(1, ar)
    else:
        menu(0, ar)

    ok = 1

    while True:
        x = input("Enter number:")
        x = int(x)
        if ok == 0:
            menu(0, ar)
        if x < 1:
            print("Invalid number for an operation.")
        elif x == 4:
            print("Goodbye.")
            quit()
        elif x == 5 or x == 6 or x == 7:
            ok = 0
            y = int(input("Type 1 for Permutation Sort and 2 for Shell Sort: "))
            if x == 5:
                bestcase(y)
            elif x == 6:
                averagecase(y)
            elif x == 7:
                worstcase(y)
        else:
            break

    if x == 1:
        option(x, ar)
        ok = 0
    else:
        if x == 2 or x == 3:
            print("Perfect! Choose a step for the algorithm.")
            step = int(input("Step value:"))

            if x == 2:
                sort1(ar, step)
            else:
                sort2(ar, step)

            print("The sorted list is: ", ar)
            print("\nReturning to the menu.")
            ok = 0
