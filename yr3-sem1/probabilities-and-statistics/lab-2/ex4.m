simulation_count = 100000;

count_phones = 0;

for i = 1:simulation_count
    p = randperm(50);
    phones_not_right = all(p ~= 1:50); % compares elements by position; all = true if all elements = 1

    if phones_not_right == 1
        count_phones = count_phones + 1;
    end
end

prob_phones_not_right = count_phones / simulation_count;
fprintf('P(no phone right) ~ %.3f\n', prob_phones_not_right);
