function [b,a] = HighShelving(wc,G)

%Cálculo de los coefifientes de un Filtro Paso-Banda 2º orden
%Parámetos de entrada: 
%wo = frecuencia central (en radianes)
%Q = factor de calidad
%
%Parametros de salida:
%Coeficientes [b,a] de un filtro IIR de segundo orden

A=10^(G/40);
sqrt2 = sqrt(2);
sqrtA = sqrt(A);
p = cos(wc);
q = sin(wc);

% Coeficientes del ecualizador
b0 =   (1+1/A) + (1-1/A)*p + sqrt2*q/sqrtA;
b1 =   -2*((1-1/A) + (1+1/A)*p);
b2 =   (1+1/A) + (1-1/A)*p - sqrt2*q/sqrtA;
a0 =   (1+A) + (1-A)*p + sqrt2*sqrtA*q;
a1 =   -2*((1-A) + (1+A)*p);
a2 =   (1+A) + (1-A)*p - sqrt2*sqrtA*q;

b=[b0, b1, b2];
b=b* A^2;
a=[a0, a1, a2];

%Normalización de coeficientes
b=b./a0;
a=a./a0;

%fvtool(b,a);