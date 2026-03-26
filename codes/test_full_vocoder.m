%% sim_vocoder_script.m
% Vocoder offline equivalente al modelo Simulink
% L = voz (moduladora)
% R = sierra (portadora)
%
% Requiere en el path:
%   vocoder_params.m
%   vcf_sys.m
%   env_mono_sys.m
%   map_sys.m
%   arf_sys.m
%   vca_mono_sys.m
%
% Supuestos:
% - Mix = 1 (solo wet)
% - ENV usa modo RMS (2)
% - MAP usa vocoder_env_db con threshold+slope (Mode 1)
% - La salida final es mono wet; abajo la duplico a estéreo

%clear; clc;

%% Parámetros globales
Fs = 48000;
N  = 128;          % samples/frame
K  = 18;           % subbandas
fmin = 160;
fmax = 7000;

% Control
envMode   = 2;     % 1=peak, 2=RMS exponencial
mapMode   = 1;     % vocoder_env_db con threshold+slope
attack_s  = 0.005; % segundos
release_s = 0.030; % segundos

% Ajuste global opcional del control
ctrlOffset_dB = 0;   % desplaza todas las ganancias
outputTrim_dB = -12; % trim final para evitar clipping en la suma

%% Señales de prueba
% Puedes sustituir esto por audioread(...) o por tus buffers reales.

dur = 25;                    % s
t = (0:1/Fs:dur-1/Fs).';

% Voz de prueba: usa archivo si lo tienes
[voice, FsFile] = audioread('voice.wav');
voice = mean(voice,2);
if FsFile ~= Fs, voice = resample(voice, Fs, FsFile); end
voice = voice(1:min(end,numel(t)));
voice(end+1:numel(t),1) = 0;

% Si no hay archivo, sintetizo una pseudo-voz modesta
%voice = 0.25 * synthPseudoVoice(t);

% Sierra portadora
f0 = 110;
saw = 0.20 * sawtooth(2*pi*f0*t, 1.0);

% Convención del modelo
xL = voice;   % L = moduladora
xR = saw;     % R = portadora

% Igualar longitud a múltiplo de N
Lsig = floor(numel(xL)/N) * N;
xL = xL(1:Lsig);
xR = xR(1:Lsig);

nFrames = Lsig / N;

%% Parámetros por banda
[type, freq, Q, gain, outMin, outMax] = vocoder_params(fmin, fmax, K);

% Recortamos a K bandas activas
type   = double(type(1:K));
freq   = double(freq(1:K));
Q      = double(Q(1:K));
gain   = double(gain(1:K));
outMin = double(outMin(1:K));
outMax = double(outMax(1:K));

%% Crear una instancia por banda
% Esto emula el For Each: una instancia con estado independiente por banda.

VCF = cell(K,1);
ENV = cell(K,1);
MAP = cell(K,1);
ARF = cell(K,1);
VCA = cell(K,1);

for k = 1:K
    VCF{k} = vcf_sys;
    VCF{k}.N  = N;
    VCF{k}.Fs = Fs;
    VCF{k}.bypass = uint8(0);

    ENV{k} = env_mono_sys;
    ENV{k}.N  = N;
    ENV{k}.Fs = Fs;

    MAP{k} = map_sys;
    MAP{k}.bypass = uint8(0);
    MAP{k}.Mode   = uint8(mapMode);

    ARF{k} = arf_sys;
    ARF{k}.N  = N;
    ARF{k}.Fs = Fs;
    ARF{k}.bypass = uint8(0);

    VCA{k} = vca_mono_sys;
    VCA{k}.N  = N;
    VCA{k}.Fs = Fs;
    VCA{k}.bypass = uint8(0);
    VCA{k}.Mode   = uint8(1); % gain en dB
end

%% Buffers de salida y diagnóstico
yWet      = zeros(Lsig,1);
gBandMean = zeros(nFrames,K);
gBandMin  = zeros(nFrames,K);
gBandMax  = zeros(nFrames,K);
eBandMean = zeros(nFrames,K);

%% Procesado frame a frame
for m = 1:nFrames
    idx = (m-1)*N + (1:N);

    % Frame estéreo de entrada
    inMod = xL(idx);   % voz
    inPor = xR(idx);   % sierra

    % Matriz [N x K] para sumar al final
    yBands = zeros(N,K);

    for k = 1:K
        % 1) Filtrado por banda de ambos canales
        [modBand, porBand] = VCF{k}(inMod, inPor, type(k), freq(k), Q(k), gain(k));

        % 2) Envolvente del modulador
        [env_dB, ~] = ENV{k}(modBand, envMode);

        % 3) Mapeo dB->dB
        g0_dB = MAP{k}(env_dB, outMin(k), outMax(k));

        % 4) Attack / Release
        g_dB = ARF{k}(g0_dB, attack_s, release_s);

        % 5) Ajuste opcional global
        g_dB = g_dB + ctrlOffset_dB;

        % 6) VCA sobre la portadora filtrada
        yBands(:,k) = VCA{k}(porBand, g_dB);

        % Diagnóstico
        gBandMean(m,k) = mean(g_dB);
        gBandMin(m,k)  = min(g_dB);
        gBandMax(m,k)  = max(g_dB);
        eBandMean(m,k) = mean(env_dB);
    end

    % 7) Suma de subbandas (wet only)
    yWet(idx) = sum(yBands, 2);
end

%% Trim final
yWet = db2mag(outputTrim_dB) * yWet;

% Salida estéreo duplicada
yStereo = [yWet, yWet];

%% Métricas rápidas
peakOut = max(abs(yWet));
rmsOut  = sqrt(mean(yWet.^2));

fprintf('\n=== Vocoder offline ===\n');
fprintf('Fs = %d Hz, N = %d, K = %d\n', Fs, N, K);
fprintf('Peak salida = %.4f (%.2f dBFS)\n', peakOut, mag2db(max(peakOut,1e-12)));
fprintf('RMS  salida = %.4f (%.2f dBFS)\n', rmsOut,  mag2db(max(rmsOut, 1e-12)));

fprintf('\nGanancia media por banda (dB), promedio temporal:\n');
disp(mean(gBandMean,1));

%% Escucha y guardado
soundsc(yStereo, Fs);
audiowrite('vocoder_offline.wav', yStereo, Fs);

%% Gráficas
tt = (0:Lsig-1)'/Fs;

figure('Name','Entradas y salida');
subplot(3,1,1);
plot(tt, xL); grid on;
title('Entrada L = voz (moduladora)');
xlabel('Tiempo (s)'); ylabel('Amplitud');

subplot(3,1,2);
plot(tt, xR); grid on;
title('Entrada R = sierra (portadora)');
xlabel('Tiempo (s)'); ylabel('Amplitud');

subplot(3,1,3);
plot(tt, yWet); grid on;
title('Salida wet');
xlabel('Tiempo (s)'); ylabel('Amplitud');

figure('Name','Ganancia media por banda');
imagesc((0:nFrames-1)*N/Fs, 1:K, gBandMean.');
axis xy;
colorbar;
xlabel('Tiempo (s)');
ylabel('Banda');
title('Ganancia media por banda (dB)');

figure('Name','Envolvente media por banda');
imagesc((0:nFrames-1)*N/Fs, 1:K, eBandMean.');
axis xy;
colorbar;
xlabel('Tiempo (s)');
ylabel('Banda');
title('ENV media por banda (dBFS)');

%% Pruebas binarias rápidas
% 1) Sin voz -> debería quedar sierra poco o nada articulada
% 2) Sin sierra -> debería caer mucho o quedar casi nada útil
doBinaryTests = false;

if doBinaryTests
    fprintf('\n=== Pruebas binarias ===\n');
    yNoVoice = runCore(zeros(size(xL)), xR, Fs, N, K, ...
        type, freq, Q, gain, outMin, outMax, ...
        envMode, mapMode, attack_s, release_s, ctrlOffset_dB, outputTrim_dB);

    yNoSaw = runCore(xL, zeros(size(xR)), Fs, N, K, ...
        type, freq, Q, gain, outMin, outMax, ...
        envMode, mapMode, attack_s, release_s, ctrlOffset_dB, outputTrim_dB);

    fprintf('Sin voz  -> RMS %.2f dBFS\n', mag2db(max(rms(yNoVoice),1e-12)));
    fprintf('Sin sierra -> RMS %.2f dBFS\n', mag2db(max(rms(yNoSaw),1e-12)));
end

%% Funciones locales
function x = synthPseudoVoice(t)
% Señal vocal-like simple para pruebas rápidas
    f1 = 140 + 20*sin(2*pi*0.7*t);
    f2 = 2*f1;
    f3 = 3*f1;

    exc = 0.7*sin(2*pi*cumtrapz(t,f1)) + ...
          0.2*sin(2*pi*cumtrapz(t,f2)) + ...
          0.1*sin(2*pi*cumtrapz(t,f3));

    env = 0.5 + 0.5*sin(2*pi*2.2*t).^2;
    env = env .* (0.7 + 0.3*sin(2*pi*0.35*t + 0.8));

    x = env .* tanh(1.5*exc);

    % "sílabas" con pausas suaves
    gate = double(mod(floor(t*3),2)==0);
    gate = smoothdata(gate, "gaussian", 401);
    x = x .* gate;
end

function y = runCore(xL, xR, Fs, N, K, type, freq, Q, gain, ...
        outMin, outMax, envMode, mapMode, attack_s, ...
        release_s, ctrlOffset_dB, outputTrim_dB)

    Lsig = numel(xL);
    nFrames = Lsig / N;
    y = zeros(Lsig,1);

    VCF = cell(K,1); ENV = cell(K,1); MAP = cell(K,1); ARF = cell(K,1); VCA = cell(K,1);
    for k = 1:K
        VCF{k} = vcf_sys; VCF{k}.N = N; VCF{k}.Fs = Fs;
        ENV{k} = env_mono_sys; ENV{k}.N = N; ENV{k}.Fs = Fs;
        MAP{k} = map_sys; MAP{k}.Mode = uint8(mapMode);
        ARF{k} = arf_sys; ARF{k}.N = N; ARF{k}.Fs = Fs;
        VCA{k} = vca_mono_sys; VCA{k}.N = N; VCA{k}.Fs = Fs; VCA{k}.Mode = uint8(1);
    end

    for m = 1:nFrames
        idx = (m-1)*N + (1:N);
        inMod = xL(idx);
        inPor = xR(idx);

        yBands = zeros(N,K);
        for k = 1:K
            [modBand, porBand] = VCF{k}(inMod, inPor, type(k), freq(k), Q(k), gain(k));
            [env_dB, ~] = ENV{k}(modBand, envMode);
            g0_dB = MAP{k}(env_dB, outMin(k), outMax(k));
            g_dB  = ARF{k}(g0_dB, attack_s, release_s) + ctrlOffset_dB;
            yBands(:,k) = VCA{k}(porBand, g_dB);
        end
        y(idx) = sum(yBands,2);
    end

    y = db2mag(outputTrim_dB) * y;
end