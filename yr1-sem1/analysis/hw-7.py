# Soptelea Sebastian

import math


def f(x):
    return math.exp(-x**2)


def trap(func, a, b, n):
    h = (b - a) / n
    s = func(a) + func(b)

    for cnt in range(1, n):
        s += 2 * func(a + cnt * h)
    return s * h / 2


# Test for increasing values of a
for A in range(1, 10):
    integral = 2 * trap(f, 0, A, 1000)  # factor of 2 because we integrate from -a to a
    print(f"For a = {A}, integral = {integral}, sqrt(pi) = {math.sqrt(math.pi)}, "
          f"difference = {abs(integral - math.sqrt(math.pi))}")
