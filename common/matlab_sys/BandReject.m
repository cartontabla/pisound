function [b,a] = BandReject(wc,Q)

%Cálculo de los coefifientes de un Filtro Paso-Banda 2º orden
%Parámetos de entrada: 
%wo = frecuencia central (en radianes)
%Q = factor de calidad
%
%Parametros de salida:
%Coeficientes [b,a] de un filtro IIR de segundo orden

% Coeficientes del ecualizador

p = cos(wc);
q = sin(wc);
k = q/Q;

b0 =   2;
b1 =   -4*p;
b2 =   2;
a0 =   2 + k;
a1 =  -4*p;
a2 =   2 - k;

b=[b0, b1, b2];
a=[a0, a1, a2];

%Normalización de coeficientes
b=b./a0;
a=a./a0;

%fvtool(b,a);