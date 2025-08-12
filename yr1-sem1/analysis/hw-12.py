# Soptelea Sebastian group 917
# Ridge regression for the California housing dataset

from sklearn.datasets import fetch_california_housing
from sklearn.model_selection import train_test_split
from sklearn.linear_model import Ridge
from sklearn.metrics import mean_squared_error
import matplotlib.pyplot as plt

# Here we try to load the California housing dataset :)
california = fetch_california_housing(as_frame=True)
X, y = california.data, california.target

# Next, we split the data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Creating a Ridge regression object
rdg = Ridge(alpha = 0.5)

# Fitting the model
rdg.fit(X_train, y_train)

# Making predictions using the testing set
y_pred = rdg.predict(X_test)

# Calculating the mean squared error of the predictions
mse = mean_squared_error(y_test, y_pred)

# Plotting actual vs predicted values
plt.scatter(y_test, y_pred)
plt.xlabel('Actual Prices')
plt.ylabel('Predicted Prices')
plt.title('Actual vs Predicted House Prices')
plt.show()

print(f'Mean Squared Error: {mse}')
