%% --- Optional 4: The Runge Phenomenon & Chebyshev Nodes ---
fprintf('--- Optional 4: Equidistant vs. Chebyshev 1st & 2nd Kind ---\n');
f4 = @(x) abs(x) + 0.5*x - x.^2;
x_plot = linspace(-1, 1, 1000);
y_true = f4(x_plot);

% Prepare Figure
figure('Name', 'Optional 4: Comparison of Node Types');

% 1. EQUIDISTANT NODES (The Problem)
m_eq = 20;
x_eq = linspace(-1, 1, m_eq);
y_eq = f4(x_eq);
y_interp_eq = LagrangeBarycentric(x_eq, y_eq, x_plot);

subplot(3, 1, 1);
plot(x_plot, y_true, 'k', 'LineWidth', 1.5); hold on;
plot(x_plot, y_interp_eq, 'r--');
title(['Equidistant Nodes (m=', num2str(m_eq), ') - Notice the edge oscillations!']);
legend('f(x)', 'L_m f(x)'); ylim([-1, 1.5]); grid on;

% 2. CHEBYSHEV NODES OF THE 1ST KIND (The Solution)
m_c1 = 100;
k1 = 1:m_c1;
x_c1 = cos((2*k1-1)*pi / (2*m_c1)); % Zeros of T_m(x)
y_c1 = f4(x_c1);
y_interp_c1 = LagrangeBarycentric(x_c1, y_c1, x_plot);

subplot(3, 1, 2);
plot(x_plot, y_true, 'k', 'LineWidth', 1.5); hold on;
plot(x_plot, y_interp_c1, 'b--');
title(['Chebyshev 1st Kind (m=', num2str(m_c1), ') - Stable at high degrees']);
legend('f(x)', 'L_m f(x)'); grid on;

% 3. CHEBYSHEV NODES OF THE 2ND KIND (The Alternative)
m_c2 = 100;
k2 = 0:m_c2-1; 
x_c2 = cos(k2*pi / (m_c2-1)); % Extrema of T_{m-1}(x)
y_c2 = f4(x_c2);
y_interp_c2 = LagrangeBarycentric(x_c2, y_c2, x_plot);

subplot(3, 1, 3);
plot(x_plot, y_true, 'k', 'LineWidth', 1.5); hold on;
plot(x_plot, y_interp_c2, 'g--');
title(['Chebyshev 2nd Kind (m=', num2str(m_c2), ') - Also stable, includes endpoints']);
legend('f(x)', 'L_m f(x)'); grid on;