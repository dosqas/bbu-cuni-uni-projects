A = [1 0 -2; 2 1 3; 0 1 0];
B = [2 1 1; 1 0 -1; 1 1 0];

# Compute the matrices C = A − B, D = A · B and E = [eij], where eij = aij · bij
C = A - B
D = A * B
E = A .* B

# Compute (efficiently!) the values x = a11b12 + a12b22 + a13b32 and y = a12b31 + a22b32 + a32b33;
x = A(1,:) * B(:,2)
y = A(:,2) * B(3,:)

# Compute the matrices A + AT ,C + CT and E + ET . What do you notice?
F = A + A'

G = C + C'

H = E + E'
