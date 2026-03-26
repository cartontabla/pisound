%% test_map_sys.m
%clear; close all; clc;

%% Parámetros generales
Fs = 48000;
N  = 128;
T  = 3.0;
Ns = round(T*Fs);
t  = (0:Ns-1)'/Fs;

%% Señal de prueba
x = zeros(Ns,1);

ii = (t >= 0.5 & t < 1.0);
x(ii) = 0.10*sin(2*pi*220*t(ii));

ii = (t >= 1.0 & t < 1.5);
x(ii) = 0.50*sin(2*pi*220*t(ii));

ii = (t >= 1.5 & t < 2.0);
x(ii) = 0.20*sin(2*pi*220*t(ii));

rng(0);
ii = (t >= 2.0 & t < 2.5);
x(ii) = 0.05*randn(sum(ii),1);

ii = (t >= 2.5);
x(ii) = 0.80*sin(2*pi*440*t(ii));

%% ENV
envObj = env_mono_sys;
envObj.N = N;
envObj.Fs = Fs;
envObj.ModeDefault = uint8(2);
envObj.TauRMS = 0.005;
envObj.Eps = 1e-12;

nFrames = floor(Ns/N);
Ns2     = nFrames*N;
tt      = t(1:Ns2);

Lin_dB = zeros(Ns2,1);

reset(envObj);
for k = 1:nFrames
    idx = (k-1)*N + (1:N);
    xin = x(idx);
    [L,~] = envObj(xin,2);
    Lin_dB(idx) = L;
end

%% GC en modo pass (para alimentar MAP mode 8)
gc_pass = gc_sys;
gc_pass.N = N;
gc_pass.Mode = uint8(1);
gc_pass.Floor_dB = -80;

GdB_pass = zeros(Ns2,1);
for k = 1:nFrames
    idx = (k-1)*N + (1:N);
    Lframe = Lin_dB(idx);
    GdB_pass(idx) = gc_pass(Lframe, -40, -4, 0, 0);
end

%% MAP mode 7: ENV(dB) -> ganancia(dB)
map7 = map_sys;
map7.Mode = uint8(7);
map7.Threshold = -40;
map7.OutMin = -60;
map7.OutMax = 0;
map7.Slope = (map7.OutMax - map7.OutMin) / (0 - map7.Threshold);  % 0 dB -> 0 dB

%% MAP mode 8: reescala ganancia dB ya existente
map8 = map_sys;
map8.Mode = uint8(8);
map8.InMin  = -80;
map8.InMax  = 0;
map8.OutMin = -80;
map8.OutMax = 0;

%% Aplicación temporal
Y7 = zeros(Ns2,1);
Y8 = zeros(Ns2,1);

for k = 1:nFrames
    idx = (k-1)*N + (1:N);

    Y7(idx) = map7(Lin_dB(idx));
    Y8(idx) = map8(GdB_pass(idx));
end

%% Curvas estáticas
Ltest = (-100:0.25:0).';

Y7_static = map7(Ltest);
Y8_static = map8(Ltest);

%% Figuras
figure('Color','w','Name','MAP - temporal');

subplot(3,1,1);
plot(tt, Lin_dB, 'b');
grid on;
ylabel('Lin\_dB');
title('Salida de ENV');

subplot(3,1,2);
plot(tt, Y7, 'r');
grid on;
ylabel('MAP7');
title('MAP mode 7: ENV(dB) -> ganancia(dB)');

subplot(3,1,3);
plot(tt, GdB_pass, 'k'); hold on;
plot(tt, Y8, 'm');
grid on;
ylabel('dB');
xlabel('Tiempo (s)');
legend('GC pass','MAP8(GC pass)','Location','best');
title('MAP mode 8 sobre una ganancia ya existente');

figure('Color','w','Name','MAP - curvas estáticas');

plot(Ltest, Y7_static, 'r', 'LineWidth', 1.2); hold on;
plot(Ltest, Y8_static, 'm', 'LineWidth', 1.2);
grid on;
xlabel('Entrada (dB)');
ylabel('Salida (dB)');
legend('mode 7','mode 8','Location','best');
title('Curvas estáticas de MAP');

%% Informe
fprintf('\n===== TEST MAP =====\n');
fprintf('MAP7: Threshold=%.1f dB, OutMin=%.1f dB, OutMax=%.1f dB, Slope=%.3f\n', ...
    map7.Threshold, map7.OutMin, map7.OutMax, map7.Slope);
fprintf('MAP8: In=[%.1f, %.1f] dB, Out=[%.1f, %.1f] dB\n', ...
    map8.InMin, map8.InMax, map8.OutMin, map8.OutMax);

fprintf('\nComprobaciones clave:\n');
fprintf('MAP7(Threshold) = %.2f dB\n', map7(map7.Threshold));
fprintf('MAP7(0 dB)      = %.2f dB\n', map7(0));
fprintf('MAP8(0 dB)      = %.2f dB\n', map8(0));
fprintf('MAP8(InMin)     = %.2f dB\n', map8(map8.InMin));