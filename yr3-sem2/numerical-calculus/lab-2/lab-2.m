%% Taylor Series Analysis: Logarithms and Binomial Expansion
clear; clc; close all;

%% --- Application 3: Logarithmic Functions ---
fprintf('--- Application 3: ln(1+x) and ln((1+x)/(1-x)) ---\n');

% a) Graphing ln(1+x) and its Taylor polynomials
x_vals = -0.9:0.01:1;
f_ln = log(1 + x_vals);

figure('Name', 'Application 3: Logarithmic Approximations');
subplot(2,1,1); hold on;
plot(x_vals, f_ln, 'k', 'LineWidth', 2, 'DisplayName', 'ln(1+x)');

syms x;
for n = [2, 5]
    T = taylor(log(1 + x), x, 0, 'Order', n+1);
    T_func = matlabFunction(T);
    plot(x_vals, T_func(x_vals), '--', 'DisplayName', ['Degree ', num2mstr(n)]);
end
grid on; xlabel('x'); ylabel('f(x)');
title('ln(1+x) and Taylor Polynomials (n=2, 5)');
legend('Location', 'southeast');
ylim([-3 2]);

% b) Terms for ln(2) via ln(1+x)
% To find ln(2), we set x = 1. The series is 1 - 1/2 + 1/3 - 1/4...
% Error for alternating series: |R_n| <= |a_{n+1}|. 
% We want 1/(n+1) < 0.5e-5 => n+1 > 200,000.
n_req_3b = (1 / 0.5e-5) - 1;
fprintf('3b) For ln(2) via ln(1+x), we need n = %d terms.\n', ceil(n_req_3b));

% c) Taylor series for ln(1-x)
% Substitute -x into formula (5)
T_ln_minus = taylor(log(1 - x), x, 0, 'Order', 10);
fprintf('3c) Expansion for ln(1-x): %s\n', char(vpa(T_ln_minus, 4)));

% d) Derive Taylor series for ln((1+x)/(1-x))
% ln((1+x)/(1-x)) = ln(1+x) - ln(1-x)
% This cancels even powers and doubles odd powers: 2 * (x + x^3/3 + x^5/5 + ...)
ln_comb = simplify(taylor(log(1+x), x, 0, 'Order', 10) - taylor(log(1-x), x, 0, 'Order', 10));
fprintf('3d) Expansion for ln((1+x)/(1-x)): %s\n', char(ln_comb));

% e) Approximate ln(2) using the combined formula
% (1+x)/(1-x) = 2  => 1+x = 2 - 2x => 3x = 1 => x = 1/3
x_target = 1/3;
sum_ln2 = 0;
n_terms = 0;
error_val = 1;
while error_val > 0.5e-5
    n_terms = n_terms + 1;
    k = 2*n_terms - 1; % Odd powers
    term = 2 * (x_target^k / k);
    sum_ln2 = sum_ln2 + term;
    error_val = term; % Upper bound for this series
end
fprintf('3e) Using the formula from (d), ln(2) approx: %.6f\n', sum_ln2);
fprintf('    Only %d terms are necessary (vs 200,000!)\n\n', n_terms);

%% --- Application 4: Binomial Approximation (Optional) ---
fprintf('--- Application 4: cube root of 999 ---\n');

% We want cuberoot(999). 999 is not |x| < 1. 
% Factor out a nearby cube: 999 = 1000 - 1 = 1000(1 - 1/1000)
% cuberoot(999) = 10 * (1 - 0.001)^(1/3)
% Here a = 1/3 and x = -0.001 (which is |x| < 1)

a = 1/3;
x_val = -0.001;
target_precision = 0.5e-10;

% Initialize binomial series
approx_root = 1; % The "1" in the formula
term = 1;
n = 0;

while abs(term * 10) > target_precision
    n = n + 1;
    % Binomial coefficient (a over n) formula:
    % current_coeff = previous_coeff * (a - n + 1) / n
    % We update the term iteratively:
    term = term * (a - n + 1) / n * x_val;
    approx_root = approx_root + term;
end

result = 10 * approx_root;
fprintf('4) Approximate cuberoot(999): %.11f\n', result);
fprintf('   Actual value:             %.11f\n', 999^(1/3));
fprintf('   Terms used: %d\n', n);