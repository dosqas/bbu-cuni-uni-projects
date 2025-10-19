simulation_count = 100000;
perm_size = 5;


count_a = 0;  % Counter for case (a)

for i = 1:simulation_count
    p = randperm(perm_size);
    idx1 = find(p == 1);  % Position of 1
    if idx1 < perm_size && p(idx1 + 1) == 2
        count_a = count_a + 1;
    end
end

prob_a = count_a / simulation_count;
fprintf('P(2 right after 1) ~ %.3f\n', prob_a);


count_b = 0;  % Counter for case (b)

for i = 1:simulation_count
    p = randperm(perm_size);
    idx1 = find(p == 1);
    idx2 = find(p == 2);
    if abs(idx1 - idx2) == 1
        count_b = count_b + 1;
    end
end

prob_b = count_b / simulation_count;
fprintf('P(1 and 2 next to each other) ~ %.3f\n', prob_b);

