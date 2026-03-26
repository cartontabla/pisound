% test_fb_tones.m
%clear; clc;

Fs = 48000;
K  = int32(18);

Fmin = 100;
Fmax = min(6000, 0.45*Fs);

TYPE_LP = uint8(1);
TYPE_HP = uint8(2);
TYPE_BP = uint8(3);

[type,freq,Q,gain] = fb_params(Fmin, Fmax, K);

Keff = min(double(K), numel(type));
type = type(1:Keff);
freq = freq(1:Keff);
Q    = Q(1:Keff);
gain = gain(1:Keff);

% Parámetros del test
Nframe   = 128;
durTone  = 0.30;                    % s por tono
Ns       = round(durTone*Fs);
nFrames  = ceil(Ns/Nframe);
A        = 0.5;                     % amplitud del seno

% Matriz de resultados:
% rows = tono inyectado, cols = filtro medido
R = zeros(Keff, Keff);

for kt = 1:Keff
    % ---- tono de prueba en la frecuencia central de la banda kt ----
    n = (0:Ns-1).';
    x = A*sin(2*pi*freq(kt)/Fs*n);

    % Para cada filtro del banco
    for kf = 1:Keff
        obj = vcf_sys;
        if isprop(obj,'Fs'); obj.Fs = Fs; end

        y = zeros(Ns,1);

        for m = 1:nFrames
            i1 = (m-1)*Nframe + 1;
            i2 = min(m*Nframe, Ns);

            inL = zeros(Nframe,1);
            inR = zeros(Nframe,1);
            inL(1:(i2-i1+1)) = x(i1:i2);

            [outL,~] = obj(inL, inR, type(kf), freq(kf), Q(kf), gain(kf));
            y(i1:i2) = outL(1:(i2-i1+1));
        end

        % medir RMS evitando un poco el transitorio inicial
        iStart = min(round(0.05*Fs)+1, Ns);
        R(kt,kf) = sqrt(mean(y(iStart:end).^2) + 1e-18);
    end
end

R_dB = 20*log10(R + 1e-12);

% ---- mostrar tabla resumida ----
[~, idxMax] = max(R, [], 2);

fprintf('\n=== TEST TONOS EN FRECUENCIAS CENTRALES ===\n');
fprintf('tono k -> filtro que mas responde\n');
for kt = 1:Keff
    fprintf('tono %2d (%7.1f Hz) -> filtro %2d (%7.1f Hz), nivel = %6.2f dB\n', ...
        kt, freq(kt), idxMax(kt), freq(idxMax(kt)), R_dB(kt, idxMax(kt)));
end

% ---- mapa de calor ----
figure;
imagesc(1:Keff, 1:Keff, R_dB);
axis xy;
colorbar;
xlabel('Filtro medido');
ylabel('Tono inyectado');
title('Banco de filtros: respuesta RMS por tono central');
xticks(1:Keff); yticks(1:Keff);

% ---- gráfico de coincidencia ----
figure; hold on; grid on;
plot(1:Keff, 1:Keff, 'k--', 'LineWidth', 1.2);
plot(1:Keff, idxMax, 'o-', 'LineWidth', 1.5);
xlabel('Índice del tono inyectado');
ylabel('Índice del filtro que más responde');
title('Test de correspondencia tono-banda');

%% ========= TEST DE SINTESIS: suma temporal de todas las bandas =========
clear release

Nframe = 128;
dur    = 2.0;                  % s
L      = round(dur*Fs);
nF     = ceil(L/Nframe);

% Señal de prueba: ruido blanco
rng(0);
x = 0.2*randn(L,1);

% Alternativa:
% t = (0:L-1)'/Fs;
% x = 0.5*chirp(t, 80, t(end), 8000, 'logarithmic');

Ybands = zeros(L, Keff);

for k = 1:Keff
  obj = vcf_sys;
  if isprop(obj,'Fs'); obj.Fs = Fs; end

  yk = zeros(L,1);

  for m = 1:nF
    i1 = (m-1)*Nframe + 1;
    i2 = min(m*Nframe, L);

    inL = zeros(Nframe,1);
    inR = zeros(Nframe,1);
    inL(1:(i2-i1+1)) = x(i1:i2);

    [outL,~] = obj(inL, inR, type(k), freq(k), Q(k), gain(k));
    yk(i1:i2) = outL(1:(i2-i1+1));
  end

  Ybands(:,k) = yk;
  % release(obj);   % opcional; si molesta, déjalo comentado
end

ysum = sum(Ybands, 2);

%% ===== Métricas simples =====
rms_x   = sqrt(mean(x.^2));
rms_sum = sqrt(mean(ysum.^2));
fprintf('\n=== TEST DE SINTESIS ===\n');
fprintf('RMS entrada   = %.4f\n', rms_x);
fprintf('RMS suma      = %.4f\n', rms_sum);
fprintf('Ganancia total= %.2f dB\n', 20*log10(rms_sum/(rms_x+1e-12)+1e-12));

%% ===== Comparación espectral =====
nfft = 65536;
X    = fft(x, nfft);
Ysum = fft(ysum, nfft);
f    = (0:nfft/2)'*(Fs/nfft);

magX_dB    = 20*log10(abs(X(1:nfft/2+1))+1e-12);
magYsum_dB = 20*log10(abs(Ysum(1:nfft/2+1))+1e-12);
magErr_dB  = magYsum_dB - magX_dB;

figure; hold on; grid on;
semilogx(f, magX_dB, 'k', 'LineWidth', 1.2);
semilogx(f, magYsum_dB, 'r', 'LineWidth', 1.2);
xlim([80 8000]);
ylim([-120 20]);
xlabel('Frecuencia (Hz)');
ylabel('Magnitud (dB)');
title('Sintesis: entrada vs suma de bandas');
legend('Entrada','Suma bandas','Location','southwest');

figure; grid on;
semilogx(f, magErr_dB, 'b', 'LineWidth', 1.2);
xlim([80 8000]);
ylim([-24 24]);
xlabel('Frecuencia (Hz)');
ylabel('\Delta magnitud (dB)');
title('Error espectral: suma de bandas - entrada');

%% ===== Escucha rápida (si quieres) =====
% soundsc([x; zeros(Fs/4,1); ysum], Fs);