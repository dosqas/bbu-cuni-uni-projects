def compare(num1, num2):
    num1_str = str(num1)
    num2_str = str(num2)
    return any(digit in num2_str for digit in num1_str)


def iterative():
    #  O(2^n * k^2) complexity
    array = [1, 10, 24, 56, 33, 90, 19, 86, 74, 28]
    n = len(array)

    subsets = []
    stack = [(0, [])]

    while stack:
        index, current_subset = stack.pop()

        if index == n:
            if len(current_subset) >= 2:
                increase = all(current_subset[j] < current_subset[j + 1] for j in range(len(current_subset) - 1))
                common = all(compare(current_subset[j], current_subset[j + 1]) for j in range(len(current_subset) - 1))
                if increase and common:
                    subsets.append(current_subset[:])
        else:
            stack.append((index + 1, current_subset[:]))
            current_subset.append(array[index])
            increase = all(current_subset[j] < current_subset[j + 1] for j in range(len(current_subset) - 1))
            common = all(compare(current_subset[j], current_subset[j + 1]) for j in range(len(current_subset) - 1))
            if increase and common:
                stack.append((index + 1, current_subset[:]))


    print(subsets)


def recursive():
    #  O(2^n * k^2)
    def backtrack(start, path):
        if len(path) >= 2:
            increase = all(path[i] < path[i + 1] for i in range(len(path) - 1))
            common = all(compare(path[i], path[i + 1]) for i in range(len(path) - 1))
            if increase and common:
                subsets.append(path[:])

        for i in range(start, len(array)):
            path.append(array[i])
            backtrack(i + 1, path)
            path.pop()

    array = [1, 10, 24, 56, 33, 90, 19, 86, 74, 28, 90]
    subsets = []
    backtrack(0, [])
    print(subsets)


def naive():
    # O(2^n)
    s = [2, 7, 5, 3, 8]
    k = 14
    n = len(s)
    subset = []
    for i in range(1, 1 << n):
        subset = [s[j] for j in range(n) if (i >> j) & 1]
        if sum(subset) == k:
            break

    print("A Subset that sums to", k, "is:", subset)


def dynamic():
    # O(n*k)
    s = [2, 7, 5, 3, 8]
    k = 14
    n = len(s)

    dp = [[False for _ in range(k + 1)] for _ in range(n + 1)]
    for i in range(n + 1):
        dp[i][0] = True

    for i in range(1, n + 1):
        for j in range(1, k + 1):
            if j < s[i - 1]:
                dp[i][j] = dp[i - 1][j]
            else:
                dp[i][j] = dp[i - 1][j] or dp[i - 1][j - s[i - 1]]

    subset = []
    i, j = n, k
    while i > 0 and j > 0:
        if dp[i][j] and not dp[i - 1][j]:
            subset.append(s[i - 1])
            j -= s[i - 1]
        i -= 1

    subset = subset[::-1]
    print("A subset that sums to", k, "is:", subset, "\n"
                                                     "The data structure used for the intermediate results is a "
                                                     "2D array. You can find it below.")

    for i in range(1, n + 1):
        print("DP[", i, "]", sep='', end='')
        for j in range(1, k + 1):
            print("[", j, "]=", dp[i][j], sep='', end=';')
        print()


print("Hello! Type 1 for the Backtracking problem and 2 for the Dynamic Programming problem.")
x = int(input("Enter number:"))

if x == 1:
    print("Backtracking problem 9.\n"
          "Type 1 for the iterative version or 2 for the recursive version.")
    x = int(input("Enter number:"))
    if x == 1:
        iterative()
    else:
        recursive()

else:
    print("Dynamic Programming problem 2.\n"
          "Type 1 for the naive(non-optimised) version or 2 for the dynamic programming version.")
    x = int(input("Enter number:"))
    if x == 1:
        naive()
    else:
        dynamic()
