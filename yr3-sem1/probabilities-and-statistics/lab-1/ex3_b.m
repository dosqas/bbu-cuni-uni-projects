# b) For any lambda > 0, the function f(x) = lambda*e^(−lamba*x), x > 0, is an important probability density function in probability theory. For the values lambda = 1/2, 3, 7 and x in  (0, 3], plot the graphs of the three functions, in different colors and linestyles, one on top of the other, in tiled windows.

x = linspace(0, 3, 100);

f = @(x, l) l .* exp(-l .* x);

lambdas = [0.5, 3, 7];

for i = 1:3
  subplot(3, 1, i)
  plot(x, f(x, lambdas(i)))
end
