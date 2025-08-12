#Soptelea Sebastian, group 917
import numpy as np
import matplotlib.pyplot as plt


def f(A, needed_x, needed_y):
    """
    Defining the function f(x, y) = 0.5 * (x, y) * A * (x, y)^T
    :param A:
    :param needed_x:
    :param needed_y:
    :return:
    """
    Z = np.zeros(needed_x.shape)
    for i in range(needed_x.shape[0]):
        for j in range(needed_x.shape[1]):
            x = np.array([needed_x[i, j], needed_y[i, j]])
            Z[i, j] = 0.5 * x.T @ A @ x
    return Z


def grad_f(A, X, Y):
    """
    Defining the gradient of the function f(x, y) = 0.5 * (x, y) * A * (x, y)^T
    :param A:
    :param X:
    :param Y:
    :return:
    """
    U = np.zeros(X.shape)
    V = np.zeros(Y.shape)
    for i in range(X.shape[0]):
        for j in range(X.shape[1]):
            x = np.array([X[i, j], Y[i, j]])
            grad = A @ x
            U[i, j], V[i, j] = grad[0], grad[1]
    return U, V


if __name__ == '__main__':
    # Define the points for the plot
    x = np.linspace(-10, 10, 100)
    y = np.linspace(-10, 10, 100)
    X, Y = np.meshgrid(x, y)

    # (a) Matrix A such that f has a unique minimum
    A_min = np.array([[2, 0], [0, 2]])
    Z_min = f(A_min, X, Y)
    U_min, V_min = grad_f(A_min, X, Y)

    # (b) Matrix A such that f has a unique maximum
    A_max = np.array([[-2, 0], [0, -2]])
    Z_max = f(A_max, X, Y)
    U_max, V_max = grad_f(A_max, X, Y)

    # (c) Matrix A such that f has a unique saddle point
    A_saddle = np.array([[2, 0], [0, -2]])
    Z_saddle = f(A_saddle, X, Y)
    U_saddle, V_saddle = grad_f(A_saddle, X, Y)

    # Plot the 3D surface, contour lines, and gradient
    fig = plt.figure(figsize=(18, 6))

    ax1 = fig.add_subplot(131, projection='3d')
    ax1.plot_surface(X, Y, Z_min, cmap='viridis')
    ax1.contour(X, Y, Z_min, levels=3, offset=-1)
    ax1.quiver(X[::5, ::5], Y[::5, ::5], -1, U_min[::5, ::5], V_min[::5, ::5], 0, length=0.5, color='r')
    ax1.set_title('Unique Minimum')

    ax2 = fig.add_subplot(132, projection='3d')
    ax2.plot_surface(X, Y, Z_max, cmap='viridis')
    ax2.contour(X, Y, Z_max, levels=3, offset=-1)
    ax2.quiver(X[::5, ::5], Y[::5, ::5], -1, U_max[::5, ::5], V_max[::5, ::5], 0, length=0.5, color='r')
    ax2.set_title('Unique Maximum')

    ax3 = fig.add_subplot(133, projection='3d')
    ax3.plot_surface(X, Y, Z_saddle, cmap='viridis')
    ax3.contour(X, Y, Z_saddle, levels=3, offset=-1)
    ax3.quiver(X[::5, ::5], Y[::5, ::5], -1, U_saddle[::5, ::5], V_saddle[::5, ::5], 0, length=0.5, color='r')
    ax3.set_title('Unique Saddle Point')

    plt.show()
