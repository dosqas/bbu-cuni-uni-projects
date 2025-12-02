% 3. The data below represent investments, in 1000 EUR’s, in the development of new software by some small computer company over an 11-year period.

% Year,        X 2014 2015 2016 2017 2018 2019 2020 2021 2022 2023 2024
% Investment,  Y 17   23   31   29   33   39   39   40   41   44   47

X = 2014:2024;
Y = [17 23 31 29 33 39 39 40 41 44 47];

% a) Compute the means and variances of the two sets of data;
mean_X = mean(X);
var_X = var(X);

mean_Y = mean(Y);
var_Y = var(Y);

% b) Compute the covariance and the correlation coefficient of X and Y;
cov_XY = cov(X, Y);
covariance = cov_XY(1,2);

corr_coeff = corrcoef(X, Y);
correlation = corr_coeff(1,2);

% c) Draw the scatter plot, fit a linear regression model and use it to predict the amount invested in 2026.
scatter(X, Y, 'filled');
title("Scatter Plot with Regression Line");
xlabel("Year"); ylabel("Investment (1000 EUR)");
grid on;

lsline;

p = polyfit(X, Y, 1);

prediction_2026 = polyval(p, 2026);


fprintf("\n=== Part (a): Means and Variances ===\n");
fprintf("Mean of X (years): %.2f\n", mean_X);
fprintf("Variance of X: %.2f\n", var_X);
fprintf("Mean of Y (investment): %.2f\n", mean_Y);
fprintf("Variance of Y: %.2f\n", var_Y);

fprintf("\n=== Part (b): Covariance and Correlation ===\n");
fprintf("Covariance(X, Y): %.2f\n", covariance);
fprintf("Correlation coefficient: %.4f\n", correlation);

fprintf("\n=== Part (c): Regression and Prediction ===\n");
fprintf("Linear model: Y = %.4f * X + %.4f\n", p(1), p(2));
fprintf("Predicted investment for 2026: %.2f (1000 EUR)\n", prediction_2026);