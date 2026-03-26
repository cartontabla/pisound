%% test_env_mono_sys.m
%clear; close all; clc;

%% Parámetros
Fs = 48000;
N  = 128;
T  = 3.0;
Ns = round(T*Fs);
t  = (0:Ns-1)'/Fs;

% Señal de prueba por tramos
x = zeros(Ns,1);

% 0 - 0.5 s: silencio
x(t < 0.5) = 0;

% 0.5 - 1.0 s: seno A=0.1
ii = (t >= 0.5 & t < 1.0);
x(ii) = 0.1*sin(2*pi*220*t(ii));

% 1.0 - 1.5 s: seno A=0.5
ii = (t >= 1.0 & t < 1.5);
x(ii) = 0.5*sin(2*pi*220*t(ii));

% 1.5 - 2.0 s: seno A=0.2
ii = (t >= 1.5 & t < 2.0);
x(ii) = 0.2*sin(2*pi*220*t(ii));

% 2.0 - 2.5 s: ruido bajo
rng(0);
ii = (t >= 2.0 & t < 2.5);
x(ii) = 0.05*randn(sum(ii),1);

% 2.5 - 3.0 s: seno A=0.8
ii = (t >= 2.5);
x(ii) = 0.8*sin(2*pi*440*t(ii));

%% Crear bloques ENV
env1 = env_mono_sys;
env1.N = N;
env1.Fs = Fs;
env1.ModeDefault = uint8(1);
env1.TauRMS = 0.02;

env2 = env_mono_sys;
env2.N = N;
env2.Fs = Fs;
env2.ModeDefault = uint8(2);
env2.TauRMS = 0.02;

env3 = env_mono_sys;
env3.N = N;
env3.Fs = Fs;
env3.ModeDefault = uint8(3);
env3.TauRMS = 0.02;

% Reset explícito
reset(env1); reset(env2); reset(env3);

%% Procesado frame a frame
nFrames = floor(Ns/N);

env_1   = zeros(nFrames*N,1);
Lin_dB1 = zeros(nFrames*N,1);

env_2   = zeros(nFrames*N,1);
Lin_dB2 = zeros(nFrames*N,1);

env_3   = zeros(nFrames*N,1);
Lin_dB3 = zeros(nFrames*N,1);

for k = 1:nFrames
    idx = (k-1)*N + (1:N);
    xin = x(idx);

    [L1,e1] = env1(xin,1);
    [L2,e2] = env2(xin,2);
    [L3,e3] = env3(xin,3);

    Lin_dB1(idx) = L1;
    env_1(idx)   = e1;

    Lin_dB2(idx) = L2;
    env_2(idx)   = e2;

    Lin_dB3(idx) = L3;
    env_3(idx)   = e3;
end

tt = t(1:nFrames*N);

%% Comprobación 1: coherencia env <-> Lin_dB
Lin_dB1_from_env = 20*log10(env_1 + 1e-12);
Lin_dB2_from_env = 20*log10(env_2 + 1e-12);
Lin_dB3_from_env = 20*log10(env_3 + 1e-12);

err1 = max(abs(Lin_dB1 - Lin_dB1_from_env));
err2 = max(abs(Lin_dB2 - Lin_dB2_from_env));
err3 = max(abs(Lin_dB3 - Lin_dB3_from_env));

fprintf('\n=== Coherencia env -> dB ===\n');
fprintf('Modo 1, error máx = %.6f dB\n', err1);
fprintf('Modo 2, error máx = %.6f dB\n', err2);
fprintf('Modo 3, error máx = %.6f dB\n', err3);

%% Comprobación 2: mode 1 y mode 2 casi iguales
diff12_env = max(abs(env_1 - env_2));
diff12_dB  = max(abs(Lin_dB1 - Lin_dB2));

fprintf('\n=== Comparación mode 1 vs mode 2 ===\n');
fprintf('Dif. máx env   = %.6e\n', diff12_env);
fprintf('Dif. máx Lin_dB= %.6e dB\n', diff12_dB);

%% Comprobación 3: RMS esperado para seno
% Para seno de amplitud A, RMS = A/sqrt(2)
A1 = 0.1;  rms1_exp = A1/sqrt(2);  dB1_exp = 20*log10(rms1_exp);
A2 = 0.5;  rms2_exp = A2/sqrt(2);  dB2_exp = 20*log10(rms2_exp);
A3 = 0.2;  rms3_exp = A3/sqrt(2);  dB3_exp = 20*log10(rms3_exp);
A4 = 0.8;  rms4_exp = A4/sqrt(2);  dB4_exp = 20*log10(rms4_exp);

% Medir en el centro de cada tramo para evitar transitorios
seg1 = (tt >= 0.85 & tt < 0.95);
seg2 = (tt >= 1.35 & tt < 1.45);
seg3 = (tt >= 1.85 & tt < 1.95);
seg4 = (tt >= 2.85 & tt < 2.95);

dB1_meas = mean(Lin_dB3(seg1));
dB2_meas = mean(Lin_dB3(seg2));
dB3_meas = mean(Lin_dB3(seg3));
dB4_meas = mean(Lin_dB3(seg4));

fprintf('\n=== RMS esperado vs medido (mode 3) ===\n');
fprintf('A=0.1  esperado %.2f dB, medido %.2f dB\n', dB1_exp, dB1_meas);
fprintf('A=0.5  esperado %.2f dB, medido %.2f dB\n', dB2_exp, dB2_meas);
fprintf('A=0.2  esperado %.2f dB, medido %.2f dB\n', dB3_exp, dB3_meas);
fprintf('A=0.8  esperado %.2f dB, medido %.2f dB\n', dB4_exp, dB4_meas);

%% Comprobación 4: suavizado real de mode 3
% Desviación típica en una zona senoidal estable
std1 = std(Lin_dB1(seg2));
std3 = std(Lin_dB3(seg2));

fprintf('\n=== Suavizado ===\n');
fprintf('std Lin_dB modo 1 = %.3f dB\n', std1);
fprintf('std Lin_dB modo 3 = %.3f dB\n', std3);

%% Figuras
figure('Color','w','Name','ENV mono sys');

subplot(4,1,1);
plot(tt,x(1:nFrames*N),'k');
grid on;
ylabel('x');
title('Señal de prueba');

subplot(4,1,2);
plot(tt,env_1,'b',tt,env_2,'g--',tt,env_3,'r','LineWidth',1);
grid on;
ylabel('env');
legend('mode 1','mode 2','mode 3','Location','best');
title('Salida lineal');

subplot(4,1,3);
plot(tt,Lin_dB1,'b',tt,Lin_dB2,'g--',tt,Lin_dB3,'r','LineWidth',1);
grid on;
ylabel('Lin\_dB');
legend('mode 1','mode 2','mode 3','Location','best');
title('Salida en dB');

subplot(4,1,4);
plot(tt,Lin_dB1 - Lin_dB2,'m');
grid on;
xlabel('Tiempo (s)');
ylabel('\Delta dB');
title('Diferencia mode 1 - mode 2');