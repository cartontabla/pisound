function [b,a] = HighPass(wc)

%Cálculo de los coefifientes de un Filtro Paso-Alto de 2º orden
%Parámetos de entrada: 
%wo = frecuencia de corte a -3dB (en radianes)
%Parametros de salida:
%Coeficientes [b,a] de un filtro IIR de segundo orden

p = cos(wc);
q = sin(wc);
sqrt2 = sqrt(2);

% Coeficientes del ecualizador
b0 = (1 + p);
b1 = -2*(1 + p);
b2 = (1 + p);
a0 = 2 + sqrt2*q;
a1 = -4*p;
a2 = 2 - sqrt2*q;
            
b=[b0, b1, b2];
a=[a0, a1, a2];

%Normalización de coeficientes
b=b./a0;
a=a./a0;

%
%fvtool(b,a);