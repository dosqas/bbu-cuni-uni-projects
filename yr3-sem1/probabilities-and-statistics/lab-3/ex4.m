% Network breakdowns are unexpected rare events that occur every 3 weeks, on the average. Compute the probability of

rate_per_week = 1/3;

% a) exactly 10 network breakdowns occurring in 50 weeks;
T_a = 50;
lambda_a = rate_per_week * T_a;
k_a = 10;

P_a = poisspdf(k_a, lambda_a);

% b) more than 4 breakdowns occurring during a 21-week period.
T_b = 21;
lambda_b = rate_per_week * T_b;

% P(X > 4) = 1 - P(X <= 4)
P_b = 1 - poisscdf(4, lambda_b);


fprintf("Poisson distribution - Network breakdowns\n");
fprintf("(a) P(exactly 10 breakdowns in 50 weeks) = %.4f\n", P_a);
fprintf("(b) P(more than 4 breakdowns in 21 weeks) = %.4f\n", P_b);
