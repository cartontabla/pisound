%% test_vocoder_control.m
% Verificación básica de la rama de control de un vocoder
% ENV -> GC -> ARF
%
% Objetivo:
% 1) comprobar seguimiento de envolvente
% 2) medir ataque y release efectivos
% 3) detectar bombeo / rizado excesivo

%clear; close all; clc;

%% Parámetros
Fs = 48000;
T  = 6.0;
N  = round(T*Fs);
t  = (0:N-1)'/Fs;

% Señal moduladora de prueba: senoide con escalones de amplitud
f0 = 220;
A  = zeros(N,1);

A(t < 1.0)                    = 0.00;
A(t >= 1.0 & t < 2.0)         = 0.10;
A(t >= 2.0 & t < 3.0)         = 0.50;
A(t >= 3.0 & t < 4.0)         = 0.15;
A(t >= 4.0 & t < 5.0)         = 0.80;
A(t >= 5.0)                   = 0.05;

z = A .* sin(2*pi*f0*t);

%% Parámetros de la rama de control
% ENV
env_floor_dB = -120;   % evita -Inf
env_tau_ms   = 2.0;    % suavizado RMS/envelope aproximado

% GC (ley simple de ejemplo)
threshold_dB = -45;
ratio_slope  = -4;     % como en tu planteamiento
makeup_dB    = 0;
knee_dB      = 6;      %#ok<NASGU> % aquí no se usa explícitamente

% ARF
attack_ms  = 2.0;
release_ms = 60.0;

%% 1) ENV: detector de envolvente en dB
% detector simple por valor absoluto + LP de 1er orden
tau_env = env_tau_ms*1e-3;
a_env   = exp(-1/(Fs*tau_env));

env_lin = zeros(N,1);
for n = 2:N
    xabs = abs(z(n));
    env_lin(n) = a_env*env_lin(n-1) + (1-a_env)*xabs;
end

env_dB = 20*log10(max(env_lin, 10^(env_floor_dB/20)));

%% 2) GC: ley de control simple en dB
% Aquí puedes sustituir esto por la ley exacta de tu bloque GC.
% La idea es producir una ganancia objetivo g_target_dB a partir de env_dB.

g_target_dB = zeros(N,1);

for n = 1:N
    e = env_dB(n);

    if e <= threshold_dB
        % por debajo del umbral: sin reducción
        g_target_dB(n) = makeup_dB;
    else
        % ley simple compatible con "pendiente" negativa tipo compresiva
        % reducción proporcional al exceso sobre threshold
        g_target_dB(n) = makeup_dB + (ratio_slope + 1)*(e - threshold_dB);
    end
end

% Limita ganancia para evitar valores absurdos en la prueba
g_target_dB = min(0, max(-60, env_dB - threshold_dB - 20));

%% 3) ARF: suavizado con ataque / release independientes
tauA = attack_ms*1e-3;
tauR = release_ms*1e-3;

aA = exp(-1/(Fs*tauA));
aR = exp(-1/(Fs*tauR));

g_dB = zeros(N,1);
g_dB(1) = g_target_dB(1);

for n = 2:N
    if g_target_dB(n) > g_dB(n-1)
        % subida
        g_dB(n) = aA*g_dB(n-1) + (1-aA)*g_target_dB(n);
    else
        % bajada
        g_dB(n) = aR*g_dB(n-1) + (1-aR)*g_target_dB(n);
    end
end

g_lin = 10.^(g_dB/20);

%% Señal "portadora" de prueba y salida controlada
x = sawtooth(2*pi*110*t, 0.5);   % portadora rica
y = g_lin .* x;

%% 4) Medida de ataque y release efectivos
% tiempos 10%-90% en la ganancia para dos transiciones conocidas

% Utilidad local
measure_transition = @(sig, idx1, idx2, rising) local_measure_transition(sig, idx1, idx2, Fs, rising);

% transición de subida cerca de t=2 s
idx1_up = round(1.95*Fs);
idx2_up = round(2.20*Fs);
attack_eff_ms = measure_transition(g_dB, idx1_up, idx2_up, true);

% transición de bajada cerca de t=3 s
idx1_dn = round(2.95*Fs);
idx2_dn = round(3.40*Fs);
release_eff_ms = measure_transition(g_dB, idx1_dn, idx2_dn, false);

%% 5) Medida simple de bombeo / rizado
% Se mide la desviación típica de g_dB en tramos donde debería estar casi estable.
stable_windows = [
    round(1.40*Fs) round(1.80*Fs)
    round(2.40*Fs) round(2.80*Fs)
    round(4.40*Fs) round(4.80*Fs)
];

ripple_dB = zeros(size(stable_windows,1),1);
for k = 1:size(stable_windows,1)
    ii = stable_windows(k,1):stable_windows(k,2);
    ripple_dB(k) = std(g_dB(ii));
end

max_ripple_dB = max(ripple_dB);

%% 6) Representación
figure('Name','Verificación rama de control','Color','w');

subplot(4,1,1);
plot(t, z, 'k');
grid on;
ylabel('z(n)');
title('Moduladora de prueba');

subplot(4,1,2);
plot(t, env_dB, 'b', 'LineWidth', 1.2); hold on;
yline(threshold_dB, '--r', 'Threshold');
grid on;
ylabel('ENV (dB)');
title('Envolvente detectada');

subplot(4,1,3);
plot(t, g_target_dB, '--', 'LineWidth', 1.0); hold on;
plot(t, g_dB, 'LineWidth', 1.4);
grid on;
ylabel('Ganancia (dB)');
legend('g target', 'g suavizada', 'Location', 'best');
title('GC + ARF');

subplot(4,1,4);
plot(t, y, 'm');
grid on;
xlabel('Tiempo (s)');
ylabel('y(n)');
title('Salida controlada');

%% 7) Informe
fprintf('\n===== VERIFICACIÓN RAMA DE CONTROL =====\n');
fprintf('Attack nominal   : %.2f ms\n', attack_ms);
fprintf('Attack efectivo  : %.2f ms\n', attack_eff_ms);
fprintf('Release nominal  : %.2f ms\n', release_ms);
fprintf('Release efectivo : %.2f ms\n', release_eff_ms);
fprintf('Ripple máximo en régimen casi estable: %.3f dB\n', max_ripple_dB);

if max_ripple_dB < 0.5
    fprintf('Diagnóstico bombeo: correcto (rizado bajo).\n');
elseif max_ripple_dB < 1.5
    fprintf('Diagnóstico bombeo: aceptable, pero conviene escuchar.\n');
else
    fprintf('Diagnóstico bombeo: posible bombeo excesivo.\n');
end

%% ===== función local =====
function tr_ms = local_measure_transition(sig, idx1, idx2, Fs, rising)
    s = sig(idx1:idx2);
    s0 = s(1);
    s1 = s(end);

    if abs(s1 - s0) < 1e-9
        tr_ms = NaN;
        return;
    end

    if rising
        y10 = s0 + 0.1*(s1-s0);
        y90 = s0 + 0.9*(s1-s0);

        i10 = find(s >= y10, 1, 'first');
        i90 = find(s >= y90, 1, 'first');
    else
        y10 = s0 + 0.9*(s1-s0);
        y90 = s0 + 0.1*(s1-s0);

        i10 = find(s <= y10, 1, 'first');
        i90 = find(s <= y90, 1, 'first');
    end

    if isempty(i10) || isempty(i90)
        tr_ms = NaN;
    else
        tr_ms = 1000*(i90 - i10)/Fs;
    end
end