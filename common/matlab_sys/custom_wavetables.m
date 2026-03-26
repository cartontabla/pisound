function WTc = custom_wavetables(L)
% WTc: [L x 5] en [-1,1], un ciclo, periodicidad implícita

L = max(2, double(L));
p = (0:L-1)'/L;          % ciclos [0,1)
s = sin(2*pi*p);

% 1) FM suave (rico pero continuo)
w1 = sin(2*pi*p + 2*sin(2*pi*2*p));

% 2) Sine + distorsión cúbica (armónicos impares)
w2 = s + 0.5*s.^3;

% 3) Soft-square (tanh), menos “dientes” que un umbral duro
w3 = tanh(4*s);

% 4) Saw aditiva con roll-off exponencial (reduce aspereza)
K  = min(64, floor(L/2)-1);
w4 = zeros(L,1);
for k = 1:K
    w4 = w4 + (1/k) * exp(-k/20) * sin(2*pi*k*p);
end

% 5) “Organ” (impares), roll-off 1/k
w5 = zeros(L,1);
for k = 1:2:K
    w5 = w5 + (1/k) * sin(2*pi*k*p);
end

WTc = [norm1(w1), norm1(w2), norm1(w3), norm1(w4), norm1(w5)];
end

function y = norm1(x)
m = max(abs(x));
if m > 0
    y = x/m;
else
    y = x;
end
end
