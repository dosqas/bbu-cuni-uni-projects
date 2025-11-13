% 3. Estimate by simulations the number π = 3.1415926535...
N = 3e6;

x = rand(N,1);
y = rand(N,1);

inside = (x.^2 + y.^2 <= 1);

pi_estimate = 4 * mean(inside);

printf('Estimate by simulations of pi = %.10f\n', pi_estimate);
