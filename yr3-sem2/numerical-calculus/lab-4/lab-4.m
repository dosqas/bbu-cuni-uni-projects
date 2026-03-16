%% LAB 4: LINEAR SYSTEMS I - DIRECT METHODS
clear; clc;

%% APPLICATION 1: 4x4 System
A1 = [ 2,  1, -1, -2;
       4,  4,  1,  3;
      -6, -1, 10, 10;
      -2,  1,  8,  4];
b1 = [2; 4; -5; 1];

fprintf('--- Application 1 ---\n');

% Gaussian Elimination with Partial Pivoting
x_ge = gauss_pivoting(A1, b1);
fprintf('Gaussian Elimination Solution: \n'); disp(x_ge');

% LUP Factorization Solution
x_lup = solve_lup(A1, b1);
fprintf('LUP Factorization Solution: \n'); disp(x_lup');

% Cholesky (Note: Only works if A is Symmetric Positive Definite)
if isequal(A1, A1') && all(eig(A1) > 0)
    x_chol = solve_cholesky(A1, b1);
    fprintf('Cholesky Solution: \n'); disp(x_chol');
else
    fprintf('Cholesky: Skipped (Matrix is not SPD)\n');
end

% QR Factorization Solution
x_qr = solve_qr(A1, b1);
fprintf('QR Factorization Solution: \n'); disp(x_qr');


%% APPLICATION 2: Tri-diagonal General System (n=10 example)
n = 10;
A2 = diag(5*ones(n,1)) + diag(-1*ones(n-1,1), 1) + diag(-1*ones(n-1,1), -1);
b2 = 3*ones(n,1); b2(1) = 4; b2(n) = 4;

fprintf('--- Application 2 (n=%d) ---\n', n);
x_gen = solve_lup(A2, b2);
fprintf('Solution for General System: \n'); disp(x_gen');


%% --- CORE FUNCTIONS ---

% 1. Back Substitution (Upper Triangular: Ux = b)
function x = back_subst(U, b)
    n = length(b);
    x = zeros(n, 1);
    for i = n:-1:1
        x(i) = (b(i) - U(i, i+1:n) * x(i+1:n)) / U(i,i);
    end
end

% 1. Forward Substitution (Lower Triangular: Ly = b)
function x = forward_subst(L, b)
    n = length(b);
    x = zeros(n, 1);
    for i = 1:n
        x(i) = (b(i) - L(i, 1:i-1) * x(1:i-1)) / L(i,i);
    end
end

% 2. Gaussian Elimination with Partial Pivoting
function x = gauss_pivoting(A, b)
    n = length(b);
    Ab = [A, b]; % Augmented matrix
    for i = 1:n-1
        [~, p] = max(abs(Ab(i:n, i))); % Find pivot
        p = p + i - 1;
        Ab([i, p], :) = Ab([p, i], :); % Swap rows
        for j = i+1:n
            m = Ab(j,i) / Ab(i,i);
            Ab(j, i:end) = Ab(j, i:end) - m * Ab(i, i:end);
        end
    end
    x = back_subst(Ab(:, 1:n), Ab(:, n+1));
end

% 3. LUP Solver
function x = solve_lup(A, b)
    [L, U, P] = lu(A); % MATLAB built-in LUP
    % PAx = Pb -> LUx = Pb
    L
    U

    y = forward_subst(L, P*b);
    x = back_subst(U, y);
end

% 3. Cholesky Solver
function x = solve_cholesky(A, b)
    R = chol(A); % A = R'R (R is upper triangular)
    y = forward_subst(R', b);
    x = back_subst(R, y);
end

% 3. QR Solver
function x = solve_qr(A, b)
    [Q, R] = qr(A);
    % Rx = Q'b
    x = back_subst(R, Q'*b);
end