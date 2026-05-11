%% Lab 11: Application 1 - Romberg Integration
clear; clc;

% Define the problem
f = @(x) 1 ./ (2 + sin(x));
a = 0;
b = pi/2;
exact_val = (pi * sqrt(3)) / 9;

% Tolerance for 5 correct decimals
tol = 1e-6; 
max_rows = 10;

% Run Romberg
[I_approx, R_table, evaluations] = Romberg(f, a, b, tol, max_rows);

% Results
fprintf('--- Romberg Integration Results ---\n');
fprintf('Exact Value:      %.10f\n', exact_val);
fprintf('Romberg Approx:   %.10f\n', I_approx);
fprintf('Absolute Error:   %e\n', abs(exact_val - I_approx));
fprintf('Function Evals:   %d\n', evaluations);
fprintf('\nRomberg Table:\n');
disp(R_table);

function [I, table, n_eval] = Romberg(f, a, b, tol, max_rows)
    % I: The final approximated value
    % table: The Romberg triangle
    % n_eval: Total number of function evaluations
    
    R = zeros(max_rows, max_rows);
    n_eval = 0;
    
    % First row: Basic Trapezoid with 1 interval
    h = b - a;
    R(1,1) = (h/2) * (f(a) + f(b));
    n_eval = n_eval + 2;
    
    for i = 2:max_rows
        % Step 1: Compute next Trapezoidal estimate (double the nodes)
        h = h / 2;
        n_intervals = 2^(i-1);
        
        % Only evaluate the new midpoints to save time
        sum_new_points = 0;
        for k = 1:2:(n_intervals)
            sum_new_points = sum_new_points + f(a + k*h);
            n_eval = n_eval + 1;
        end
        
        R(i,1) = 0.5 * R(i-1,1) + h * sum_new_points;
        
        % Step 2: Richardson Extrapolation (Filling the row)
        for j = 2:i
            R(i,j) = R(i,j-1) + (R(i,j-1) - R(i-1,j-1)) / (4^(j-1) - 1);
        end
        
        % Step 3: Check for convergence
        if abs(R(i,i) - R(i,i-1)) < tol
            I = R(i,i);
            table = R(1:i, 1:i);
            return;
        end
    end
    I = R(max_rows, max_rows);
    table = R;
end