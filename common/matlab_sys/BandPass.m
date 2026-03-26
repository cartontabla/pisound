function [b,a] = BandPass(wc, Q)

%Cálculo de los coefifientes de un Filtro Paso-Banda 2º orden
%Parámetos de entrada: 
%wo = frecuencia central (en radianes)
%Q = factor de calidad
%
%Parametros de salida:
%Coeficientes [b,a] de un filtro IIR de segundo orden

p = cos(wc);
q = sin(wc);

k  = q/ Q;          % <-- Q aquí

% Coeficientes del ecualizador

b0 = k;
b1 = 0;
b2 = -k;
a0 = 2 + k;
a1 = -4*p;
a2 = 2 - k;

b=[b0, b1, b2];
a=[a0, a1, a2];

%Normalización de coeficientes
b=b./a0;
a=a./a0;

%fvtool(b,a);