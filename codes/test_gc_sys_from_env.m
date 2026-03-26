%% test_gc_sys_from_env.m
% Prueba de gc_sys usando como entrada Lin_dB generado por env_mono_sys

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
envObj.ModeDefault = uint8(2);   % 1=peak, 2=RMS exponencial
envObj.TauRMS = 0.005;
envObj.Eps = 1e-12;

nFrames = floor(Ns/N);
Ns2     = nFrames*N;
tt      = t(1:Ns2);

Lin_dB = zeros(Ns2,1);
envLin = zeros(Ns2,1);

reset(envObj);

for k = 1:nFrames
    idx = (k-1)*N + (1:N);
    xin = x(idx);

    [L,e] = envObj(xin,2);
    Lin_dB(idx) = L;
    envLin(idx) = e;
end

%% Objetos GC
gc_neutral = gc_sys;
gc_neutral.N = N;
gc_neutral.Mode = uint8(0);
gc_neutral.Floor_dB = -80;

gc_pass = gc_sys;
gc_pass.N = N;
gc_pass.Mode = uint8(1);
gc_pass.Floor_dB = -80;

gc_comp = gc_sys;
gc_comp.N = N;
gc_comp.Mode = uint8(2);
gc_comp.Floor_dB = -80;

gc_exp = gc_sys;
gc_exp.N = N;
gc_exp.Mode = uint8(2);
gc_exp.Floor_dB = -80;

gc_gate = gc_sys;
gc_gate.N = N;
gc_gate.Mode = uint8(2);
gc_gate.Floor_dB = -80;

%% Parámetros GC
thr_comp    = -35;
ratio_comp  = 4;
knee_comp   = 6;
makeup_comp = 0;

thr_exp    = -40;
ratio_exp  = -4;
knee_exp   = 6;
makeup_exp = 0;

thr_gate    = -45;
ratio_gate  = -Inf;
knee_gate   = 0;
makeup_gate = 0;

%% Procesado frame a frame
GdB_neutral = zeros(Ns2,1);
GdB_pass    = zeros(Ns2,1);
GdB_comp    = zeros(Ns2,1);
GdB_exp     = zeros(Ns2,1);
GdB_gate    = zeros(Ns2,1);

for k = 1:nFrames
    idx = (k-1)*N + (1:N);
    Lframe = Lin_dB(idx);

    GdB_neutral(idx) = gc_neutral(Lframe, thr_exp, ratio_exp, knee_exp, makeup_exp);
    GdB_pass(idx)    = gc_pass(Lframe,    thr_exp, ratio_exp, knee_exp, makeup_exp);
    GdB_comp(idx)    = gc_comp(Lframe,    thr_comp, ratio_comp, knee_comp, makeup_comp);
    GdB_exp(idx)     = gc_exp(Lframe,     thr_exp,  ratio_exp,  knee_exp,  makeup_exp);
    GdB_gate(idx)    = gc_gate(Lframe,    thr_gate, ratio_gate, knee_gate, makeup_gate);
end

%% Conversión a lineal
g_neutral = 10.^(GdB_neutral/20);
g_pass    = 10.^(GdB_pass/20);
g_comp    = 10.^(GdB_comp/20);
g_exp     = 10.^(GdB_exp/20);
g_gate    = 10.^(GdB_gate/20);

%% Figura temporal
figure('Color','w','Name','gc_sys desde ENV');

subplot(5,1,1);
plot(tt, x(1:Ns2), 'k');
grid on;
ylabel('x');
title('Señal de prueba');

subplot(5,1,2);
plot(tt, Lin_dB, 'b', 'LineWidth', 1.1); hold on;
yline(thr_comp, '--r', 'thr comp');
yline(thr_exp,  '--m', 'thr exp');
yline(thr_gate, '--g', 'thr gate');
grid on;
ylabel('Lin\_dB');
title('Salida de ENV');

subplot(5,1,3);
plot(tt, GdB_neutral, 'k', 'LineWidth', 1.0); hold on;
plot(tt, GdB_pass, 'b');
plot(tt, GdB_comp, 'r');
plot(tt, GdB_exp,  'm');
plot(tt, GdB_gate, 'g');
grid on;
ylabel('GdB0');
legend('neutral','pass','comp','exp','gate','Location','best');
title('Salida de GC (dB)');

subplot(5,1,4);
plot(tt, g_neutral, 'k', 'LineWidth', 1.0); hold on;
plot(tt, g_pass, 'b');
plot(tt, g_comp, 'r');
plot(tt, g_exp,  'm');
plot(tt, g_gate, 'g');
grid on;
ylabel('g');
legend('neutral','pass','comp','exp','gate','Location','best');
title('Ganancia lineal equivalente');

subplot(5,1,5);
plot(tt, envLin, 'c', 'LineWidth', 1.0); hold on;
plot(tt, g_pass, 'b');
grid on;
xlabel('Tiempo (s)');
ylabel('nivel/g');
legend('ENV lineal','gain pass','Location','best');
title('ENV lineal frente a ganancia passthrough');

%% Curvas estáticas
Ltest = (-100:0.25:0).';

G_neutral_static = gc_neutral(Ltest, thr_exp, ratio_exp, knee_exp, makeup_exp);
G_pass_static    = gc_pass(Ltest,    thr_exp, ratio_exp, knee_exp, makeup_exp);
G_comp_static    = gc_comp(Ltest,    thr_comp, ratio_comp, knee_comp, makeup_comp);
G_exp_static     = gc_exp(Ltest,     thr_exp,  ratio_exp,  knee_exp,  makeup_exp);
G_gate_static    = gc_gate(Ltest,    thr_gate, ratio_gate, knee_gate, makeup_gate);

figure('Color','w','Name','Curvas estáticas gc\_sys');
plot(Ltest, G_neutral_static, 'k', 'LineWidth', 1.2); hold on;
plot(Ltest, G_pass_static,    'b', 'LineWidth', 1.2);
plot(Ltest, G_comp_static,    'r', 'LineWidth', 1.2);
plot(Ltest, G_exp_static,     'm', 'LineWidth', 1.2);
plot(Ltest, G_gate_static,    'g', 'LineWidth', 1.2);
grid on;
xlabel('Lin\_dB');
ylabel('GdB0');
legend('neutral','pass','comp','exp','gate','Location','best');
title('Curvas estáticas Lin\_dB -> GdB0');

%% Informe
fprintf('\n===== TEST gc_sys DESDE ENV =====\n');
fprintf('Lin_dB observado : [%.2f, %.2f] dB\n', min(Lin_dB), max(Lin_dB));
fprintf('GdB neutral      : [%.2f, %.2f] dB\n', min(GdB_neutral), max(GdB_neutral));
fprintf('GdB pass         : [%.2f, %.2f] dB\n', min(GdB_pass), max(GdB_pass));
fprintf('GdB comp         : [%.2f, %.2f] dB\n', min(GdB_comp), max(GdB_comp));
fprintf('GdB exp          : [%.2f, %.2f] dB\n', min(GdB_exp), max(GdB_exp));
fprintf('GdB gate         : [%.2f, %.2f] dB\n', min(GdB_gate), max(GdB_gate));