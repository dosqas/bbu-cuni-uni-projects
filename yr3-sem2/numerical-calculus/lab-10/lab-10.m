%% Lab Nr. 10: Numerical Integration - Proper Solutions
clear; clc; close all;

%% --- Application 1: Approximate ln(2) ---
% We approximate integral of 1/x from 1 to 2.
fprintf('--- Application 1: ln(2) Approximation ---\n');
f1 = @(x) 1./x;
a1 = 1; b1 = 2;
exact_ln2 = log(2);

% For 3 correct decimals, we select n=20 as a safe bet for all rules
n1 = 20; 
resR = CompositeRectangle(f1, a1, b1, n1);
resT = CompositeTrapezoid(f1, a1, b1, n1);
resS = CompositeSimpson(f1, a1, b1, n1);

fprintf('Exact ln(2):   %.10f\n', exact_ln2);
fprintf('Rectangle (n=%d): %.10f | Error: %e\n', n1, resR, abs(exact_ln2 - resR));
fprintf('Trapezoid (n=%d): %.10f | Error: %e\n', n1, resT, abs(exact_ln2 - resT));
fprintf('Simpson   (n=%d): %.10f | Error: %e\n\n', n1, resS, abs(exact_ln2 - resS));


%% --- Application 2: Area and Convergence Rates (All Rules) ---
fprintf('--- Application 2: Convergence Rates for All Rules ---\n');
f2 = @(x) (x .* exp(-x)) ./ (x.^2 + 1);
a2 = 0; b2 = 1;
exact_area = integral(f2, a2, b2);

% a) Plotting (Already in previous script)
figure('Name', 'Application 2: Function Graph');
x_range2 = linspace(0, 1, 500);
plot(x_range2, f2(x_range2), 'b', 'LineWidth', 2);
title('f(x) = xe^{-x} / (x^2 + 1)'); grid on;

% b) Error and Ratio Analysis for all 3 rules
ns = 2.^(1:8); % 2, 4, 8, 16, 32, 64, 128, 256
errsR = zeros(size(ns)); errsT = zeros(size(ns)); errsS = zeros(size(ns));

for i = 1:length(ns)
    n = ns(i);
    errsR(i) = abs(exact_area - CompositeRectangle(f2, a2, b2, n));
    errsT(i) = abs(exact_area - CompositeTrapezoid(f2, a2, b2, n));
    errsS(i) = abs(exact_area - CompositeSimpson(f2, a2, b2, n));
end

% Print the formatted table
fprintf('n\t| Rect Error\tRatio\t| Trap Error\tRatio\t| Simp Error\tRatio\n');
fprintf('----------------------------------------------------------------------------\n');
for i = 1:length(ns)
    if i == 1
        fprintf('%d\t| %e\t-\t| %e\t-\t| %e\t-\n', ns(i), errsR(i), errsT(i), errsS(i));
    else
        ratioR = errsR(i-1) / errsR(i);
        ratioT = errsT(i-1) / errsT(i);
        ratioS = errsS(i-1) / errsS(i);
        fprintf('%d\t| %e\t%.2f\t| %e\t%.2f\t| %e\t%.2f\n', ...
                ns(i), errsR(i), ratioR, errsT(i), ratioT, errsS(i), ratioS);
    end
end

fprintf('\nSummary:\n');
fprintf('Rectangle/Trapezoid: Ratios approach 4.00 (Order h^2)\n');
fprintf('Simpson: Ratios approach 16.00 (Order h^4)\n\n');


%% --- Application 3: Error Function erf(x) ---
fprintf('--- Application 3: erf(x) Tabulation ---\n');
tol = 1e-7;
x_vals = 0.1:0.1:1.0;

fprintf('x\t  Adaptive Trap\t  MATLAB erf\t  MATLAB integral\tError\n');
fprintf('--------------------------------------------------------------------------\n');
for x = x_vals
    % erf(x) = (2/sqrt(pi)) * integral of e^-t^2 from 0 to x
    integrand3 = @(t) exp(-t.^2);
    raw_int = AdaptiveQuad(integrand3, 0, x, tol, 'trapezoid');
    my_erf = (2/sqrt(pi)) * raw_int;
    
    mat_erf = erf(x);
    mat_int = (2/sqrt(pi)) * integral(integrand3, 0, x);
    
    fprintf('%.1f\t  %.10f\t  %.10f\t  %.10f\t%e\n', x, my_erf, mat_erf, mat_int, abs(my_erf - mat_erf));
end
fprintf('\n');


%% --- Application 4: Curve Length ---
fprintf('--- Application 4: Length of sin(pi*x) ---\n');
f4 = @(x) sin(pi * x);
df4 = @(x) pi * cos(pi * x);
arc_integrand = @(x) sqrt(1 + (df4(x)).^2);

% a) Plotting
figure('Name', 'Application 4: Curve Length');
x_range4 = linspace(0, 1, 500);
plot(x_range4, f4(x_range4), 'r', 'LineWidth', 2);
title('Curve y = sin(\pi x)'); xlabel('x'); ylabel('y'); grid on; axis equal;

% b) Adaptive Simpson for length
L_approx = AdaptiveQuad(arc_integrand, 0, 1, 1e-8, 'simpson');
L_exact = integral(arc_integrand, 0, 1);

fprintf('Approximate Length (Adaptive Simpson): %.10f\n', L_approx);
fprintf('Exact Length (MATLAB integral):        %.10f\n', L_exact);
fprintf('Absolute Difference:                   %e\n\n', abs(L_approx - L_exact));


%% --- CORE ALGORITHM FUNCTIONS ---

function I = CompositeRectangle(f, a, b, n)
    h = (b - a) / n;
    midpoints = a + (0:n-1)*h + h/2;
    I = h * sum(f(midpoints));
end

function I = CompositeTrapezoid(f, a, b, n)
    h = (b - a) / n;
    x = linspace(a, b, n+1);
    y = f(x);
    I = (h/2) * (y(1) + 2*sum(y(2:end-1)) + y(end));
end

function I = CompositeSimpson(f, a, b, n)
    if mod(n,2) ~= 0, n = n + 1; end % Ensure n is even
    h = (b - a) / n;
    x = linspace(a, b, n+1);
    y = f(x);
    % Formula: h/3 * (f0 + 4*f_odd + 2*f_even + fn)
    I = (h/3) * (y(1) + 4*sum(y(2:2:end)) + 2*sum(y(3:2:end-1)) + y(end));
end

function I = AdaptiveQuad(f, a, b, tol, method)
    % Recursive Adaptive Quadrature
    m = (a + b) / 2;
    if strcmp(method, 'trapezoid')
        I1 = CompositeTrapezoid(f, a, b, 1);
        I2 = CompositeTrapezoid(f, a, b, 2);
        scale = 3; 
    else % Simpson
        I1 = CompositeSimpson(f, a, b, 2);
        I2 = CompositeSimpson(f, a, b, 4);
        scale = 15;
    end
    
    % Check tolerance
    if abs(I2 - I1) < scale * tol
        I = I2; % Good enough
    else
        % Subdivide and recurse
        I = AdaptiveQuad(f, a, m, tol/2, method) + ...
            AdaptiveQuad(f, m, b, tol/2, method);
    end
end