% 1. The average height of NBA professional basketball players is around 1.98 m, and the standard deviation is 8.8 cm. Assuming a Normal distribution of heights within this group,

mu = 1.98;
sigma = 0.088;


% a) what percent of professional basketball players are taller than 2.1 m?
% P(X > 2.1) = P(X <= 2.1)
p_gt_2_1 = 1 - normcdf(2.1, mu, sigma);


% b) what is the probability that the height of a player is exactly 2.1 m?
% P(X = 2.1) = 0 since the p% of getting 1 particular number is infinitely small
p_eq_2_1 = 0;


% c) what proportion of professional basketball players are at least 2.1 m tall?
p_atleast_2_1 = p_gt_2_1;


% d) what percentage of basketball players have heights between 1.9 and 2.2 m?
p_between = normcdf(2.2, mu, sigma) - normcdf(1.9, mu, sigma);


% e) estimate by simulations the probabilities above.
N = 2e6;

function z = boxmuller(N)
    npairs = ceil(N/2);
    U = rand(npairs,1);
    V = rand(npairs,1);

    R = sqrt(-2 .* log(U));
    Theta = 2 .* pi .* V;

    Z1 = R .* cos(Theta);
    Z2 = R .* sin(Theta);

    zpair = [Z1; Z2];
    z = zpair(1:N);
end

Z = boxmuller(N);
X = mu + sigma .* Z;

p_gt_2_1_sim = mean(X > 2.1);
p_eq_2_1_sim = mean(abs(X - 2.1) < 1e-6);
p_atleast_2_1_sim = mean(X >= 2.1);
p_between_sim = mean((X >= 1.9) & (X <= 2.2));


% f) if your favorite player is within the shortest 20% of all players, what can his height be?
% inv = inverse CDF
q20 = norminv(0.20, mu, sigma);


% g) if your favorite player is within the tallest 15% of all players, what can his height be?
q85 = norminv(0.85, mu, sigma);


fprintf('Analytical Results:\n');
fprintf('a) P(X > 2.1)              = %.4f (%.2f%%)\n', p_gt_2_1, p_gt_2_1*100);
fprintf('b) P(X = 2.1)              = %.4f (%.2f%%)\n', p_eq_2_1, p_eq_2_1*100);
fprintf('c) P(X >= 2.1)             = %.4f (%.2f%%)\n', p_atleast_2_1, p_atleast_2_1*100);
fprintf('d) P(1.9 <= X <= 2.2)      = %.4f (%.2f%%)\n', p_between, p_between*100);
fprintf('f) Shortest 20%% threshold  = %.4fm\n', q20);
fprintf('g) Tallest 15%% threshold   = %.4fm\n\n', q85);

fprintf('Simulation Results (N = %.0f):\n', N);
fprintf('a) P(X > 2.1)              ≈ %.4f (%.2f%%)\n', p_gt_2_1_sim, p_gt_2_1_sim*100);
fprintf('b) P(X = 2.1)              ≈ %.4f (%.2f%%)\n', p_eq_2_1_sim, p_eq_2_1_sim*100);
fprintf('c) P(X >= 2.1)             ≈ %.4f (%.2f%%)\n', p_atleast_2_1_sim, p_atleast_2_1_sim*100);
fprintf('d) P(1.9 <= X <= 2.2)      ≈ %.4f (%.2f%%)\n', p_between_sim, p_between_sim*100);
