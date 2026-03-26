%% test_vocoder_selectivity.m
% Prueba de viabilidad de un vocoder de canales
% Idea:
%   - el modulador activa una sola banda cada vez
%   - la portadora es ruido blanco
%   - se mide en qué bandas aparece energía a la salida
%
% Si el vocoder funciona, la matriz M debe ser aproximadamente diagonal.

%clear; close all; clc;

%% Parámetros generales
Fs   = 48000;
T    = 2.5;
t    = (0:1/Fs:T-1/Fs).';
Ns   = numel(t);

K    = 18;         % pocas bandas para depurar
fmin = 200;
fmax = 4000;

thr_dB   = -45;   % umbral de apertura
Gmin_dB  = -60;   % cierre máximo
Slope    = 2.0;   % pendiente de apertura
tauEnv   = 0.003; % detector (s)
tauA     = 0.003; % attack (s)
tauR     = 0.040; % release (s)

rng(0);

%% Frecuencias centrales logarítmicas
fc = logspace(log10(fmin), log10(fmax), K).';

% Bordes entre bandas por medias geométricas
mid = sqrt(fc(1:end-1).*fc(2:end));

flow  = zeros(K,1);
fhigh = zeros(K,1);

flow(1)  = max(40, fc(1)^2 / mid(1));
fhigh(1) = mid(1);

for k = 2:K-1
    flow(k)  = mid(k-1);
    fhigh(k) = mid(k);
end

flow(K)  = mid(end);
fhigh(K) = min(0.95*Fs/2, fc(end)^2 / mid(end));

%% Banco de filtros
bpOrder = 2;   % orden moderado para depuración

B = cell(K,1);
A = cell(K,1);

for k = 1:K
    wn = [flow(k) fhigh(k)]/(Fs/2);
    [B{k}, A{k}] = butter(bpOrder, wn, 'bandpass');
end

%% Portadora de prueba
carrier = randn(Ns,1);

%% Ventana activa para medir
idxActive = round(0.8*Fs) : round(1.6*Fs);

%% Matriz de selectividad
% M(i,j) = energía en banda de salida j cuando se excita la banda i
M = zeros(K,K);

for i = 1:K

    % Modulador de prueba: senoide en la fc(i), con envolvente por tramos
    envTest = zeros(Ns,1);
    envTest(t >= 0.4 & t < 1.8) = 0.8;
    m = envTest .* sin(2*pi*fc(i)*t);

    % Vocoder multibanda simple
    yBands = zeros(Ns,K);

    for k = 1:K

        % Análisis modulador
        mk = filter(B{k}, A{k}, m);

        % ENV en dB
        env_dB = local_env_db(mk, Fs, tauEnv, -100);

        % MAP simple: abrir cuando supera el umbral
        gTarget_dB = local_clip(Slope*(env_dB - thr_dB), Gmin_dB, 0);

        % ARF
        g_dB = local_attack_release(gTarget_dB, Fs, tauA, tauR);

        % Síntesis: banda k de la portadora
        ck = filter(B{k}, A{k}, carrier);

        % VCA
        yBands(:,k) = ck .* 10.^(g_dB/20);
    end

    % Suma de subbandas
    y = sum(yBands, 2);

    % Medir energía de salida por bandas
    for j = 1:K
        yj = filter(B{j}, A{j}, y);
        M(i,j) = sqrt(mean(yj(idxActive).^2));
    end
end

%% Normalización por fila
Mn = M ./ max(M, [], 2);

%% Representación
figure('Color','w','Name','Selectividad del vocoder');
imagesc(1:K, 1:K, Mn);
axis xy;
colorbar;
xlabel('Banda de salida j');
ylabel('Banda excitada i');
title('Matriz normalizada de selectividad');

xticks(1:K); yticks(1:K);

figure('Color','w','Name','Frecuencias centrales');
stem(1:K, fc, 'filled');
grid on;
xlabel('Canal k');
ylabel('f_c (Hz)');
title('Frecuencias centrales');

%% Métrica simple: relación diagonal / fuera de diagonal
diagMean = mean(diag(Mn));

offVals = Mn(~eye(K));
offMean = mean(offVals);

fprintf('\n===== TEST DE SELECTIVIDAD DEL VOCODER =====\n');
fprintf('Media diagonal      : %.3f\n', diagMean);
fprintf('Media fuera diagonal: %.3f\n', offMean);
fprintf('Contraste diag/off  : %.3f\n', diagMean / max(offMean,1e-12));

if diagMean > 0.75 && offMean < 0.35
    fprintf('Diagnóstico: la selectividad por bandas es buena.\n');
elseif diagMean > 0.55 && offMean < 0.50
    fprintf('Diagnóstico: la selectividad es aceptable, pero mejorable.\n');
else
    fprintf('Diagnóstico: la selectividad es pobre; el efecto tenderá a sonar como modulación global.\n');
end

%% ===== Funciones locales =====

function env_dB = local_env_db(x, Fs, tau, floor_dB)
    a = exp(-1/(Fs*tau));
    env = zeros(size(x));
    for n = 2:numel(x)
        xr = abs(x(n));
        env(n) = a*env(n-1) + (1-a)*xr;
    end
    env_dB = 20*log10(max(env, 10^(floor_dB/20)));
end

function y = local_attack_release(x, Fs, tauA, tauR)
    aA = exp(-1/(Fs*tauA));
    aR = exp(-1/(Fs*tauR));

    y = zeros(size(x));
    y(1) = x(1);

    for n = 2:numel(x)
        if x(n) < y(n-1)
            % baja la ganancia -> ataque
            y(n) = aA*y(n-1) + (1-aA)*x(n);
        else
            % sube la ganancia -> release
            y(n) = aR*y(n-1) + (1-aR)*x(n);
        end
    end
end

function y = local_clip(x, xmin, xmax)
    y = min(max(x, xmin), xmax);
end