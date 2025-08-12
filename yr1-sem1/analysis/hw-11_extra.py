# Soptelea Sebastian group 917
import numpy as np


# Function 1: f(x, y) = x^2 + 3y^2
def func1(x):
    return x[0] ** 2 + 3 * x[1] ** 2


def grad1(x):
    return np.array([2 * x[0], 6 * x[1]])


def hessian1(x):
    return np.array([[2, 0], [0, 6]])


# Function 2: f(x, y) = 100(y - x^2)^2 + (1 - x)^2
def func2(x):
    return 100 * (x[1] - x[0] ** 2) ** 2 + (1 - x[0]) ** 2


def grad2(x):
    return np.array([400 * x[0] ** 3 - 400 * x[0] * x[1] + 2 * x[0] - 2, 200 * (x[1] - x[0] ** 2)])


def hessian2(x):
    return np.array([[1200 * x[0] ** 2 - 400 * x[1] + 2, -400 * x[0]], [-400 * x[0], 200]])


def newtons_method(func, grad, hessian, x0, tol=1e-6, max_iter=1000):
    x = x0
    for i in range(max_iter):
        x_new = x - np.linalg.inv(hessian(x)).dot(grad(x))
        if np.linalg.norm(x_new - x) < tol:
            return x_new, i + 1
        x = x_new
    return x, max_iter


def gradient_descent(func, grad, x0, learning_rate=0.01, tol=1e-6, max_iter=1000):
    x = x0
    for i in range(max_iter):
        x_new = x - learning_rate * grad(x)
        if np.linalg.norm(x_new - x) < tol:
            return x_new, i + 1
        x = x_new
    return x, max_iter


# Minimize function 1
x0 = np.array([2.0, 2.0])  # initial value
x_min, num_iter = newtons_method(func1, grad1, hessian1, x0)
print(f"Function 1 converged to {x_min} in {num_iter} iterations.")

# Minimize function 2
x0 = np.array([-1.2, 1.0])  # initial value
x_min, num_iter = newtons_method(func2, grad2, hessian2, x0)
print(f"Function 2 converged to {x_min} in {num_iter} iterations.'\n'")

# Minimize function 1 using gradient descent
x0 = np.array([2.0, 2.0])  # initial value
x_min, num_iter = gradient_descent(func1, grad1, x0, learning_rate=0.001, max_iter=15000)
print(f"Function 1 (Gradient Descent) converged to {x_min} in {num_iter} iterations.")

# Minimize function 2 using gradient descent
x0 = np.array([-1.2, 1.0])  # initial value
x_min, num_iter = gradient_descent(func2, grad2, x0, learning_rate=0.001, max_iter=15000)
print(f"Function 2 (Gradient Descent) converged to {x_min} in {num_iter} iterations.")
