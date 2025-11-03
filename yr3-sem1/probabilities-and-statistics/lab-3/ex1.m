% 1. A student takes a multiple-choice test consisting of 20 questions. There are 4 answers to choose from for each question and only one is correct. If he randomly guesses every answer, find the probability that

n = 20;
p = 1/4;

% a) he gets exactly 8 correct answers;
P_a = binopdf(8, n, p);


% b) he gets at least 13 questions wrong;
% >= 13 wrong ==> <= 7 correct
P_b = binocdf(7, n, p);


% c) he passes the exam (gets 50% correct answers);
% ==>  >= 10 ==> < 9
P_c = 1 - binocdf(9, n, p);


% d) he gets more than 7 right answers;
% > 7 ==> 1 - (<= 7)
P_d = 1 - binocdf(7, n, p);


% e) he passes the exam, but does not get a maximum score;
% ==> >= 10 && < 20
P_e = (1 - binocdf(9, n, p)) - binopdf(20, n, p);


% f) he passes the exam, given that he answered at least 3 questions correctly.
% P(X >= 3 | X >= 10) = P(X >= 3 AND X >= 10) / P(X >= 3) = P(X >= 10) / P(X >= 3)
P_Xge3 = 1 - binocdf(2, n, p);
P_f = P_c / P_Xge3;


fprintf("(a) P(exactly 8 correct) = %.4f\n", P_a);
fprintf("(b) P(at least 13 wrong) = %.4f\n", P_b);
fprintf("(c) P(pass, >=10 correct) = %.4f\n", P_c);
fprintf("(d) P(more than 7 right) = %.4f\n", P_d);
fprintf("(e) P(pass but not max) = %.4f\n", P_e);
fprintf("(f) P(pass | >=3 correct) = %.4f\n", P_f);
