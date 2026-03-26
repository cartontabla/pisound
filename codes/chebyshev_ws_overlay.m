% chebyshev_ws_overlay.m
% Superpone T1, T3 y T5 (Chebyshev 1ª especie) y una combinación.
% Leyenda dentro, abajo, con fondo blanco (sin tema oscuro).

clear; close all; clc;

x = linspace(-1,1,2001).';

T1 = x;
T3 = 4*x.^3 - 3*x;
T5 = 16*x.^5 - 20*x.^3 + 5*x;

a3 = 0.8;
a5 = 0.2;
Y  = T1 + a3*T3 + a5*T5;

figure('Color','w','InvertHardcopy','off','Name','Chebyshev superpuestos');
ax = axes(); %#ok<LAXES>
set(ax,'Color','w','XColor','k','YColor','k', ...
    'GridColor',[0.6 0.6 0.6],'GridAlpha',0.6);

plot(x, T1, 'LineWidth', 2); hold on;
plot(x, T3, 'LineWidth', 2);
plot(x, T5, 'LineWidth', 2);
plot(x, Y , '--', 'LineWidth', 2);

grid on; box on; axis square;
axis([-1 1 -1.5 1.5]);
xlabel('x'); ylabel('f(x)');
title(sprintf('Chebyshev: T_1, T_3, T_5 y T_1 + %.2fT_3 + %.2fT_5', a3, a5));

set(gcf,'Color','w','InvertHardcopy','off');
set(gca,'Color','w','XColor','k','YColor','k', ...
        'GridColor',[0.6 0.6 0.6],'GridAlpha',0.6);

lgd = legend('T_1(x)=x', 'T_3(x)=4x^3-3x', 'T_5(x)=16x^5-20x^3+5x', ...
             'Combinación', 'Location','south');  % valor válido

% Sin recuadro ni fondo
lgd.Box = 'off';
lgd.Color = 'none';
lgd.TextColor = 'k';

% Colocar dentro, abajo-derecha (coordenadas normalizadas)
lgd.Units = 'normalized';
lgd.Position = [0.52 0.123 0.35 0.12];   % [x y w h]