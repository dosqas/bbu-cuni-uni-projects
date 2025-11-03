% 2. One year, 1000 high-school graduates take an exam for admission at the Faculty of Mathematics and Computer Science. Seven hundred of them take the exam in CS, the rest in Math. What is the probability that in 200 randomly selected students,

N = 1000;
n1 = 700;
k = 200;


% a) 50 take the exam in Math;
x_a = 150;
P_a = hygepdf(x_a, N, n1, k);

% b) at least 150 take it in CS.
% >= 150 ==> < 150 ==> <= 149
x_b = 149;
P_b = 1 - hygecdf(x_b, N, n1, k);


fprintf("(a) P(50 take Math) = %.4f\n", P_a);
fprintf("(b) P(at least 150 take CS) = %.4f\n", P_b);
