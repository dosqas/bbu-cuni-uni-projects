# Solve the problem from the third set here
# Problem 13


def prime(y):
    if y < 2:
        return 0
    for d in range(2, y // 2 + 1, 1):
        if y % d == 0:
            return 0
    return 1


print("Input a number. The program will print the n-th number from the sequence of natural numbers "
      "by replacing composed numbers with their prime divisors.")

x = input(">>>")

x = int(x)

cnt = 1
nr = 2
ok = 1

if x == 1:
    print("The number is: 1")
else:
    while ok == 1:
        for i in range(2, nr + 1, 1):
            if prime(i) and nr % i == 0:
                cnt += 1

            if cnt == x and ok == 1:
                print("The number is:", i)
                ok = 0

        nr += 1
