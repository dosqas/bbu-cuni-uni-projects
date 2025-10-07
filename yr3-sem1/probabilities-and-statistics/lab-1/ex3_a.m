# a) Plot on the same graph the functions y = x^2, x in [0, 2], y =
# sqrt(x), x in [0, 4] and the dotted line y = x, x in [0, 4]. What property of functions is illustrated?

f = @(x) x.^2
x = linspace(0, 2, 100);

g = @(x) sqrt(x)
y = linspace(0, 4, 100);

h = @(x) x
z = linspace(0, 4, 100);

plot(x, f(x), y, g(y), z, h(z))

