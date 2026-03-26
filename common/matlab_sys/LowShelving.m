function [b,a] = LowShelving(wc, G)

%Cálculo de los coefifientes de un Filtro Paso-Banda 2º orden
%Parámetos de entrada: 
%wo = frecuencia central (en radianes)
%Q = factor de calidad
%
%Parametros de salida:
%Coeficientes [b,a] de un filtro IIR de segundo orden

A=10^(G/40);
sqrtA = sqrt(A);
sqrt2 = sqrt(2);
p = cos(wc);
q = sin(wc);

% Coeficientes del ecualizador
b0 =   (1+A)+ (1-A)*p+ sqrt2*sqrtA*q;
b1 =   -2*((1-A)+ (1+A)*p);
b2 =   (1+A)+ (1-A)*p- sqrt2*sqrtA*q;
a0 =   (1+1/A)+ (1-1/A)*p + sqrt2*q/sqrtA;
a1 =   -2*((1-1/A)+ (1+1/A)*p);
a2 =   (1+1/A)+ (1-1/A)*p - sqrt2*q/sqrtA;

b=[b0, b1, b2];
a=[a0, a1, a2];

%Normalización de coeficientes
b=b./a0;
a=a./a0;

%fvtool(b,a);