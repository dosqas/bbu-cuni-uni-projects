% 1. Two friends who like to play a popular online game, debate which one 
% of them is a better player. Player A says he is a stronger player because
% his average score is, in general, higher. However, player B replies that he
% is more stable, because the variability of his scores is lower. The actual 
% scores of the two friends (presumably, independent and Normally distributed), 
% recorded over one month, are given in the table below.
% Player A 85 92 97 65 75 96
% Player B 81 79 76 84 83 77 78 82

A = [85 92 97 65 75 96];
B = [81 79 76 84 83 77 78 82];

nA = length(A);
nB = length(B);

xA_bar = mean(A);
xB_bar = mean(B);

sA = std(A);
sB = std(B);


% a) At the 5% significance level, is there significant evidence to support 
% player A’s claim? Find also the rejection regions.

alpha_a = 0.05;

% Welch t-test statistic
% Measures how many std errors the difference in the sample mean is away from 0
% (accounts for uneven variances)
t_stat_a = (xA_bar - xB_bar) / sqrt(sA^2/nA + sB^2/nB);

% Welch degrees of freedom
% Variances may not be equal, so we cant use nA + nB - 2
df = (sA^2/nA + sB^2/nB)^2 / ...
     ((sA^2/nA)^2/(nA-1) + (sB^2/nB)^2/(nB-1));

% For a one-sided test (Player A > Player B)
% tinv gives the t-value for which the upper tail probability = alpha
t_crit_a = tinv(1 - alpha_a, df);

% Probability of observing a t-statistic as extreme or more extreme under null hypothesis
p_value_a = 1 - tcdf(t_stat_a, df);
h_a = t_stat_a > t_crit_a;


% b) Is there significant evidence to support player B’s claim? Find also 
% the rejection region.

alpha_b = 0.05;

% Ratio of sample variances
% Large => A is bigger; else B is bigger
F_stat = sA^2 / sB^2;

F_lower = finv(alpha_b/2, nA-1, nB-1);       % lower critical F value
F_upper = finv(1 - alpha_b/2, nA-1, nB-1);   % upper critical F value

% Two-tailed: multiply by 2 the smaller tail probability
p_value_b = 2 * min( ...
    fcdf(F_stat, nA-1, nB-1), ...     % lower tail probability
    1 - fcdf(F_stat, nA-1, nB-1));    % upper tail probability

h_b = (F_stat <= F_lower) || (F_stat >= F_upper);


% c) Construct a 90% confidence interval for the difference of population means.

alpha_c = 0.10;

% Two-tailed because CI includes both directions
% df is from Welch approximation (due to unequal variances)
t_crit_c = tinv(1 - alpha_c/2, df);

% Standard error of the difference in means
margin = t_crit_c * sqrt(sA^2/nA + sB^2/nB);

% Lower and upper bounds of (mu_A - mu_B)
CI_diff_means = [(xA_bar - xB_bar) - margin, ...
                 (xA_bar - xB_bar) + margin];


% d) Find a 92% confidence interval for the ratio of population standard deviations.

alpha_d = 0.08;

F_low = finv(alpha_d/2, nA-1, nB-1);      % lower tail
F_up  = finv(1 - alpha_d/2, nA-1, nB-1);  % upper tail

CI_sd_ratio = [ ...
    sqrt((sA^2/sB^2) / F_up), ...   % lower bound
    sqrt((sA^2/sB^2) / F_low) ];    % upper bound



fprintf("PART (a): Test if Player A has higher mean score (alpha = 0.05)\n");
fprintf("t statistic = %.3f\n", t_stat_a);
fprintf("p-value = %.3f\n", p_value_a);
fprintf("Rejection region: t >= %.3f\n", t_crit_a);
fprintf("Decision (1 = reject H0): %d\n\n", h_a);

fprintf("PART (b): Test for equality of variances (alpha = 0.05)\n");
fprintf("F statistic = %.3f\n", F_stat);
fprintf("p-value = %.3f\n", p_value_b);
fprintf("Rejection region: F <= %.3f or F >= %.3f\n", F_lower, F_upper);
fprintf("Decision (1 = reject H0): %d\n\n", h_b);

fprintf("PART (c): 90%% CI for difference of means (mu_A - mu_B)\n");
fprintf("CI = [%.3f , %.3f]\n\n", CI_diff_means(1), CI_diff_means(2));

fprintf("PART (d): 92%% CI for ratio of standard deviations (sigma_A / sigma_B)\n");
fprintf("CI = [%.3f , %.3f]\n", CI_sd_ratio(1), CI_sd_ratio(2));