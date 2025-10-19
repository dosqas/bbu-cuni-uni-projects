simulation_count = 1000000;
tosses = randi([0,1], simulation_count, 3);

num_heads = sum(tosses, 2); % sum across rows

p_0 = sum(num_heads == 0) / simulation_count;
p_1 = sum(num_heads == 1) / simulation_count;
p_2 = sum(num_heads == 2) / simulation_count;

fprintf('P(exactly 0 heads) = %.3f\n', p_0);
fprintf('P(exactly 1 head) = %.3f\n', p_1);
fprintf('P(exactly 2 heads) = %.3f\n', p_2);
