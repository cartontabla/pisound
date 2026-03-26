function R = diag_ctrl_voice(wavFile, K)
% diag_ctrl_voice
% Diagnóstico de señales de control a partir de una voz.
%
% Objetivo:
%   Extraer y representar señales que permitan inferir qué "usa realmente"
%   un bloque CTRL tipo vocoder:
%     - envolvente global
%     - envolventes por banda
%     - banda dominante
%     - centroide / frecuencia representativa
%
% Uso:
%   R = diag_ctrl_voice('voice.wav', 16);
%
% Salida:
%   R: struct con señales intermedias y medidas.

if nargin < 1
    wavFile = 'voice.wav';
end
if nargin < 2
    K = 16;
end

%% ----------------------- Parámetros de análisis -----------------------
fmin = 120;
fmax = 8000;
envCutoff = 30;          % Hz, LPF para envolvente
frameMs = 20;            % ms
hopMs   = 10;            % ms
plotSeconds = 12;        % segundos mostrados en gráficas temporales
eps0 = 1e-12;

%% -------------------------- Lectura audio -----------------------------
[x, Fs] = audioread(wavFile);

if size(x,2) == 2
    xMono = mean(x, 2);
else
    xMono = x;
end

xMono = double(xMono);
xMono(~isfinite(xMono)) = 0;

% evita problemas con versiones antiguas de MATLAB
m = mean(xMono(isfinite(xMono)));
if isempty(m) || ~isfinite(m)
    m = 0;
end
xMono = xMono - m;

mx = max(abs(xMono));
if isempty(mx) || mx == 0 || ~isfinite(mx)
    error('La señal de entrada es nula o no finita.');
end
xMono = xMono / mx;

N = length(xMono);
t = (0:N-1)'/Fs;

fprintf('Archivo: %s\n', wavFile);
fprintf('Fs = %d Hz\n', Fs);
fprintf('Canales = %d\n', size(x,2));
fprintf('Duración = %.3f s\n', N/Fs);

%% ----------------- Señal global: envolvente y energía -----------------
% LPF robusto en SOS para la envolvente
[zEnv,pEnv,kEnv] = butter(2, envCutoff/(Fs/2));
[sosEnv, gEnv] = zp2sos(zEnv,pEnv,kEnv);

envGlobal = filtfilt(sosEnv, gEnv, abs(xMono));
envGlobal(~isfinite(envGlobal)) = 0;
envGlobal = max(envGlobal, 0);

% RMS en ventana corta
frameLen = max(1, round(frameMs*1e-3*Fs));
hopLen   = max(1, round(hopMs*1e-3*Fs));

if N < frameLen
    error('La señal es más corta que una ventana de análisis.');
end

nFrames  = 1 + floor((N - frameLen)/hopLen);

rmsFrame = zeros(nFrames,1);
zcrFrame = zeros(nFrames,1);
specPeakHz = zeros(nFrames,1);
specCentroidHz = zeros(nFrames,1);

w = hann(frameLen, 'periodic');
nfft = 2^nextpow2(max(1024, frameLen));

for i = 1:nFrames
    i0 = (i-1)*hopLen + 1;
    i1 = i0 + frameLen - 1;
    xf = xMono(i0:i1);

    if any(~isfinite(xf))
        xf(~isfinite(xf)) = 0;
    end

    rmsFrame(i) = sqrt(mean(xf.^2));

    sgn = sign(xf);
    zcrFrame(i) = sum(abs(diff(sgn)) > 0) / max(1, length(xf)-1);

    X = fft(xf .* w, nfft);
    P = abs(X(1:nfft/2+1)).^2;
    f = (0:nfft/2)' * Fs / nfft;

    P1 = P;
    P1(1) = 0;
    [~, idxPk] = max(P1);
    specPeakHz(i) = f(idxPk);

    specCentroidHz(i) = sum(f .* P) / (sum(P) + eps0);
end

tf = ((0:nFrames-1)*hopLen + frameLen/2)'/Fs;

%% ------------------------- Banco de filtros ---------------------------
% Bandas logarítmicas simples para diagnóstico
fc = logspace(log10(fmin), log10(fmax), K);

% Bordes a partir de medias geométricas
edges = zeros(K+1,1);
edges(1) = fmin;
edges(end) = min(fmax, Fs/2 * 0.98);

for k = 2:K
    edges(k) = sqrt(fc(k-1)*fc(k));
end

bandSig  = zeros(N, K);
bandEnv  = zeros(N, K);
bandRMS  = zeros(nFrames, K);
bandOK   = false(K,1);

for k = 1:K
    fl = edges(k);
    fh = edges(k+1);

    if fh >= 0.98*(Fs/2)
        fh = 0.98*(Fs/2);
    end
    if fl <= 20
        fl = 20;
    end
    if fh <= fl * 1.05
        fh = min(fl * 1.2, 0.98*(Fs/2));
    end

    try
        [z,p,kk] = butter(4, [fl fh]/(Fs/2), 'bandpass');
        [sos, g] = zp2sos(z,p,kk);

        y = filtfilt(sos, g, xMono);

        if any(~isfinite(y))
            warning('Banda %d no finita tras filtfilt: [%.1f %.1f] Hz', k, fl, fh);
            continue;
        end

        e = filtfilt(sosEnv, gEnv, abs(y));

        if any(~isfinite(e))
            warning('Envolvente no finita en banda %d: [%.1f %.1f] Hz', k, fl, fh);
            continue;
        end

        bandSig(:,k) = y;
        bandEnv(:,k) = max(e, 0);
        bandOK(k) = true;

        for i = 1:nFrames
            i0 = (i-1)*hopLen + 1;
            i1 = i0 + frameLen - 1;
            yy = y(i0:i1);
            if any(~isfinite(yy))
                yy(~isfinite(yy)) = 0;
            end
            bandRMS(i,k) = sqrt(mean(yy.^2));
        end

    catch ME
        warning('Falló banda %d [%.1f %.1f] Hz: %s', k, fl, fh, ME.message);
    end
end

fprintf('\nBandas válidas: %d de %d\n', sum(bandOK), K);
disp('Índices de bandas válidas:');
disp(find(bandOK).');

% por seguridad, si alguna banda falló, queda a cero
bandSig(:,~bandOK) = 0;
bandEnv(:,~bandOK) = 0;
bandRMS(:,~bandOK) = 0;

%% ------------------ Medidas "qué usa realmente" -----------------------
% 1) energía total por bandas
sumBandEnv = sum(bandEnv, 2);

% 2) banda dominante instantánea
[domBandVal, domBandIdx] = max(bandEnv, [], 2);
domBandFreq = fc(domBandIdx).';

% 3) centroide espectral por bandas
bandCentroid = (bandEnv * fc(:)) ./ (sumBandEnv + eps0);

% 4) flatness simple entre bandas
bandFlatness = zeros(N,1);
for n = 1:N
    v = bandEnv(n,:) + eps0;
    bandFlatness(n) = exp(mean(log(v))) / mean(v);
end

% 5) correlaciones útiles
c1 = safe_corr(envGlobal, sumBandEnv);
c2 = safe_corr(envGlobal, domBandVal);
c3 = safe_corr(sumBandEnv, domBandVal);

% interpola centroide de banda a tiempos de frame
bandCentroidFrame = interp1(t, bandCentroid, tf, 'linear', 'extrap');
c4 = safe_corr(specCentroidHz, bandCentroidFrame);

fprintf('\nCorrelaciones diagnósticas:\n');
fprintf('corr(envGlobal, sumBandEnv)      = %.4f\n', c1);
fprintf('corr(envGlobal, domBandVal)      = %.4f\n', c2);
fprintf('corr(sumBandEnv, domBandVal)     = %.4f\n', c3);
fprintf('corr(specCentroid, bandCentroid) = %.4f\n', c4);

%% ---------------------------- Gráficas -------------------------------
tmax = min(plotSeconds, N/Fs);
idxT = (t <= tmax);
idxF = (tf <= tmax);

figure('Name','Diagnóstico CTRL - Resumen','Color','w');

subplot(3,2,1);
plot(t(idxT), xMono(idxT), 'k'); hold on;
plot(t(idxT), envGlobal(idxT), 'r', 'LineWidth', 1.2);

sumBandScaled = sumBandEnv;
m1 = max(sumBandScaled(idxT) + eps0);
m2 = max(envGlobal(idxT) + eps0);
sumBandScaled = sumBandScaled / m1 * m2;

plot(t(idxT), sumBandScaled(idxT), 'b', 'LineWidth', 1.0);
grid on;
xlabel('Tiempo (s)');
ylabel('Amplitud');
title('Señal y envolventes');
legend('voz','env global','suma env bandas');

subplot(3,2,2);
spectrogram(xMono, hann(round(0.03*Fs)), round(0.02*Fs), 1024, Fs, 'yaxis');
title('Espectrograma');

subplot(3,2,3);
imagesc(tf(idxF), 1:K, 20*log10(bandRMS(idxF,:)' + eps0));
axis xy;
xlabel('Tiempo (s)');
ylabel('Banda');
title('Energía por bandas (dB)');
colorbar;

subplot(3,2,4);
plot(t(idxT), domBandFreq(idxT), 'LineWidth', 1.0); hold on;
plot(t(idxT), bandCentroid(idxT), 'LineWidth', 1.0);
grid on;
xlabel('Tiempo (s)');
ylabel('Hz');
title('Frecuencia dominante / centroide por bandas');
legend('banda dominante','centroide bandas');

subplot(3,2,5);
plot(tf(idxF), specPeakHz(idxF), 'LineWidth', 1.0); hold on;
plot(tf(idxF), specCentroidHz(idxF), 'LineWidth', 1.0);
grid on;
xlabel('Tiempo (s)');
ylabel('Hz');
title('Pico espectral / centroide espectral');
legend('pico FFT','centroide FFT');

subplot(3,2,6);
plot(t(idxT), bandFlatness(idxT), 'LineWidth', 1.0);
grid on;
xlabel('Tiempo (s)');
ylabel('Flatness');
title('Planitud espectral entre bandas');

sgtitle('Diagnóstico de señales candidatas para CTRL');

%% ----------------- Figura detallada por bandas -----------------------
figure('Name','Envolventes por banda','Color','w');
nShow = min(K, 8);

for k = 1:nShow
    subplot(nShow,1,k);
    plot(t(idxT), bandEnv(idxT,k), 'k');
    grid on;
    ylabel(sprintf('B%d', k));
    if k == 1
        title('Envolventes por banda');
    end
    if k == nShow
        xlabel('Tiempo (s)');
    end
end

%% --------------- Figura comparativa de medidas globales --------------
figure('Name','Comparativa de medidas de control','Color','w');

yyaxis left;
plot(t(idxT), envGlobal(idxT), 'r', 'LineWidth', 1.1); hold on;
plot(t(idxT), domBandVal(idxT), 'b', 'LineWidth', 1.0);
plot(t(idxT), sumBandScaled(idxT), 'k', 'LineWidth', 1.0);
ylabel('Amplitud relativa');

yyaxis right;
plot(t(idxT), domBandFreq(idxT), '--', 'LineWidth', 1.0);
plot(t(idxT), bandCentroid(idxT), ':', 'LineWidth', 1.2);
ylabel('Hz');

grid on;
xlabel('Tiempo (s)');
title('¿CTRL usa amplitud global, banda dominante o frecuencia representativa?');
legend('env global','valor banda dominante','suma env bandas','freq banda dominante','centroide bandas');

%% ------------------------- Resultados --------------------------------
R = struct();
R.wavFile = wavFile;
R.Fs = Fs;
R.xMono = xMono;
R.t = t;

R.envGlobal = envGlobal;
R.sumBandEnv = sumBandEnv;
R.domBandVal = domBandVal;
R.domBandIdx = domBandIdx;
R.domBandFreq = domBandFreq;
R.bandCentroid = bandCentroid;
R.bandFlatness = bandFlatness;

R.tf = tf;
R.rmsFrame = rmsFrame;
R.zcrFrame = zcrFrame;
R.specPeakHz = specPeakHz;
R.specCentroidHz = specCentroidHz;
R.bandCentroidFrame = bandCentroidFrame;

R.fc = fc;
R.edges = edges;
R.bandSig = bandSig;
R.bandEnv = bandEnv;
R.bandRMS = bandRMS;
R.bandOK = bandOK;

R.corr_envGlobal_sumBandEnv = c1;
R.corr_envGlobal_domBandVal = c2;
R.corr_sumBandEnv_domBandVal = c3;
R.corr_specCentroid_bandCentroid = c4;

end

function c = safe_corr(x, y)
x = x(:);
y = y(:);

m = isfinite(x) & isfinite(y);
x = x(m);
y = y(m);

if numel(x) < 2 || numel(y) < 2
    c = NaN;
    return;
end

sx = std(x);
sy = std(y);

if sx < eps || sy < eps
    c = NaN;
    return;
end

C = corrcoef(x, y);
c = C(1,2);
end