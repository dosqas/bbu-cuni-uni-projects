# Solve the problem from the first set here
# Problem 4

def build_m(nur):
    for i in range(len(ar)-1, -1, -1):
        nur = nur * 10 + ar[i]
        ar[i] -= 1
    return nur


print("Enter a number. The program will print the biggest number written with your number's digits!")
x = input(">>>")

x = int(x)

m = 0

ar = []

if x == 0:
    print("0")
else:
    while x > 0:
        ar.append(x % 10)
        x //= 10

    ar.sort()

    m = build_m(m)

    print("The number is: ", m)
