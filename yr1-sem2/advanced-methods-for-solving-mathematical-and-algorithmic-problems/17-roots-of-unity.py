import numpy as np
import matplotlib.pyplot as plt

# definesc cele 17 rădăcini ale unității
roots_of_unity = [np.exp(2j * np.pi * n / 17) for n in range(17)]

x = [z.real for z in roots_of_unity]
y = [z.imag for z in roots_of_unity]

x.append(roots_of_unity[0].real)
y.append(roots_of_unity[0].imag)

plt.figure(figsize=(6, 6))
plt.plot(x, y, 'o-')
plt.gca().set_aspect('equal', adjustable='box')
plt.title('heptadecagon')
plt.grid(True)
plt.show()