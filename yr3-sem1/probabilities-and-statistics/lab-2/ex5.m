simulation_count = 100000;

dice1 = randi([1,6], simulation_count, 1);
dice2 = randi([1,6], simulation_count, 1);
dice3 = randi([1,6], simulation_count, 1);

prod2 = dice1 .* dice2;
prod3 = dice1 .* dice2 .* dice3;

% We divide the product by 10 to the power of the floor of log 10 of the product, so get the first digit
first_digit_2 = floor(prod2 ./ 10.^floor(log10(prod2)));
first_digit_3 = floor(prod3 ./ 10.^floor(log10(prod3)));

prob_first1_2 = sum(first_digit_2 == 1) / simulation_count;
prob_first1_3 = sum(first_digit_3 == 1) / simulation_count;

fprintf('Probability (2 dice) ~ %.3f\n', prob_first1_2);
fprintf('Probability (3 dice) ~ %.3f\n', prob_first1_3);
