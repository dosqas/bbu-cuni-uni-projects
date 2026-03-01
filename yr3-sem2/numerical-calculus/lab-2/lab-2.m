%% Lab 2: Polinoame ortogonale. Polinoame Taylor. Diferențe finite și divizate

clear all;  % Șterge toate variabilele din workspace
close all;  % Închide toate figurile deschise
clc;        % Șterge fereastra de comandă

%% 1. Primele 4 polinoame Legendre
% Polinoamele Legendre sunt soluții ale ecuației diferențiale Legendre
% și sunt ortogonale pe intervalul [-1,1] cu ponderea 1

figure(1)  % Creează o nouă figură cu identificatorul 1

% linspace(start, end, number_of_points) - creează un vector cu puncte egal spațiate
% Parametri: start = 0 (punctul de început), end = 1 (punctul final), 
%            number_of_points = 1000 (numărul de puncte)
x = linspace(0, 1, 1000);  % Generează 1000 de puncte între 0 și 1

% Definirea polinoamelor Legendre conform formulelor din enunț
% Notație: .^ înseamnă ridicare la putere element cu element (pentru vectori)
l1 = x;                                      % L1(x) = x
l2 = 3/2*x.^2 - 1/2;                         % L2(x) = (3/2)x^2 - 1/2
l3 = 5/2*x.^3 - 3/2*x;                       % L3(x) = (5/2)x^3 - (3/2)x  
l4 = 35/8*x.^4 - 15/4*x.^2 + 3/8;            % L4(x) = (35/8)x^4 - (15/4)x^2 + 3/8

% subplot(m, n, p) - împarte figura într-o matrice m x n de subgrafice
% și activează subgraficul cu poziția p (numărare de la stânga la dreapta, sus în jos)
subplot(2,2,1)  % Împarte figura în 2 rânduri și 2 coloane, activează subgraficul 1
% plot(x, y, 'style', 'LineWidth', width) - reprezintă grafic datele
% Parametri: x = vectorul axei x, y = vectorul axei y, 
%            'b-' = linie albastră continuă, LineWidth = grosimea liniei (2 puncte)
plot(x, l1, 'b-', 'LineWidth', 2)
grid on  % Activează grila în grafic
title('L_1(x) = x')  % Adaugă titlu
xlabel('x')          % Eticheta axei x
ylabel('L_1(x)')     % Eticheta axei y

subplot(2,2,2)
plot(x, l2, 'r-', 'LineWidth', 2)  % 'r-' = linie roșie continuă
grid on
title('L_2(x) = 3/2 * x^2 - 1/2')
xlabel('x')
ylabel('L_2(x)')

subplot(2,2,3)
plot(x, l3, 'g-', 'LineWidth', 2)  % 'g-' = linie verde continuă
grid on
title('L_3(x) = 5/2 * x^3 - 3/2 * x')
xlabel('x')
ylabel('L_3(x)')

subplot(2,2,4)
plot(x, l4, 'm-', 'LineWidth', 2)  % 'm-' = linie magenta continuă
grid on
title('L_4(x) = 35/8 * x^4 - 15/4 * x^2 + 3/8')
xlabel('x')
ylabel('L_4(x)')

% sgtitle('text') - adaugă un titlu general pentru toată figura (super titlu)
sgtitle('Polinoame Legendre pe [0,1]')

%% 2. Polinoame Chebyshev
% Polinoamele Chebyshev de speța I sunt ortogonale pe [-1,1] cu ponderea 1/sqrt(1-x^2)

figure(2)  % Creează o nouă figură pentru polinoamele Chebyshev

% a) Polinoamele T1, T2, T3 definite prin formula trigonometrică
% Formula: Tn(t) = cos(n * arccos(t))

t = linspace(-1, 1, 1000);  % Generează puncte pentru evaluarea polinoamelor

% cos(n * acos(t)) - calculează polinomul Chebyshev folosind definiția trigonometrică
% acos(t) - calculează arccosinus (inversa cosinusului)
T1_trig = cos(1 * acos(t));  % T1(x) = cos(arccos(x)) = x
T2_trig = cos(2 * acos(t));  % T2(x) = cos(2*arccos(x)) = 2x^2 - 1
T3_trig = cos(3 * acos(t));  % T3(x) = cos(3*arccos(x)) = 4x^3 - 3x

subplot(1,2,1)  % 1 rând, 2 coloane, primul subgrafic
% Plot multiple pe același grafic
plot(t, T1_trig, 'b-', t, T2_trig, 'r-', t, T3_trig, 'g-', 'LineWidth', 2)
grid on
% legend('label1', 'label2', ..., 'Location', 'position') - adaugă legendă
% 'Location', 'best' - plasează legenda în cea mai bună poziție automat
legend('T_1', 'T_2', 'T_3', 'Location', 'best')
title('P. Chebyshev (def. trig.)')
xlabel('t')
ylabel('T_n(t)')

% b) Primele n polinoame Chebyshev folosind formula de recurență
% Formula de recurență: T_{n+1}(x) = 2x*T_n(x) - T_{n-1}(x)
% cu T_0(x) = 1 și T_1(x) = x

n = 6;  % Numărul de polinoame de generat
x = linspace(-1, 1, 1000);  % Puncte pentru evaluare

% zeros(rows, columns) - creează o matrice de zero-uri
% Inițializare matrice pentru stocarea polinoamelor
T = zeros(n, length(x));  % Matrice cu n rânduri (câte un polinom pe rând) și coloane = numărul de puncte

% Inițializare cazuri de bază pentru recurență
T(1,:) = ones(1, length(x));  % T0(x) = 1 (primul rând)
T(2,:) = x;                   % T1(x) = x (al doilea rând)

% Recurența pentru generarea polinoamelor
% for k = start:step:end - buclă de la start la end cu pasul step
for k = 2:n-1  % k de la 2 la n-1 (generăm T2 până la T{n})
    % Calculăm T(k+1) folosind T(k) și T(k-1)
    % .* - înmulțire element cu element
    T(k+1,:) = 2*x.*T(k,:) - T(k-1,:);
end

subplot(1,2,2)  % 1 rând, 2 coloane, al doilea subgrafic

% lines(n) - generează n culori distincte pentru plotare
colors = lines(n);

hold on  % Păstrează graficele existente și adaugă peste ele

% Plotăm toate polinoamele generate
for k = 1:n
    % 'Color', colors(k,:) - folosește culoarea k din setul generat
    plot(x, T(k,:), 'Color', colors(k,:), 'LineWidth', 2)
end

hold off  % Oprește modul hold on

grid on

% arrayfun(@(k) sprintf(...), 1:n) - aplică funcția la fiecare element 1:n
% sprintf - formatează string-uri (similar cu printf)
% 'UniformOutput', false - pentru că rezultatul este un cell array de string-uri
legend(arrayfun(@(k) sprintf('T_{%d}', k-1), 1:n, 'UniformOutput', false), ...
       'Location', 'best')
title('1st 6 p. Chebyshev (recurență)')
xlabel('x')
ylabel('T_n(x)')

sgtitle('Polinoame Chebyshev')

%% 3. Polinoame Taylor pentru f(x) = e^x
% Polinomul Taylor de grad n: P_n(x) = sum_{k=0}^n (x-x0)^k/k! * f^(k)(x0)

figure(3)

% Definim intervalul și punctul de dezvoltare
x = linspace(-1, 3, 1000);  % Intervalul pe care plotăm
x0 = 0;                      % Punctul în jurul căruia facem dezvoltarea

% @(x) exp(x) - definește o funcție anonimă (lambda function)
% f = @(argumente) expresie
f = @(x) exp(x);  % Funcția originală f(x) = e^x

% Pentru f(x)=e^x, toate derivatele sunt e^x, deci f^(k)(0) = 1
f_deriv = 1;  % Valoarea derivatelor în punctul x0

hold on

% jet(n) - generează n culori din paleta jet
colors = jet(7);  % 7 culori (pentru n=0..5 și funcția originală)

% Generăm polinoame Taylor de grad 0 până la 5
for n = 0:5  % n = gradul polinomului
    % zeros(size(x)) - creează un vector de zero-uri de aceeași dimensiune ca x
    Pn = zeros(size(x));  % Inițializare polinom
    
    % Suma Taylor
    for k = 0:n
        % factorial(k) - calculează k! (factorial)
        % (x - x0).^k - (x-x0) la puterea k (element cu element)
        Pn = Pn + (x - x0).^k / factorial(k) * f_deriv;
    end
    
    % 'DisplayName', sprintf(...) - numele pentru legendă
    plot(x, Pn, 'Color', colors(n+1,:), 'LineWidth', 1.5, ...
         'DisplayName', sprintf('P_%d(x)', n))
end

% Plot funcția originală pentru comparație
% 'k-' = linie neagră continuă
plot(x, f(x), 'k-', 'LineWidth', 3, 'DisplayName', 'e^x')

hold off
grid on
legend('Location', 'best')
xlabel('x')
ylabel('y')
title('Polinoame Taylor pentru f(x) = e^x, x_0 = 0')

% xlim([min max]) - setează limitele axei x
xlim([-1, 3])
% ylim([min max]) - setează limitele axei y
ylim([-2, 20])

%% 4. Tabelul diferențelor finite
% Diferențe finite: Δf(x) = f(x+h) - f(x)

h = 0.25;  % Pasul pentru diferențe finite
i = 0:6;   % Vector de indici de la 0 la 6
xi = 1 + i * h;  % Punctele: 1, 1.25, 1.5, ..., 2.5

% Definim funcția f(x) = sqrt(5x^2 + 1)
f = @(x) sqrt(5*x.^2 + 1);
yi = f(xi);  % Evaluăm funcția în punctele xi

% Construirea tabelului diferențelor finite
n = length(yi);  % Numărul de puncte
dif_finite = zeros(n, n);  % Matrice pentru tabel (triunghiulară)
dif_finite(:,1) = yi';     % Prima coloană conține valorile funcției

% Calculăm diferențele finite de ordin superior
% for j = 2:n - pentru fiecare coloană (începând cu a doua)
for j = 2:n
    % for i = 1:n-j+1 - pentru fiecare element valid din coloana curentă
    for i = 1:n-j+1
        % Diferența finită de ordin j: Δ^j f(i) = Δ^(j-1) f(i+1) - Δ^(j-1) f(i)
        dif_finite(i,j) = dif_finite(i+1,j-1) - dif_finite(i,j-1);
    end
end

% Afișarea tabelului
% fprintf(format, variables) - scrie formatat în consolă
fprintf('\n=== Problema 4: Tabelul diferențelor finite ===\n');
fprintf('h = %.2f\n', h);  % %.2f - afișează cu 2 zecimale
fprintf('xi\t\tf(xi)\t\t\tΔf\t\t\tΔ^2f\t\t\tΔ^3f\t\t\tΔ^4f\t\t\tΔ^5f\t\t\tΔ^6f\n');  % \t = tab

for i = 1:n
    fprintf('%.2f\t\t%.6f', xi(i), dif_finite(i,1));  % %.6f - afișează cu 6 zecimale
    for j = 2:n-i+1
        fprintf('\t\t%.6f', dif_finite(i,j));
    end
    fprintf('\n');  % Linie nouă
end

%% 5. Tabelul diferențelor divizate
% Diferențe divizate: f[xi, xj] = (f(xj) - f(xi))/(xj - xi)

x = [2, 4, 6, 8];  % Punctele date
f = [4, 8, 14, 16];  % Valorile funcției în puncte
n = length(x);  % Numărul de puncte

% Construirea tabelului diferențelor divizate
dif_div = zeros(n, n);  % Matrice pentru tabel
dif_div(:,1) = f';      % Prima coloană = valorile funcției

% Calculăm diferențele divizate de ordin superior
for j = 2:n
    for i = 1:n-j+1
        % Diferența divizată de ordin j:
        % f[xi, xi+1, ..., xi+j] = (f[xi+1, ..., xi+j] - f[xi, ..., xi+j-1]) / (x(i+j) - x(i))
        dif_div(i,j) = (dif_div(i+1,j-1) - dif_div(i,j-1)) / (x(i+j-1) - x(i));
    end
end

% Afișarea tabelului
fprintf('\n=== Problema 5: Tabelul diferențelor divizate ===\n');
fprintf('xi\t\tf[xi]\t\tf[xi,xi+1]\tf[xi,...,i+2]\tf[xi,...,xi+3]\n');

for i = 1:n
    fprintf('%.0f\t\t%.2f', x(i), dif_div(i,1));  % %.0f - afișează fără zecimale
    for j = 2:n-i+1
        fprintf('\t\t%.4f', dif_div(i,j));  % %.4f - afișează cu 4 zecimale
    end
    fprintf('\n');
end

% Verificare - calculul polinomului de interpolare Newton folosind diferențele divizate
% Forma Newton: P(x) = f[x0] + f[x0,x1](x-x0) + f[x0,x1,x2](x-x0)(x-x1) + ...
fprintf('\nVerificare: Polinomul de interpolare Newton:\n');
fprintf('P(x) = %.2f + %.2f(x-%.0f) + %.2f(x-%.0f)(x-%.0f) + %.2f(x-%.0f)(x-%.0f)(x-%.0f)\n', ...
    dif_div(1,1), dif_div(1,2), x(1), dif_div(1,3), x(1), x(2), ...
    dif_div(1,4), x(1), x(2), x(3));