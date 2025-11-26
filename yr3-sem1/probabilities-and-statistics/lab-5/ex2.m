% 2. The following data set represents the number of new computer accounts registered during ten consecutive days, by some online service provider company.
% 43, 37, 50, 51, 58, 105, 52, 45, 45, 10.

new_computer_accounts = [43, 37, 50, 51, 58, 105, 52, 45, 45, 10];

% a) Compute the mean, median, quartiles and standard deviation.

mean_new_computer_accounts = mean(new_computer_accounts);
median_new_computer_accounts = median(new_computer_accounts);
quartiles_new_computer_accounts = quantile(new_computer_accounts, [0.25 0.5 0.75]);
std_new_computer_accounts = std(new_computer_accounts);
IQR_new_computer_accounts = iqr(new_computer_accounts);

% b) Construct the boxplot and check for outliers using the 3/2 (IQR) rule.

lower_bound = quartiles_new_computer_accounts(1) - 1.5 * IQR_new_computer_accounts;
upper_bound = quartiles_new_computer_accounts(3) + 1.5 * IQR_new_computer_accounts;

outliers = new_computer_accounts(new_computer_accounts < lower_bound | new_computer_accounts > upper_bound);

fprintf("\n=== Outlier Detection (1.5 × IQR Rule) ===\n");
fprintf("Lower Bound: %.2f\n", lower_bound);
fprintf("Upper Bound: %.2f\n", upper_bound);
fprintf("Outliers: ");
disp(outliers);

% Boxplot
figure;
boxplot(new_computer_accounts);
title("Boxplot of New Computer Accounts (Original Data)");
ylabel("Number of Accounts");

% c) Remove the detected outliers and compute the mean, median, quartiles and standard deviation again.

new_computer_accounts_clean = new_computer_accounts(new_computer_accounts >= lower_bound & new_computer_accounts <= upper_bound);

mean_clean = mean(new_computer_accounts_clean);
median_clean = median(new_computer_accounts_clean);
quartiles_clean = quantile(new_computer_accounts_clean, [0.25 0.5 0.75]);
std_clean = std(new_computer_accounts_clean);
IQR_clean = iqr(new_computer_accounts_clean);

fprintf("\n=== Original Data Statistics ===\n");
fprintf("Mean:       %.2f\n", mean_new_computer_accounts);
fprintf("Median:     %.2f\n", median_new_computer_accounts);
fprintf("Q1:         %.2f\n", quartiles_new_computer_accounts(1));
fprintf("Q2/Median:  %.2f\n", quartiles_new_computer_accounts(2));
fprintf("Q3:         %.2f\n", quartiles_new_computer_accounts(3));
fprintf("IQR:        %.2f\n", IQR_new_computer_accounts);
fprintf("Std Dev:    %.2f\n", std_new_computer_accounts);

fprintf("\n=== Cleaned Data Statistics (Outliers Removed) ===\n");
fprintf("Mean:       %.2f\n", mean_clean);
fprintf("Median:     %.2f\n", median_clean);
fprintf("Q1:         %.2f\n", quartiles_clean(1));
fprintf("Q2/Median:  %.2f\n", quartiles_clean(2));
fprintf("Q3:         %.2f\n", quartiles_clean(3));
fprintf("IQR:        %.2f\n", IQR_clean);
fprintf("Std Dev:    %.2f\n", std_clean);

% What conclusion can be drawn about the effect of outliers on basic descriptive statistics?

% Removing outliers reduced mean slightly
% Median did not change => robust statistics
% IQR, Q3 decreased due to the highest value (105) got removed; Q1 increased since the lowest value (10) was removed
% STD Dev changed a lot => is sensitive to extreme values

