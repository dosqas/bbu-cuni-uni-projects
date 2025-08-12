# SOPTELEA SEBASTIAN
def function(x):
    return x ** 2 # f(x) = x42

def gradient(x):
    return 2 * x # f(x) = x^2 => f'(x) = 2x

def gradient_descent(gradient, learning_rate, initial_x, num_iterations):
    x = initial_x
    for _ in range(num_iterations) :
        x = x - learning_rate * gradient(x)
    return x

learning_rate = 0.01
initial_x = 4
num_iterations = 200

result = gradient_descent(gradient, learning_rate, initial_x, num_iterations)

print("Result after applying gradient descent method is", result)