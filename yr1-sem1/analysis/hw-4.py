import math


def iterate(iterations, p, q):
    summ = 0
    n = 1
    for i in range(1, iterations, 1):
        term = ((-1) ** (i + 1)) / n

        if i % (p + q) < p:
            summ += term
        else:
            summ -= term
        n += 1

    print("After ", iterations, " iterations of the sum(epsilon) we have "
                                "the sum equal to: ", {summ}, sep='')


vlu = math.log(2)

print("The exact value of the sum is equal to ", {vlu}, sep='')
iterate(10000, 3, 2)
iterate(75000, 3, 2)
iterate(250000, 3, 2)
