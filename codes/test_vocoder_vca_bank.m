% test_vocoder_vca_bank.m
%clear; clc;

%% ================== PARAMETROS ==================
Fs = 48000;
N  = 128;
K  = int32(18);

Fmin = 100;
Fmax = min(6000, 0.45*Fs);

dur = 2.0;                 % s
L   = round(dur*Fs);
nF  = ceil(L/N);

[type,freq,Q,gain] = fb_params(Fmin, Fmax, K);

Keff = min(double(K), numel(type));
type = type(1:Keff);
freq = freq(1:Keff);
Q    = Q(1:Keff);
gain = gain(1:Keff);

%% ================== PORTADORA ==================
% Usa una portadora rica. Puedes cambiarla por sierra real si quieres.
t = (0:L-1)'/Fs;

carrier = ...
    0.25*sin(2*pi*110*t) + ...
    0.20*sin(2*pi*220*t) + ...
    0.18*sin(2*pi*330*t) + ...
    0.15*sin(2*pi*440*t) + ...
    0.12*sin(2*pi*660*t) + ...
    0.10*sin(2*pi*880*t);

% alternativa:
% carrier = sawtooth(2*pi*110*t);

%% ================== CONTROLES ARTIFICIALES ==================
% Matrices [Keff x nF], en dB

G_open   = zeros(Keff, nF);          % todo abierto
G_closed = -80*ones(Keff, nF);       % todo cerrado

% solo una banda abierta
kSolo = 8;
G_one = -80*ones(Keff, nF);
G_one(kSolo,:) = 0;

% banda abierta que se desplaza cada cierto numero de frames
G_sweep = -80*ones(Keff, nF);
framesPerBand = max(1, floor(nF / Keff));
for m = 1:nF
    kact = min(Keff, floor((m-1)/framesPerBand) + 1);
    G_sweep(kact,m) = 0;
end

%% ================== FUNCION DE PROCESADO ==================
y_open   = run_bank_vca_sum(carrier, Fs, N, type, freq, Q, gain, G_open);
y_closed = run_bank_vca_sum(carrier, Fs, N, type, freq, Q, gain, G_closed);
y_one    = run_bank_vca_sum(carrier, Fs, N, type, freq, Q, gain, G_one);
y_sweep  = run_bank_vca_sum(carrier, Fs, N, type, freq, Q, gain, G_sweep);

%% ================== METRICAS ==================
rms_carrier = sqrt(mean(carrier.^2));
rms_open    = sqrt(mean(y_open.^2));
rms_closed  = sqrt(mean(y_closed.^2));
rms_one     = sqrt(mean(y_one.^2));
rms_sweep   = sqrt(mean(y_sweep.^2));

fprintf('\n=== TEST VOcoder: banco + VCA + suma ===\n');
fprintf('RMS carrier original   = %.6f\n', rms_carrier);
fprintf('RMS all open (0 dB)    = %.6f\n', rms_open);
fprintf('RMS all closed (-80dB) = %.6f\n', rms_closed);
fprintf('RMS one band open      = %.6f\n', rms_one);
fprintf('RMS moving band        = %.6f\n', rms_sweep);

fprintf('\nRel closed/open = %.2f dB\n', 20*log10((rms_closed+1e-15)/(rms_open+1e-15)));
fprintf('Rel one/open    = %.2f dB\n', 20*log10((rms_one+1e-15)/(rms_open+1e-15)));

%% ================== GRAFICAS TEMPORALES ==================
tt = (0:L-1)/Fs;

figure; plot(tt, carrier); grid on;
title('Carrier original'); xlabel('t (s)'); ylabel('amplitud');

figure; plot(tt, y_open); grid on;
title('Salida: todas las bandas abiertas'); xlabel('t (s)'); ylabel('amplitud');

figure; plot(tt, y_closed); grid on;
title('Salida: todas las bandas cerradas'); xlabel('t (s)'); ylabel('amplitud');

figure; plot(tt, y_one); grid on;
title(sprintf('Salida: solo banda %d abierta', kSolo)); xlabel('t (s)'); ylabel('amplitud');

figure; plot(tt, y_sweep); grid on;
title('Salida: banda abierta desplazandose'); xlabel('t (s)'); ylabel('amplitud');

%% ================== ESPECTROS ==================
nfft = 65536;
f = (0:nfft/2)'*(Fs/nfft);

Xc = fft(carrier, nfft);
Yo = fft(y_open,   nfft);
Yz = fft(y_closed, nfft);
Y1 = fft(y_one,    nfft);
Ys = fft(y_sweep,  nfft);

magC = 20*log10(abs(Xc(1:nfft/2+1))+1e-12);
magO = 20*log10(abs(Yo(1:nfft/2+1))+1e-12);
magZ = 20*log10(abs(Yz(1:nfft/2+1))+1e-12);
mag1 = 20*log10(abs(Y1(1:nfft/2+1))+1e-12);
magS = 20*log10(abs(Ys(1:nfft/2+1))+1e-12);

figure; hold on; grid on;
semilogx(f, magC, 'k', 'LineWidth', 1.2);
semilogx(f, magO, 'b', 'LineWidth', 1.2);
xlim([80 8000]); ylim([-120 20]);
title('Carrier vs todas abiertas');
xlabel('Frecuencia (Hz)'); ylabel('Magnitud (dB)');
legend('carrier','all open','Location','southwest');

figure; hold on; grid on;
semilogx(f, magZ, 'r', 'LineWidth', 1.2);
xlim([80 8000]); ylim([-140 20]);
title('Todas cerradas');
xlabel('Frecuencia (Hz)'); ylabel('Magnitud (dB)');

figure; hold on; grid on;
semilogx(f, mag1, 'm', 'LineWidth', 1.2);
for k = 1:Keff
    xline(freq(k), ':', 'HandleVisibility','off');
end
xlim([80 8000]); ylim([-120 20]);
title(sprintf('Solo banda %d abierta', kSolo));
xlabel('Frecuencia (Hz)'); ylabel('Magnitud (dB)');

figure; hold on; grid on;
semilogx(f, magS, 'g', 'LineWidth', 1.2);
for k = 1:Keff
    xline(freq(k), ':', 'HandleVisibility','off');
end
xlim([80 8000]); ylim([-120 20]);
title('Banda abierta desplazandose');
xlabel('Frecuencia (Hz)'); ylabel('Magnitud (dB)');

%% ================== ESCUCHA OPCIONAL ==================
% soundsc([carrier; zeros(Fs/4,1); y_open; zeros(Fs/4,1); y_closed; ...
%          zeros(Fs/4,1); y_one; zeros(Fs/4,1); y_sweep], Fs);

%% ================== FUNCION LOCAL ==================
function ysum = run_bank_vca_sum(x, Fs, N, type, freq, Q, gain, Gctrl_dB)

    Keff = numel(type);
    L    = numel(x);
    nF   = ceil(L/N);

    ysum = zeros(L,1);

    % un filtro y un VCA por banda
    F = cell(Keff,1);
    V = cell(Keff,1);

    for k = 1:Keff
        F{k} = vcf_sys;
        if isprop(F{k},'Fs'); F{k}.Fs = Fs; end

        V{k} = vca_mono_sys;
        V{k}.N = N;
        V{k}.Fs = Fs;
        V{k}.Mode = uint8(1);   % control en dB
        V{k}.bypass = uint8(0);
    end

    for m = 1:nF
        i1 = (m-1)*N + 1;
        i2 = min(m*N, L);
        Mloc = i2-i1+1;

        inL = zeros(N,1);
        inR = zeros(N,1);
        inL(1:Mloc) = x(i1:i2);

        yframe = zeros(N,1);

        for k = 1:Keff
            [bandL,~] = F{k}(inL, inR, type(k), freq(k), Q(k), gain(k));

            gk = Gctrl_dB(k,m) * ones(N,1);  % control constante por frame
            yk = step(V{k}, bandL, gk);

            yframe = yframe + yk;
        end

        ysum(i1:i2) = yframe(1:Mloc);
    end
end