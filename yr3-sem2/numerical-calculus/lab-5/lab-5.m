%% Lab Nr. 5: Linear Systems II - Iterative Methods
clear; clc;

% --- Application 1: Comparison of Jacobi and Gauss-Seidel (n=1000) ---
fprintf('--- Application 1: Tridiagonal System (n=1000) ---\n');
n = 1000;
% Constructing the tridiagonal matrix
A1 = diag(5*ones(n,1)) + diag(-1*ones(n-1,1), 1) + diag(-1*ones(n-1,1), -1);
b1 = 3 * ones(n,1); 
b1(1) = 4; b1(n) = 4;
x0 = zeros(n,1);
err = 1e-5;
maxnit = 5000;

[xJ, nitJ] = Jacobi(A1, b1, x0, err, maxnit);
[xGS, nitGS] = GaussSeidel(A1, b1, x0, err, maxnit);

fprintf('Jacobi iterations: %d\n', nitJ);
fprintf('Gauss-Seidel iterations: %d\n\n', nitGS);

% --- Application 2: Wilson’s Example (Conditioning) ---
fprintf('--- Application 2: Wilson Example ---\n');
A2 = [10 7 8 7; 7 5 6 5; 8 6 10 9; 7 5 9 10];
b2 = [32; 23; 33; 31];

% a) Solve original
x_orig = A2 \ b2;
fprintf('a) Original solution x: [%s]\n', sprintf(' %.1f', x_orig));

% b) Perturb b
b_tilde = [32.1; 22.9; 33.1; 30.9];
x_b_perturbed = A2 \ b_tilde;
rel_err_in_b = norm(b2 - b_tilde) / norm(b2);
rel_err_out_b = norm(x_orig - x_b_perturbed) / norm(x_orig);
fprintf('b) Relative input error (b): %e\n', rel_err_in_b);
fprintf('   Relative output error (x): %e\n', rel_err_out_b);

% c) Perturb A
A_tilde = [10 7 8.1 7.2; 7.8 5.04 6 5; 8 5.98 9.89 9; 6.99 4.99 9 9.98];
x_A_perturbed = A_tilde \ b2;
rel_err_in_A = norm(A2 - A_tilde) / norm(A2);
rel_err_out_A = norm(x_orig - x_A_perturbed) / norm(x_orig);
fprintf('c) Relative input error (A): %e\n', rel_err_in_A);
fprintf('   Relative output error (x): %e\n', rel_err_out_A);

% d) Explanation
fprintf('d) Explanation: The Matrix A is ill-conditioned. cond(A) = %.2f.\n', cond(A2, inf));
fprintf('   Small changes in input are amplified by the condition number.\n\n');

% --- Optional: Application 3 (SOR Comparison) ---
fprintf('--- Optional 3: SOR vs Others (n=100) ---\n');
n3 = 100; % Reduced for speed, can be set to 1000
% Matrix: 5 on diag, -1 on offset 1 and offset 3 (as per the pattern)
A3 = 5*eye(n3) - diag(ones(n3-1,1),1) - diag(ones(n3-1,1),-1) ...
              - diag(ones(n3-3,1),3) - diag(ones(n3-3,1),-3);
b3 = 1*ones(n3,1); b3(1)=3; b3(end)=3; b3(2)=2; b3(end-1)=2;

% Optimal omega calculation (approximate for this structure)
rho_jacobi = max(abs(eig(eye(n3) - diag(1./diag(A3))*A3)));
w_opt = 2 / (1 + sqrt(1 - rho_jacobi^2));

[~, nJ] = Jacobi(A3, b3, zeros(n3,1), err, maxnit);
[~, nGS] = GaussSeidel(A3, b3, zeros(n3,1), err, maxnit);
[~, nSOR] = SOR(A3, b3, w_opt, zeros(n3,1), err, maxnit);

fprintf('Method Comparison (n=%d):\n', n3);
fprintf('Jacobi: %d, Gauss-Seidel: %d, SOR (w=%.2f): %d\n', nJ, nGS, w_opt, nSOR);

%% --- FUNCTION DEFINITIONS ---

function [x, nit] = Jacobi(A, b, x0, err, maxnit)
    D = diag(diag(A));
    LU = A - D;
    x = x0;
    for nit = 1:maxnit
        x_new = D \ (b - LU * x);
        if norm(x_new - x, inf) < err
            x = x_new;
            return;
        end
        x = x_new;
    end
end

function [x, nit] = GaussSeidel(A, b, x0, err, maxnit)
    DL = tril(A);
    U = triu(A, 1);
    x = x0;
    for nit = 1:maxnit
        x_new = DL \ (b - U * x);
        if norm(x_new - x, inf) < err
            x = x_new;
            return;
        end
        x = x_new;
    end
end

function [x, nit] = SOR(A, b, omega, x0, err, maxnit)
    D = diag(diag(A));
    L = -tril(A, -1);
    U = -triu(A, 1);
    M = (D - omega * L);
    N = ((1 - omega) * D + omega * U);
    x = x0;
    for nit = 1:maxnit
        x_new = M \ (N * x + omega * b);
        if norm(x_new - x, inf) < err
            x = x_new;
            return;
        end
        x = x_new;
    end
end