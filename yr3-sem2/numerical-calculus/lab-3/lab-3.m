%% Lab 3: Numerical Calculus - Divided and Finite Differences

function T = div_diff(x, y)
    n = length(x);
    T = NaN(n, n);
    T(:,1) = y(:);
    for j = 2:n
        for i = 1:n-j+1
            T(i,j) = (T(i+1,j-1) - T(i,j-1)) / (x(i+j-1) - x(i));
        end
    end
end

function T = div_diff_double(x, y, dy)
    n = length(x);
    z = reshape([x; x], 1, []); % Interleave: [x1, x1, x2, x2...]
    fz = reshape([y; y], 1, []);
    m = 2*n;
    T = NaN(m, m);
    T(:,1) = fz(:);
    
    for j = 2:m
        for i = 1:m-j+1
            if j == 2 && mod(i, 2) ~= 0
                % Use derivative when the two nodes are identical
                T(i,j) = dy((i+1)/2);
            else
                % Standard divided difference formula
                T(i,j) = (T(i+1,j-1) - T(i,j-1)) / (z(i+j-1) - z(i));
            end
        end
    end
end

function T = forward_diff(y)
    n = length(y);
    T = NaN(n, n);
    T(:,1) = y(:);
    for j = 2:n
        T(1:n-j+1, j) = diff(T(1:n-j+2, j-1));
    end
end

function T = backward_diff(y)
    n = length(y);
    T = NaN(n, n);
    T(:,1) = y(:);
    for j = 2:n
        % For backward, results are usually aligned with the "bottom" index
        T(j:n, j) = diff(T(j-1:n, j-1));
    end
end

%% Application 1: f(x) = 1/(1+x)
f = @(x) 1 ./ (1 + x);
df = @(x) -1 ./ (1 + x).^2; % Derivative for double nodes

% a) Simple nodes: 0, 1, 2
x_a = [0, 1, 2];
y_a = f(x_a);
T1a = div_diff(x_a, y_a);
disp('Table 1a: Simple nodes [0, 1, 2]'); disp(T1a);

% b) Double nodes: 0, 1, 2
dy_b = df(x_a);
T1b = div_diff_double(x_a, y_a, dy_b);
disp('Table 1b: Double nodes [0, 1, 2]'); disp(T1b);

% c) 11 equidistant nodes on [1, 2]
x_c = linspace(1, 2, 11);
y_c = f(x_c);
dy_c = df(x_c);
T1c_simple = div_diff(x_c, y_c);
T1c_double = div_diff_double(x_c, y_c, dy_c);
disp('Table 1c (Simple)'); disp(T1c_simple);
disp('Table 1c (Double)'); disp(T1c_double);

%% Application 2: Discrete Data
x2 = -2:4;
f2 = [-5, 1, 1, 1, 7, 25, 60];

% a) Divided Differences
T2a = div_diff(x2, f2);
disp('Table 2a: Divided Differences (Data)'); disp(T2a);

% b) Forward Differences
T2b = forward_diff(f2);
disp('Table 2b: Forward Finite Differences'); disp(T2b);

% c) Backward Differences
T2c = backward_diff(f2);
disp('Table 2c: Backward Finite Differences'); disp(T2c);