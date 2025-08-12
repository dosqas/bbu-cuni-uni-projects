# Soptelea Sebastian 917

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm
import matplotlib.colors


def f(x_value, y_value, b_value):
    return 0.5 * (x_value ** 2 + b_value * y_value ** 2)


def grad_f(x_value, y_value, b_value):
    return np.array([x_value, b_value*y_value])


def gradient_descent(x_start, y_start, learning_rate=0.1, n_iter=10):
    x_value = x_start
    y_value = y_start
    hist = np.zeros((n_iter + 1, 2))
    hist[0] = [x_value, y_value]

    for cnt in range(n_iter):
        grad = grad_f(x_value, y_value, b)
        x_value, y_value = np.array([x_value, y_value]) - learning_rate * grad
        hist[cnt + 1] = [x_value, y_value]

    return hist


x = np.linspace(-1.5, 1.5, 100)
y = np.linspace(-1.5, 1.5, 100)
X, Y = np.meshgrid(x, y)

b_values = [1, 1/2, 1/5, 1/10]
fig, axs = plt.subplots(1, len(b_values), figsize=(5*len(b_values), 5))

for i, b in enumerate(b_values):
    Z = f(X, Y, b)
    axs[i].contour(X, Y, Z, levels=np.logspace(0, 5, 35), norm=LogNorm(), cmap='viridis')
    history = gradient_descent(1.0, 1.0, b)
    axs[i].plot(history[:, 0], history[:, 1], 'w+')
    axs[i].set_title(f'b = {b}')

plt.show()
