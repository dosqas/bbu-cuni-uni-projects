% 1. In order to ensure efficient usage of a server, it is necessary to estimate the average number of concurrent users. These numbers of concurrent users were recorded at 40 randomly selected times
% 67 84 88 68 64 75 69 71 82 78
% 87 63 87 78 62 68 76 88 88 64
% 81 77 89 66 79 74 84 89 89 72
% 49 76 59 41 67 87 52 66 51 42
% Past experience indicates that the number of concurrent users at all times has a standard deviation of 10.5.

X = [67, 84, 88, 68, 64, 75, 69, 71, 82, 78, 87, 63, 87, 78, 62, 68, 76, 88, 88, 64, 81, 77, 89, 66, 79, 74, 84, 89, 89, 72, 49, 76, 59, 41, 67, 87, 52, 66, 51, 42];
n = length(X);
x_bar = mean(X);
sigma = 10.5;
alpha_a = 0.01;


% a) Construct a 99% confidence interval for the expectation of the number of concurrent users.
alpha = 0.01;
z = norminv(1 - alpha/2);
CI_a = x_bar + [-1, 1] * z * sigma / sqrt(n);


% b) At the 5% significance level, do these data provide significant evidence that the average number of concurrent users is greater than 70? What about at 8%? Find also the rejection region for this test.
mu0 = 70;
[h_5, p_5, ci_5, stats_5] = ztest(X, mu0, sigma, "Tail", "right", "Alpha", 0.05);
[h_8, p_8, ci_8, stats_8] = ztest(X, mu0, sigma, "Tail", "right", "Alpha", 0.08);
z_value = (x_bar - mu0) / (sigma / sqrt(n));


zcrit_5 = norminv(1 - 0.05);
zcrit_8 = norminv(1 - 0.08);

RR_5 = [zcrit_5, inf];
RR_8 = [zcrit_8, inf];


% c) A manager questions the assumptions above. She says that the sample of 40 numbers of concurrent users has a sample standard deviation of s = 13.2392, which is significantly different from the assumed value of σ = 10.5. Do you agree with the manager? Assuming that the number of concurrent users has a Normal distribution, conduct a suitable test for the standard deviation.
s = std(X);
sigma0 = 10.5;

[h_c, p_c, ci_c, stats_c] = vartest(X, sigma0^2, "Tail", "both", "Alpha", 0.05);
chi2_value = (n - 1) * s^2 / sigma0^2;

chi2_lower = chi2inv(0.025, n-1); % lower critical value
chi2_upper = chi2inv(0.975, n-1); % upper critical value
RR_chi2 = [-inf, chi2_lower; chi2_upper, inf];


fprintf("PART (a): 99%% CI for the mean (sigma known)\n");
fprintf("Mean = %.3f\n", x_bar);
fprintf("99%% CI = [%.3f , %.3f]\n\n", CI_a(1), CI_a(2));

fprintf("PART (b): Hypothesis tests for mu > 70\n");
fprintf("Test statistic z = %.3f\n\n", z_value);

fprintf("At 5%% significance level:\n");
fprintf("  p-value = %.3f\n", p_5);
fprintf("  Rejection region at 5%%: z > %.3f (i.e., [%.3f, inf))\n", zcrit_5, RR_5(1));
fprintf("  Decision (1 = reject H0): %d\n\n", h_5);

fprintf("At 8%% significance level:\n");
fprintf("  p-value = %.3f\n", p_8);
fprintf("  Rejection region at 8%%: z > %.3f (i.e., [%.3f, inf))\n\n", zcrit_8, RR_8(1));
fprintf("  Decision (1 = reject H0): %d\n\n", h_8);

fprintf("PART (c): Test for standard deviation\n");
fprintf("Sample std = %.4f\n", s);
fprintf("Test statistic chi^2 = %.3f\n", chi2_value);
fprintf("p-value = %.3f\n", p_c);
fprintf("Decision (1 = reject H0: sigma = 10.5): %d\n\n", h_c);
fprintf("Rejection region for sigma test (chi^2, two-tailed, alpha=0.05):\n");
fprintf("  (-inf, %.3f] U [%.3f, inf)\n\n", chi2_lower, chi2_upper);
