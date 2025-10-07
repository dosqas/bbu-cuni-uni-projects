function out = combinations(n, k)

out = prod((n - k + 1):n) / prod(1:k);
