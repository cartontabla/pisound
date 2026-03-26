function [b,a] = CenterShelving(wc,G,Q)

%Cálculo de los coefifientes de un Filtro Paso-Banda 2º orden
%Parámetos de entrada: 
%wo = frecuencia central (en radianes)
%Q = factor de calidad
%
%Parametros de salida:
%Coeficientes [b,a] de un filtro IIR de segundo orden

A=10^(G/40);
sqrtA = sqrt(A);
p = cos(wc);
q = sin(wc);
alpha = q/(2*Q);

% Coeficientes del ecualizador
b0 = 1 + alpha*A;
b1 = -2*p;
b2 = 1 - alpha*A;
a0 = 1 + alpha/A;
a1 = -2*p;
a2 = 1 - alpha/A;

b = [b0, b1, b2];
a = [a0, a1, a2];

%Normalización de coeficientes
b = b./ a0;
a = a./ a0;

%fvtool(b,a);