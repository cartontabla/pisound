% lut_ws_shapes_demo.m
% Genera 6 curvas típicas para LUT-WS en una figura 2x3:
% Hard Clip, Soft Clip, tanh, arctan, Chebyshev, Asimétrica

clear; close all; clc;

% Dominio de entrada (amplitud normalizada)
x = linspace(-1, 1, 2001).';

% Parámetros (ajusta a gusto)
drive_dB = 12;                 % "amount" típico
drive    = 10^(drive_dB/20);   % lineal
bias     = 0.20;               % asimetría (offset) típico
a3 = 0.8;                      % Chebyshev T3
a5 = 0.2;                      % Chebyshev T5

% Helpers
clamp = @(u) max(-1, min(1, u));

% 1) Hard clip (simétrico)
u1 = clamp(drive*x);
y_hard = clamp(u1);

% 2) Soft clip polinómico (3er orden) en [-1,1]
u2 = clamp(drive*x);
y_soft = u2 - (u2.^3)/3;

% 3) tanh (normalizada)
u3 = clamp(drive*x);
y_tanh = tanh(2*u3)/tanh(2);

% 4) arctan (normalizada)
u4 = clamp(drive*x);
k  = 3; % pendiente/curvatura (ajusta)
y_atan = atan(k*u4)/atan(k);

% 5) Chebyshev (T1 + a3*T3 + a5*T5), re-limitado
u5 = clamp(drive*x);
T1 = u5;
T3 = 4*u5.^3 - 3*u5;
T5 = 16*u5.^5 - 20*u5.^3 + 5*u5;
y_cheb = clamp(T1 + a3*T3 + a5*T5);

% 6) Asimétrica (bias + hard clip) con "corrección" de bias al final
u6 = clamp(drive*x + bias);
y_asym = clamp(u6) - bias;
y_asym = clamp(y_asym);

% ---- Plot 2x3 ----
figure('Name','Curvas típicas LUT-WS','Color','w', 'InvertHardcopy','off');

plots = {
  y_hard, 'Hard Clip';
  y_soft, 'Soft Clip';
  y_tanh, 'tanh';
  y_atan, 'Arctan';
  y_cheb, 'Chebyshev';
  y_asym, 'Asimétrica'
};

for k = 1:6
  subplot(2,3,k);
  plot(x, plots{k,1}, 'LineWidth', 2); grid on;
  set(gca, ...
    'Color','w', ...            % fondo blanco
    'XColor','k', ...           % ejes negros
    'YColor','k', ...
    'GridColor',[0.6 0.6 0.6],... % grid gris
    'GridAlpha',0.6);           % intensidad grid

  grid on
  box on
  axis([-1 1 -1.1 1.1]); axis square;
  xlabel('x'); ylabel('f(x)');
  title(plots{k,2});
end