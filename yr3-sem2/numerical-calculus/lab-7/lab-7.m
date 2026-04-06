%% --- Optional 4: Newton Forward Differences ---
fprintf('--- Optional 4: Forward Differences (sin x) ---\n');

% Data: x is in degrees, y is sin(x)
x_deg = [39, 41, 43, 45, 47, 49, 51];
y_sin = [0.6293204, 0.6560590, 0.6819984, 0.7071068, 0.7313597, 0.7547096, 0.7771460];

% Targets to approximate
targets = [40, 44, 50];

fprintf('Approximations using Forward Differences:\n');
for t = targets
    val = NewtonForward(x_deg, y_sin, t);
    true_val = sin(deg2rad(t));
    fprintf('sin(%d°) approx: %.7f (True: %.7f, Error: %e)\n', ...
            t, val, true_val, abs(val - true_val));
end

%% --- NEWTON FORWARD DIFFERENCE FUNCTION ---

function val = NewtonForward(x_nodes, y_nodes, x)
    n = length(x_nodes);
    h = x_nodes(2) - x_nodes(1); % Step size
    s = (x - x_nodes(1)) / h;
    
    % Construct Forward Difference Table
    D = zeros(n, n);
    D(:, 1) = y_nodes(:);
    for j = 2:n
        for i = 1:(n - j + 1)
            D(i, j) = D(i+1, j-1) - D(i, j-1);
        end
    end
    
    % Apply the Forward Difference Formula
    % L(x) = y0 + s*D[1,2] + s(s-1)/2! * D[1,3] + ...
    val = D(1, 1);
    term = 1;
    for k = 1:n-1
        term = term * (s - k + 1) / k;
        val = val + term * D(1, k+1);
    end
end