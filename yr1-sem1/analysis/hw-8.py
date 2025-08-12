# Soptelea Sebastian
import numpy as np
import matplotlib.pyplot as plt


def p_norm(x1, y1, p1):
    return (abs(x1) ** p1 + abs(y1) ** p1)**(1 / p1)


p_values = [1.25, 1.5, 3, 8]

num_points = 100000

x = np.random.uniform(-1, 1, num_points)
y = np.random.uniform(-1, 1, num_points)

fig, axs = plt.subplots(1, len(p_values), figsize=(5*len(p_values), 5))

for i, p in enumerate(p_values):
    norms = p_norm(x, y, p)

    inside = norms <= 1

    axs[i].scatter(x[inside], y[inside], s=1)
    axs[i].set_title(f'p = {p}')
    axs[i].set_aspect('equal', 'box')

plt.show()
