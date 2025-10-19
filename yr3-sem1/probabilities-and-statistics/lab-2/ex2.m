simulation_count = 100000;
rolls = randi([1,6], simulation_count, 25);

num_sixes = sum(rolls == 6, 2); % sum across rows

p_at_least_10 = sum(num_sixes >= 10) / simulation_count;
fprintf('Probability of at least 10 sixes = %.3f\n', p_at_least_10);
