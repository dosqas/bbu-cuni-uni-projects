# Solve the problem from the second set here
# Problem 9


def build_p(y):
    for i in range(2, x // 2 + 1, 1):
        if x % i == 0:
            y *= i
    return y


print("Input a number. This program will determine the product 'p' of all proper factors of it.")

x = input(">>>")

x = int(x)

p = 1

p = build_p(p)

print("The number is: ", p)
