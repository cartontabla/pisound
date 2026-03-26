%% Figura didáctica ARF: GdB0(n) vs GdB(n) + zonas Attack/Release
clear; close all; clc

% Parámetros
Fs = 48000;
Ta = 0.010;   % attack (s)
Tr = 0.200;   % release (s)

alphaA = exp(-1/(Ta*Fs));
alphaR = exp(-1/(Tr*Fs));

% Señal de prueba: ganancia instantánea en dB (escalones)
% (0 dB = sin reducción, valores negativos = reducción de ganancia)
dur_s = 1.2;
N = round(dur_s*Fs);
GdB0 = zeros(N,1);

% Segmentos (ajusta libremente)
seg = round([0.10 0.20 0.30 0.45 0.60 0.80 1.00 1.20]*Fs);
GdB0(1:seg(1))        = 0;
GdB0(seg(1)+1:seg(2)) = -12;
GdB0(seg(2)+1:seg(3)) = -3;
GdB0(seg(3)+1:seg(4)) = -18;
GdB0(seg(4)+1:seg(5)) = -6;
GdB0(seg(5)+1:seg(6)) = 0;
GdB0(seg(6)+1:seg(7)) = -9;
GdB0(seg(7)+1:end)    = 0;

% ARF (misma lógica que en arf_sys.m, pero vectorial para la figura)
GdB = zeros(N,1);
attackActive  = false(N,1);
releaseActive = false(N,1);

g = 0; % estado inicial (0 dB)
for n = 1:N
    x = GdB0(n);
    if x < g
        % entrando en compresión (más negativo): ATTACK
        g = alphaA*g + (1-alphaA)*x;
        attackActive(n) = true;
    else
        % saliendo (hacia 0 dB): RELEASE
        g = alphaR*g + (1-alphaR)*x;
        releaseActive(n) = true;
    end
    GdB(n) = g;
end

t = (0:N-1)'/Fs;

% --- Dibujo ---
figure('Color','w');
ax = axes; hold(ax,'on');

% Sombreado por tramos (attack/release)
yl = [min([GdB0;GdB])-2, 2];  % límites verticales
ylim(yl);

% Función auxiliar para sombrear segmentos contiguos
shadeSegments(t, attackActive,  yl, 0.90);  % gris claro: attack
shadeSegments(t, releaseActive, yl, 0.97);  % gris muy claro: release

% Curvas
stairs(t, GdB0, 'LineWidth', 1.2);           % GdB0(n)
plot(t, GdB, 'LineWidth', 1.6);              % GdB(n) suavizada

grid on
xlabel('t (s)');
ylabel('Ganancia (dB)');
title('ARF (Attack/Release) sobre ganancia en dB: G_{dB,0}(n) y G_{dB}(n)');

legend({'Attack activo','Release activo','G_{dB,0}(n) (instantánea)','G_{dB}(n) (suavizada)'}, ...
       'Location','southwest');

% Texto breve en la figura (opcional)
text(0.02, yl(2)-0.6, sprintf('T_A=%.0f ms, T_R=%.0f ms', 1000*Ta, 1000*Tr), ...
     'FontSize', 10, 'BackgroundColor', 'w', 'Margin', 2);

hold off

%% --- Función local: sombrear segmentos contiguos ---
function shadeSegments(t, mask, yl, grayLevel)
% Sombrea intervalos donde mask==true con un rectángulo de fondo.
% grayLevel: 0=negro, 1=blanco.
    if ~any(mask), return; end
    mask = mask(:);
    dt = mean(diff(t));
    d = diff([false; mask; false]);
    starts = find(d==1);
    ends   = find(d==-1)-1;
    for k = 1:numel(starts)
        x1 = t(starts(k));
        x2 = t(ends(k)) + dt;
        patch([x1 x2 x2 x1], [yl(1) yl(1) yl(2) yl(2)], grayLevel*[1 1 1], ...
              'EdgeColor','none', 'HandleVisibility','off');
    end
end