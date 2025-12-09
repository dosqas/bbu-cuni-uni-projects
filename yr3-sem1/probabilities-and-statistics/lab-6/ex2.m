% 2. Salaries of entry-level computer engineers have a Normal distribution with unknown mean and variance.
% Ten randomly selected computer engineers have monthly salaries (in 1000 RONs):
% 5.2 3.1 8.1 7.9 6.4 5.6 7.5 8.0 4.3 8.8.

X = [5.2, 3.1, 8.1, 7.9, 6.4, 5.6, 7.5, 8.0, 4.3, 8.8];
n = length(X);
x_bar = mean(X);
s = std(X);

% a) Find a 98% right-sided confidence interval for the average salary of an entry-level computer engineer.
alpha_a = 0.02;  % right-tailed
t_crit = tinv(1 - alpha_a, n-1);
CI_a = [x_bar - t_crit * s / sqrt(n), Inf];


% b) Does this sample provide significant evidence, at the 5% level of significance, that the monthly average
% salary of all entry-level computer engineers is less than 6500? Explain. Find also the rejection region for this test.
mu0 = 6.5;
alpha_b = 0.05;
t_stat = (x_bar - mu0) / (s / sqrt(n));      % test statistic
t_crit_b = tinv(alpha_b, n-1);               % critical t-value (left-tailed)
RR_b = [-inf, t_crit_b];                     % rejection region
p_value = tcdf(t_stat, n-1);                 % left-tailed p-value
h_b = t_stat < t_crit_b;                     % 1 = reject H0

% c) Looking at this sample, one may think that the starting salaries have a great deal of variability.
% Construct a 90% confidence interval for the standard deviation of entry level salaries.
alpha_c = 0.10;
chi2_lower = chi2inv(alpha_c/2, n-1);
chi2_upper = chi2inv(1 - alpha_c/2, n-1);
CI_sigma = [sqrt((n-1)*s^2 / chi2_upper), sqrt((n-1)*s^2 / chi2_lower)];



fprintf("PART (a): 98%% right-sided CI for the mean\n");
fprintf("Sample mean = %.3f\n", x_bar);
fprintf("98%% right-sided CI = [%.3f , %.3f]\n\n", CI_a(1), CI_a(2));

fprintf("PART (b): Test if mean < 6.5 at 5%% significance level\n");
fprintf("Test statistic t = %.3f\n", t_stat);
fprintf("p-value = %.3f\n", p_value);
fprintf("Rejection region: t <= %.3f\n", t_crit_b);
fprintf("Decision (1 = reject H0): %d\n\n", h_b);

fprintf("PART (c): 90%% CI for the standard deviation\n");
fprintf("Sample std = %.3f\n", s);
fprintf("90%% CI = [%.3f , %.3f]\n", CI_sigma(1), CI_sigma(2));