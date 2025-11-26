% 1. The numbers of blocked intrusion attempts on each day, during the first two weeks of the month, on some site, were
% 56, 47, 49, 37, 38, 60, 50, 43, 43, 59, 50, 56, 54, 58.
% After the change of firewall settings, the numbers of blocked intrusions during the next 20 days were
% 53, 21, 32, 49, 45, 38, 44, 33, 32, 43, 53, 46, 36, 48, 39, 35, 37, 36, 39, 45.
% To compare the number of blocked intrusions before and after the change,

first_two_week_attempts = [56, 47, 49, 37, 38, 60, 50, 43, 43, 59, 50, 56, 54, 58];
after_change_attempts = [53, 21, 32, 49, 45, 38, 44, 33, 32, 43, 53, 46, 36, 48, 39, 35, 37, 36, 39, 45];

% a) compute the minimum, maximum, mean, standard deviation, quartiles and interquartile range, for the two sets of data;

% Before change
min_before = min(first_two_week_attempts);
max_before = max(first_two_week_attempts);
mean_before = mean(first_two_week_attempts);
std_before = std(first_two_week_attempts);
quartiles_before = quantile(first_two_week_attempts, [0.25 0.5 0.75]);
iqr_before = iqr(first_two_week_attempts);

% After change
min_after = min(after_change_attempts);
max_after = max(after_change_attempts);
mean_after = mean(after_change_attempts);
std_after = std(after_change_attempts);
quartiles_after = quantile(after_change_attempts, [0.25 0.5 0.75]);
iqr_after = iqr(after_change_attempts);

% b) construct histograms (with 10 classes), one on top of the other;
figure;
subplot(2,1,1);
histogram(first_two_week_attempts, 10);
title("Blocked Intrusions (Before Change)");
xlabel("Attempts"); ylabel("Frequency");

subplot(2,1,2);
histogram(after_change_attempts, 10);
title("Blocked Intrusions (After Change)");
xlabel("Attempts"); ylabel("Frequency");

% c) construct side-by-side boxplots;
figure;

all_values = [first_two_week_attempts(:); after_change_attempts(:)];
groups = [repmat("Before", length(first_two_week_attempts), 1); ...
          repmat("After", length(after_change_attempts), 1)];

boxplot(all_values, groups);
title("Side-by-Side Boxplots: Blocked Intrusion Attempts");
ylabel("Attempts");



% Comment on your findings.

fprintf("\n=== Summary Statistics: Before Firewall Change ===\n");
fprintf("Minimum:              %.2f\n", min_before);
fprintf("Maximum:              %.2f\n", max_before);
fprintf("Mean:                 %.2f\n", mean_before);
fprintf("Standard Deviation:   %.2f\n", std_before);
fprintf("Q1 (25th percentile): %.2f\n", quartiles_before(1));
fprintf("Median:               %.2f\n", quartiles_before(2));
fprintf("Q3 (75th percentile): %.2f\n", quartiles_before(3));
fprintf("IQR:                  %.2f\n", iqr_before);

fprintf("\n=== Summary Statistics: After Firewall Change ===\n");
fprintf("Minimum:              %.2f\n", min_after);
fprintf("Maximum:              %.2f\n", max_after);
fprintf("Mean:                 %.2f\n", mean_after);
fprintf("Standard Deviation:   %.2f\n", std_after);
fprintf("Q1 (25th percentile): %.2f\n", quartiles_after(1));
fprintf("Median:               %.2f\n", quartiles_after(2));
fprintf("Q3 (75th percentile): %.2f\n", quartiles_after(3));
fprintf("IQR:                  %.2f\n", iqr_after);

% Mean is smaller after firewall change => fewer attacks; more efficient filtering
% STD Dev increased slightly => daily variation became marginally higher
% IQR decreased => middle 50% of the data became more concentrated
% Min and Max became smaller => fewer attacks
% All 3 quartiles decreased => reduction across the board

