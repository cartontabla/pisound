% dyn_transfer_mosaic.m
% Curvas típicas L_out(L_in) en dB: compresor, limitador, expansor y puerta

clear; close all; clc;

Lin = linspace(-80, 10, 2000);   % dB

% Parámetros (ajusta a tu gusto)
Tcomp = -18;  Rcomp = 4;     % compresor: umbral y ratio
Tlim  = -1;                 % limitador: techo
Texp  = -30; Rexp  = 3;     % expansor descendente: umbral y "ratio" (pendiente) > 1
Tgate = -40; Floor = -80;   % puerta idealizada: umbral y suelo (L_out constante)

% --- Curvas ---
% Compresor (hard knee): pendiente 1/R por encima de T
Lout_comp = Lin;
idx = Lin > Tcomp;
Lout_comp(idx) = Tcomp + (Lin(idx) - Tcomp)/Rcomp;

% Limitador (techo): clamp a T por encima
Lout_lim = min(Lin, Tlim);

% Expansor descendente (hard knee): pendiente R por debajo de T (R>1)
Lout_exp = Lin;
idx = Lin < Texp;
Lout_exp(idx) = Texp + Rexp*(Lin(idx) - Texp);

% Puerta idealizada: por debajo del umbral, salida al suelo
Lout_gate = Lin;
idx = Lin < Tgate;
Lout_gate(idx) = Floor;

% --- Mosaico 2x2 ---
tiledlayout(2,2,'TileSpacing','compact','Padding','compact');

% Compresor
nexttile;
plot(Lin, Lout_comp, 'LineWidth', 1.5); hold on;
plot(Lin, Lin, '--', 'LineWidth', 1.0);
grid on; xlabel('L_{in} (dB)'); ylabel('L_{out} (dB)');
title('Compresor (hard knee)');
legend('L_{out}(L_{in})','Identidad','Location','northwest');

% Limitador
nexttile;
plot(Lin, Lout_lim, 'LineWidth', 1.5); hold on;
plot(Lin, Lin, '--', 'LineWidth', 1.0);
grid on; xlabel('L_{in} (dB)'); ylabel('L_{out} (dB)');
title('Limitador (techo)');
legend('L_{out}(L_{in})','Identidad','Location','northwest');

% Expansor
nexttile;
plot(Lin, Lout_exp, 'LineWidth', 1.5); hold on;
plot(Lin, Lin, '--', 'LineWidth', 1.0);
grid on; xlabel('L_{in} (dB)'); ylabel('L_{out} (dB)');
title('Expansor descendente');
legend('L_{out}(L_{in})','Identidad','Location','northwest');

% Puerta
nexttile;
plot(Lin, Lout_gate, 'LineWidth', 1.5); hold on;
plot(Lin, Lin, '--', 'LineWidth', 1.0);
grid on; xlabel('L_{in} (dB)'); ylabel('L_{out} (dB)');
title('Puerta (idealizada)');
legend('L_{out}(L_{in})','Identidad','Location','northwest');