% 2. The time it takes a printer to print a job is an Exponential random variable with the expectation of 12 seconds.

mu = 12;
lambda = 1/mu;


% a) what is the probability that a document will take more than 30 seconds to be printed?
p_gt_30 = 1 - expcdf(30, mu);


% b) if you send a job to the printer at 10:00 a.m. and it appears to be third in line, how likely is it that your document will be ready before 10:01?
t = 60;
p_ready_before_60 = gamcdf(t, 3, mu);


%c) estimate by simulations the probabilities above.
N = 2e6;

% Generate exponential random variables: X = -log(U)/lambda
X = -log(rand(N, 1)) / lambda;
p_gt_30_sim = mean(X > 30);

% 3 jobs in queue: sum of 3 exponential times
S3 = -log(rand(N, 3)) / lambda;
S3_sum = sum(S3, 2);
p_ready_before_60_sim = mean(S3_sum <= 60);


fprintf('Analytical results:\n');
fprintf('a) P(X > 30s) = %.4f (%.4f%%)\n', p_gt_30, p_gt_30 * 100);
fprintf('b) P(3 jobs finish within 60s) = %.4f (%.4f%%)\n\n', p_ready_before_60, p_ready_before_60 * 100);

fprintf('Simulation results (N = %.0f):\n', N);
fprintf('a) P(X > 30s) (simulated) = %.4f (%.4f%%)\n', p_gt_30_sim, p_gt_30_sim * 100);
fprintf('b) P(3 jobs finish within 60s) (simulated) = %.4f (%.4f%%)\n', p_ready_before_60_sim, p_ready_before_60_sim * 100);

