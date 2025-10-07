# Write a Matlab function that computes the combinations C k choose n
# Use it to compute C1 choose 3 ,C7 choose 10 and C15 choose 25
# Compare your results with the ones given by the Matlab function nchoosek.

fprintf("Custom combinations function\n")
C1 = combinations(3, 1)
C2 = combinations(10, 7)
C3 = combinations(25, 15)

fprintf("nchoosek function\n")
C1alt = nchoosek(3, 1)
C2alt = nchoosek(10, 7)
C3alt = nchoosek(25, 15)
