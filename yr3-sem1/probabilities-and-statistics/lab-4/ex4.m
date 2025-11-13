% 4. Approximate by simulations the integral from 0 to 1 of sin(cube root of x)dx
N = 3e6;

x = rand(N,1);

f = sin(cbrt(x));

I_estimate = mean(f);

printf('Approximate integral of sin(3*sqrt(x)) from 0 to 1 = %.4f\n', I_estimate);

