clear; clc;

%% CONFIG
Fs = 48000;
N  = 128;
K  = int32(18);

Fmin = 100;
Fmax = min(6000, 0.45*Fs);

ATTACK_S  = 0.005;
RELEASE_S = 0.010;

MAP_OUT_MIN = -80;
MAP_OUT_MAX = 0;

[type,freq,Q,gain] = fb_params(Fmin, Fmax, K);
Keff = double(K);
type = type(1:Keff);
freq = freq(1:Keff);
Q    = Q(1:Keff);
gain = gain(1:Keff);

%% CARGA MODULADORA Y PORTADORA
[mod,Fs1] = audioread('voz.wav');
[por,Fs2] = audioread('sierra.wav');

if size(mod,2)>1, mod = mean(mod,2); end
if size(por,2)>1, por = mean(por,2); end

if Fs1~=Fs, mod = resample(mod,Fs,Fs1); end
if Fs2~=Fs, por = resample(por,Fs,Fs2); end

L = min(numel(mod), numel(por));
mod = mod(1:L);
por = por(1:L);

nF = ceil(L/N);
Lpad = nF*N;

modx = zeros(Lpad,1); modx(1:L)=mod;
porx = zeros(Lpad,1); porx(1:L)=por;

%% =========================================================
%% PASADA 1: SOLO ANALISIS DE LA MODULADORA -> Lenv(k,m)
%% =========================================================
F1 = cell(Keff,1);
E1 = cell(Keff,1);

for k = 1:Keff
    F1{k} = vcf_sys;
    if isprop(F1{k},'Fs'), F1{k}.Fs = Fs; end

    E1{k} = env_mono_sys;
    E1{k}.N  = N;
    E1{k}.Fs = Fs;
end

Lenv = zeros(Keff,nF);

for m = 1:nF
    i1 = (m-1)*N + 1;
    i2 = m*N;

    inMod = modx(i1:i2);
    inPor = porx(i1:i2); %#ok<NASGU>  % no se usa en esta pasada

    for k = 1:Keff
        [bandMod,~] = F1{k}(inMod, zeros(N,1), type(k), freq(k), Q(k), gain(k));
        le = step(E1{k}, bandMod, 2);

        % resumen por frame para calibracion
        Lenv(k,m) = mean(le);
    end
end

%% CALIBRACION MAP POR BANDA
InMin_k = zeros(Keff,1);
InMax_k = zeros(Keff,1);

for k = 1:Keff
    xk = Lenv(k,:);

    % percentiles por banda
    InMin_k(k) = prctile(xk,10);
    InMax_k(k) = prctile(xk,95);

    % ancho minimo para evitar mapas degenerados
    if InMax_k(k) - InMin_k(k) < 10
        InMax_k(k) = InMin_k(k) + 10;
    end
end

fprintf('\n=== CALIBRACION MAP POR BANDA ===\n');
for k = 1:Keff
    fprintf('B%02d  InMin=%7.2f dB   InMax=%7.2f dB\n', ...
        k, InMin_k(k), InMax_k(k));
end

%% =========================================================
%% PASADA 2: VOCODER COMPLETO CON MAP POR BANDA
%% =========================================================
F2 = cell(Keff,1);
E2 = cell(Keff,1);
M2 = cell(Keff,1);
A2 = cell(Keff,1);
V2 = cell(Keff,1);

for k = 1:Keff
    F2{k} = vcf_sys;
    if isprop(F2{k},'Fs'), F2{k}.Fs = Fs; end

    E2{k} = env_mono_sys;
    E2{k}.N  = N;
    E2{k}.Fs = Fs;

    M2{k} = map_sys;
    M2{k}.bypass = 0;
    M2{k}.Mode   = 3;
    M2{k}.InMin  = InMin_k(k);
    M2{k}.InMax  = InMax_k(k);
    M2{k}.OutMin = MAP_OUT_MIN;
    M2{k}.OutMax = MAP_OUT_MAX;

    A2{k} = arf_sys;
    A2{k}.N  = N;
    A2{k}.Fs = Fs;

    V2{k} = vca_mono_sys;
    V2{k}.N      = N;
    V2{k}.Fs     = Fs;
    V2{k}.Mode   = uint8(1);   % control en dB
    V2{k}.bypass = uint8(0);
end

G0 = zeros(Keff,nF);
G  = zeros(Keff,nF);
Y  = zeros(Lpad,1);

for m = 1:nF
    i1 = (m-1)*N + 1;
    i2 = m*N;

    inMod = modx(i1:i2);
    inPor = porx(i1:i2);

    ysum = zeros(N,1);

    for k = 1:Keff
        [bandMod, bandPor] = F2{k}(inMod, inPor, type(k), freq(k), Q(k), gain(k));

        le = step(E2{k}, bandMod, 2);
        g0 = step(M2{k}, le);
        g  = step(A2{k}, g0, ATTACK_S, RELEASE_S);
        yk = step(V2{k}, bandPor, g);

        G0(k,m) = mean(g0);
        G(k,m)  = mean(g);
        ysum = ysum + yk;
    end

    Y(i1:i2) = ysum;
end

%% =========================================================
%% GRAFICAS
%% =========================================================
figure;
imagesc(Lenv);
axis xy; colorbar;
xlabel('Frame'); ylabel('Banda');
title('ENV por banda Lenv(k,m) [dB]');

figure;
imagesc(G0);
axis xy; colorbar;
xlabel('Frame'); ylabel('Banda');
title('MAP por banda G0(k,m) [dB]');

figure;
imagesc(G);
axis xy; colorbar;
xlabel('Frame'); ylabel('Banda');
title('ARF por banda G(k,m) [dB]');

kb = [3 9 15];
figure; hold on; grid on;
plot(G(kb(1),:), 'LineWidth',1.2);
plot(G(kb(2),:), 'LineWidth',1.2);
plot(G(kb(3),:), 'LineWidth',1.2);
legend("B3","B9","B15");
title('Tres ganancias por banda');

figure; hold on; grid on;
plot(InMin_k, 'o-');
plot(InMax_k, 'o-');
xlabel('Banda');
ylabel('dB');
legend('InMin_k','InMax_k');
title('Calibracion MAP por banda');

%% ESCUCHA
soundsc(Y(1:L), Fs);