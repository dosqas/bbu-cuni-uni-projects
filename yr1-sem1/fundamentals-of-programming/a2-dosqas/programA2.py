# The algorithms I have to use are Permutation Sort and Shell Sort.

from random import randint


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
        ar[i], ar[t] = ar[t], ar[i]


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


def sort2(ar1, step1):
    print("Initial list:", ar)
    gap = 1
    cnt = 0
    while gap < len(ar1)//3:
        gap = gap * 3 + 1

    while gap > 0:
        for o in range(gap, len(ar1), 1):
            val = ar1[o]
            i = o
            while i > gap - 1 and ar1[i-gap] >= val:
                ar1[i] = ar1[i-gap]
                i = i - gap
            ar1[i] = val
            cnt += 1
            if cnt == step1:
                print("The list after", step1, "(more) steps is:", ar)
                cnt = 0
        gap = (gap - 1)// 3



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


print("Hello! This is the menu.\n"
      "Choose and type a number for a specific option. The options provided are:\n"
      "1 - Generate a list of 'n' random natural numbers between 0-100. If you are unhappy"
      " with the generated numbers or the chosen 'n', just type 1 again and repeat the process! :)\n"
      "2 - Sort the generated list using Permutation Sort.\n"
      "3 - Sort the generated list using Shell Sort.\n"
      "4 - Exit the program. :(\n")

while True:
    x = input("Enter number:")
    x = int(x)
    if x == 2 or x == 3:
        print("Uh oh! You need to generate a list first(option 1). Try again.")
    elif x < 1 or x > 4:
        print("Invalid number for an operation.")
    elif x == 4:
        print("Goodbye.")
        quit()
    else:
        break

ar = []
x = 0
x, ar = option(x, ar)
ok = 1

while True:
    if ok == 1:
        print("Perfect! Returning to the menu...\n")
        print("Hello! This is the menu.\n"
              "The generated number list is:", ar,
              "Choose and type a number for a specific option. The options provided are:\n"
              "1 - Generate a list of 'n' random natural numbers between 0-100. If you are unhappy"
              " with the generated numbers or the chosen 'n', just type 1 again and repeat the process! :)\n"
              "2 - Sort the generated list using Permutation Sort.\n"
              "3 - Sort the generated list using Shell Sort.\n"
              "4 - Exit the program. :(\n")
    else:
        print("Hello! This is the menu.\n"
              "Choose and type a number for a specific option. The options provided are:\n"
              "1 - Generate a list of 'n' random natural numbers between 0-100. If you are unhappy"
              " with the generated numbers or the chosen 'n', just type 1 again and repeat the process! :)\n"
              "2 - Sort the generated list using Permutation Sort.\n"
              "3 - Sort the generated list using Shell Sort.\n"
              "4 - Exit the program. :(\n")
    ok = 1

    while True:
        x = input("Enter number:")
        x = int(x)
        if x < 1 or x > 4:
            print("Invalid number for an operation.")
        elif x == 4:
            print("Goodbye.")
            quit()
        else:
            break

    if x == 1:
        option(x, ar)
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

