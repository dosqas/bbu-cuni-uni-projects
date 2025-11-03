% A search engine goes through a list of sites looking for a given key phrase. The probability that any given site contains that phrase is 0.3 and the search terminates as soon as the key phrase is found.

p = 0.3;
max_sites = 5;
Nsim = 200000;


% a) find the probability that the search stops at the 5th site visited;
% stops at 5th site ==> = 4 failures
k_a = 4;
P_a_formula = geopdf(k_a, p);

% b) find the probability that the phrase is found after at most 3 sites;
% ==>  <= 2 failures
k_b = 2;
P_b_formula = geocdf(k_b, p);

% c) estimate by simulations the two probabilities in parts a) and b).
found_at = zeros(Nsim,1);

for i = 1:Nsim
    k = 0;
    while rand >= p
        k = k + 1;
        if k > max_sites
            break
        end
    end
    found_at(i) = k;  % number of failures before success
end

P_a_sim = mean(found_at == 4);     % stops at 5th site
P_b_sim = mean(found_at <= 2);     % found within 3 sites

fprintf("Search engine problem (p = %.2f)\n", p);
fprintf("(a) P(stop at 5th site): formula = %.4f, simulation = %.4f\n", ...
    P_a_formula, P_a_sim);
fprintf("(b) P(found within 3 sites): formula = %.4f, simulation = %.4f\n", ...
    P_b_formula, P_b_sim);
