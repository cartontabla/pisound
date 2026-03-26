function R = diag_ctrl_voice_lego_simulink_faithful(wavFile)
% diag_ctrl_voice_lego_simulink_faithful
% Aproximación fiel al esquema Simulink:
%
%   VOZ -> FB(mod) -> ENV -> MAP -> ARF -> VCA(gain)
%   CARRIER -> FB(car) -----------------------> VCA -> suma
%
% La idea es reproducir:
% - procesado frame-based
% - estados persistentes por banda
% - mismos bloques LEGO reales
% - mismo VCA que en Simulink
%
% Requisitos:
%   fb_params.m
%   ctrl_params_from_freq.m
%   vcf_sys.m
%   env_mono_sys.m
%   map_sys.m
%   arf_sys.m
%   vca_mono_sys.m

%% ======================== CONFIGURACIÓN ================================
cfg = struct();

cfg.Fs_ref = 48000;
cfg.K      = int32(18);

cfg.Fmin = 100;
cfg.Fmax = min(6000, 0.45*cfg.Fs_ref);

cfg.plotSeconds = 10;
cfg.eps0 = 1e-12;

% ---------- frame ----------
cfg.NframeProc = 128;

% ---------- ENV ----------
cfg.envMode = uint8(1);      % usar exactamente el de Simulink
cfg.tauRMS  = 0.02;

% ---------- MAP ----------
cfg.mapBypass   = uint8(0);
cfg.mapMode     = uint8(8);  % poner aquí el real de Simulink
cfg.mapGain     = 1.0;
cfg.mapOffset   = 0.0;
cfg.mapFloorLin = 1e-6;
cfg.mapThreshold = -50.0;
cfg.mapSlope     = 1.0;

% ---------- ARF ----------
cfg.attack_s  = 0.005;
cfg.release_s = 0.050;
cfg.arfBypass = uint8(0);

% ---------- VCA ----------
cfg.vcaBypass          = uint8(0);
cfg.vcaMode            = uint8(1);   % 0=lineal, 1=dB
cfg.vcaForceGainEnable = uint8(0);
cfg.vcaForceGainValue  = 0.0;

% ---------- carrier ----------
cfg.carrierType = 'saw';     % 'saw' | 'noise' | 'buzz'
cfg.carrierF0   = 120;
cfg.carrierAmp  = 0.8;

%% ============================ AUDIO ====================================
[x, Fs] = audioread(wavFile);

if size(x,2) == 2
    xMono = mean(x,2);
else
    xMono = x;
end

xMono = double(xMono(:));
xMono(~isfinite(xMono)) = 0;
xMono = xMono - mean(xMono);

mx = max(abs(xMono));
if mx <= 0 || ~isfinite(mx)
    error('La señal de entrada es nula o no finita.');
end
xMono = xMono / mx;

if Fs ~= cfg.Fs_ref
    warning('El wav está a %g Hz y el banco está planteado para %g Hz.', Fs, cfg.Fs_ref);
end

N = length(xMono);
t = (0:N-1)'/Fs;

fprintf('Archivo: %s\n', wavFile);
fprintf('Fs = %d Hz\n', Fs);
fprintf('Canales = %d\n', size(x,2));
fprintf('Duración = %.3f s\n', N/Fs);

%% ========================= BANCO REAL ==================================
[type,freq,Q,gain] = fb_params(cfg.Fmin, cfg.Fmax, cfg.K);

Keff = min(double(cfg.K), numel(type));
type = type(1:Keff);
freq = double(freq(1:Keff));
Q    = double(Q(1:Keff));
gain = double(gain(1:Keff));

[outMinVec,outMaxVec] = ctrl_params_from_freq(freq, Keff);

%% ======================= PORTADORA INTERNA =============================
carrier = local_make_carrier(cfg.carrierType, N, Fs, cfg.carrierF0, cfg.carrierAmp);
carrier = carrier(:);
carrier = carrier - mean(carrier);
carrier = carrier / max(max(abs(carrier)), cfg.eps0);

%% ===================== RESERVA DE MEMORIA ==============================
nBlocks = ceil(N / cfg.NframeProc);

modBandSig   = zeros(N, Keff);
carBandSig   = zeros(N, Keff);

envLinBand   = zeros(N, Keff);
envdBBand    = zeros(N, Keff);
mapOutBand   = zeros(N, Keff);
arfOutBand   = zeros(N, Keff);
gainLinBand  = zeros(N, Keff);
vocBandSig   = zeros(N, Keff);

bandOK       = false(Keff,1);

ctrlVar_map  = zeros(Keff,1);
ctrlVar_arf  = zeros(Keff,1);
ctrlMean_dB  = zeros(Keff,1);
modRMS       = zeros(Keff,1);
carRMS       = zeros(Keff,1);
vocRMS       = zeros(Keff,1);

%% ===================== OBJETOS POR BANDA ===============================
% Esto se parece más al For Each: una instancia por banda y estados persistentes.
vcfModObjs = cell(Keff,1);
vcfCarObjs = cell(Keff,1);
envObjs    = cell(Keff,1);
mapObjs    = cell(Keff,1);
arfObjs    = cell(Keff,1);
vcaObjs    = cell(Keff,1);

for k = 1:Keff
    % --- filtro modulador ---
    vcfMod = vcf_sys;
    if isprop(vcfMod,'Fs'), vcfMod.Fs = Fs; end
    vcfModObjs{k} = vcfMod;

    % --- filtro carrier ---
    vcfCar = vcf_sys;
    if isprop(vcfCar,'Fs'), vcfCar.Fs = Fs; end
    vcfCarObjs{k} = vcfCar;

    % --- ENV ---
    envObj = env_mono_sys;
    if isprop(envObj,'Fs'), envObj.Fs = Fs; end
    if isprop(envObj,'N'),  envObj.N  = cfg.NframeProc; end
    if isprop(envObj,'TauRMS'), envObj.TauRMS = cfg.tauRMS; end
    envObjs{k} = envObj;

    % --- MAP ---
    mapObj = map_sys;
    if isprop(mapObj,'bypass'),    mapObj.bypass    = cfg.mapBypass; end
    if isprop(mapObj,'Mode'),      mapObj.Mode      = cfg.mapMode; end
    if isprop(mapObj,'Gain'),      mapObj.Gain      = cfg.mapGain; end
    if isprop(mapObj,'Offset'),    mapObj.Offset    = cfg.mapOffset; end
    if isprop(mapObj,'FloorLin'),  mapObj.FloorLin  = cfg.mapFloorLin; end
    if isprop(mapObj,'Threshold'), mapObj.Threshold = cfg.mapThreshold; end
    if isprop(mapObj,'Slope'),     mapObj.Slope     = cfg.mapSlope; end
    mapObjs{k} = mapObj;

    % --- ARF ---
    arfObj = arf_sys;
    if isprop(arfObj,'Fs'), arfObj.Fs = Fs; end
    if isprop(arfObj,'N'),  arfObj.N  = cfg.NframeProc; end
    if isprop(arfObj,'bypass'), arfObj.bypass = cfg.arfBypass; end
    arfObjs{k} = arfObj;

    % --- VCA ---
    vcaObj = vca_mono_sys;
    if isprop(vcaObj,'Fs'), vcaObj.Fs = Fs; end
    if isprop(vcaObj,'N'),  vcaObj.N  = cfg.NframeProc; end
    if isprop(vcaObj,'bypass'), vcaObj.bypass = cfg.vcaBypass; end
    if isprop(vcaObj,'Mode'),   vcaObj.Mode   = cfg.vcaMode; end
    if isprop(vcaObj,'ForceGainEnable')
        vcaObj.ForceGainEnable = cfg.vcaForceGainEnable;
    end
    if isprop(vcaObj,'ForceGainValue')
        vcaObj.ForceGainValue = cfg.vcaForceGainValue;
    end
    vcaObjs{k} = vcaObj;
end

fprintf('\n=== PROCESADO POR BANDAS ===\n');

for k = 1:Keff
    inMin_k  = inMinVec(k);
    inMax_k  = inMaxVec(k);
    outMin_k = outMinVec(k);
    outMax_k = outMaxVec(k);

    try
        vcfMod = vcfModObjs{k};
        vcfCar = vcfCarObjs{k};
        envObj = envObjs{k};
        mapObj = mapObjs{k};
        arfObj = arfObjs{k};
        vcaObj = vcaObjs{k};

        yMod = zeros(N,1);
        yCar = zeros(N,1);
        eLin = zeros(N,1);
        eDB  = zeros(N,1);
        mOut = zeros(N,1);
        aOut = zeros(N,1);
        gLin = zeros(N,1);
        yVoc = zeros(N,1);

        for b = 1:nBlocks
            i1 = (b-1)*cfg.NframeProc + 1;
            i2 = min(b*cfg.NframeProc, N);
            Lb = i2 - i1 + 1;

            xvb = zeros(cfg.NframeProc,1);
            xcb = zeros(cfg.NframeProc,1);
            xvb(1:Lb) = xMono(i1:i2);
            xcb(1:Lb) = carrier(i1:i2);

            inR0 = zeros(cfg.NframeProc,1);

            % --- banda de voz ---
            [modBlk, ~] = vcfMod(xvb, inR0, type(k), freq(k), Q(k), gain(k));

            % --- banda de carrier ---
            [carBlk, ~] = vcfCar(xcb, inR0, type(k), freq(k), Q(k), gain(k));

            % --- ENV ---
            [envdB_blk, envLin_blk] = envObj(modBlk, cfg.envMode);

            % --- MAP ---
            %mapBlk = mapObj(envdB_blk, inMin_k, inMax_k, outMin_k, outMax_k);
            mapBlk = envdB_blk + 40;

            % --- ARF ---
            arfBlk = arfObj(mapBlk, cfg.attack_s, cfg.release_s);

            % --- VCA REAL ---
            vocBlk = vcaObj(carBlk, arfBlk);

            % --- solo para diagnóstico visual ---
            if cfg.vcaMode == uint8(1)
                gainBlk = 10.^(arfBlk/20);
            else
                gainBlk = arfBlk;
            end

            yMod(i1:i2) = modBlk(1:Lb);
            yCar(i1:i2) = carBlk(1:Lb);
            eLin(i1:i2) = envLin_blk(1:Lb);
            eDB(i1:i2)  = envdB_blk(1:Lb);
            mOut(i1:i2) = mapBlk(1:Lb);
            aOut(i1:i2) = arfBlk(1:Lb);
            gLin(i1:i2) = gainBlk(1:Lb);
            yVoc(i1:i2) = vocBlk(1:Lb);
        end

        if any(~isfinite(yMod)) || any(~isfinite(yCar)) || ...
           any(~isfinite(eDB))  || any(~isfinite(mOut)) || ...
           any(~isfinite(aOut)) || any(~isfinite(gLin)) || ...
           any(~isfinite(yVoc))
            warning('Banda %d contiene valores no finitos.', k);
            continue;
        end

        modBandSig(:,k)  = yMod;
        carBandSig(:,k)  = yCar;
        envLinBand(:,k)  = eLin;
        envdBBand(:,k)   = eDB;
        mapOutBand(:,k)  = mOut;
        arfOutBand(:,k)  = aOut;
        gainLinBand(:,k) = gLin;
        vocBandSig(:,k)  = yVoc;

        bandOK(k) = true;

        ctrlVar_map(k) = std(mOut);
        ctrlVar_arf(k) = std(aOut);
        ctrlMean_dB(k) = mean(aOut);
        modRMS(k)      = sqrt(mean(yMod.^2));
        carRMS(k)      = sqrt(mean(yCar.^2));
        vocRMS(k)      = sqrt(mean(yVoc.^2));

        fprintf('B%02d ok | type=%d f=%.1f Hz | RMSmod=%.4f | RMSvoc=%.4f | meanCtrl=%.2f dB\n', ...
            k, type(k), freq(k), modRMS(k), vocRMS(k), ctrlMean_dB(k));

    catch ME
        warning('Falló banda %d: %s', k, ME.message);
    end
end

%% release explícito
for k = 1:Keff
    try, release(vcfModObjs{k}); end
    try, release(vcfCarObjs{k}); end
    try, release(envObjs{k}); end
    try, release(mapObjs{k}); end
    try, release(arfObjs{k}); end
    try, release(vcaObjs{k}); end
end

fprintf('\nBandas válidas: %d de %d\n', sum(bandOK), Keff);

modBandSig(:,~bandOK)  = 0;
carBandSig(:,~bandOK)  = 0;
envLinBand(:,~bandOK)  = 0;
envdBBand(:,~bandOK)   = -120;
mapOutBand(:,~bandOK)  = -120;
arfOutBand(:,~bandOK)  = -120;
gainLinBand(:,~bandOK) = 0;
vocBandSig(:,~bandOK)  = 0;

%% ======================== RECONSTRUCCIÓN ===============================
yVoc = sum(vocBandSig, 2);
yVoc = yVoc - mean(yVoc);
mxVoc = max(abs(yVoc));
if mxVoc > cfg.eps0
    yVocN = yVoc / mxVoc;
else
    yVocN = yVoc;
end

sumEnvLin = sum(envLinBand, 2);
sumGainLin = sum(gainLinBand, 2);
sumCarAbs = sum(abs(carBandSig), 2);

[domEnvVal, domEnvIdx] = max(envLinBand, [], 2); %#ok<ASGLU>
domEnvFreq = freq(domEnvIdx).';

bandCentroid = (envLinBand * freq(:)) ./ (sumEnvLin + cfg.eps0);
ctrlPostMapCentroid = ((10.^(arfOutBand/20)) * freq(:)) ./ (sumGainLin + cfg.eps0);

envGlobalAbs = abs(xMono);

c_env_sumenv = safe_corr(envGlobalAbs, sumEnvLin);
c_map_arf    = safe_corr(mapOutBand(:), arfOutBand(:));
c_env_gain   = safe_corr(sumEnvLin, sumGainLin);
c_centroids  = safe_corr(bandCentroid, ctrlPostMapCentroid);
c_in_out     = safe_corr(abs(xMono), abs(yVocN));

fprintf('\nCorrelaciones diagnósticas:\n');
fprintf('corr(abs(voz), sumEnvLin)          = %.4f\n', c_env_sumenv);
fprintf('corr(MAP, ARF)                     = %.4f\n', c_map_arf);
fprintf('corr(sumEnvLin, sumGainLin)        = %.4f\n', c_env_gain);
fprintf('corr(centroide ENV, centroide G)   = %.4f\n', c_centroids);
fprintf('corr(abs(voz), abs(yVoc))          = %.4f\n', c_in_out);

%% ====================== GRÁFICAS GENERALES =============================
tmax = min(cfg.plotSeconds, N/Fs);
idxT = (t <= tmax);

[~, ord] = sort(modRMS, 'descend');
showBands = ord(1:min(6, Keff));
showBands = showBands(bandOK(showBands));

figure('Name','Diagnóstico vocoder CTRL real - Resumen','Color','w');

subplot(4,2,1);
plot(t(idxT), xMono(idxT), 'k'); hold on;
plot(t(idxT), carrier(idxT), 'Color', [0.4 0.4 1.0]);
plot(t(idxT), yVocN(idxT), 'r');
grid on;
xlabel('Tiempo (s)');
ylabel('Amp');
title('Entrada voz / portadora / salida vocoder');
legend('voz','carrier','yVoc');

subplot(4,2,2);
plot(t(idxT), envGlobalAbs(idxT), 'k'); hold on;
plot(t(idxT), sumEnvLin(idxT), 'r');
plot(t(idxT), sumGainLin(idxT), 'b');
grid on;
xlabel('Tiempo (s)');
ylabel('Magnitud');
title('Magnitudes globales');
legend('|voz|','\Sigma ENV','\Sigma G_{lin}');

subplot(4,2,3);
imagesc(t(idxT), 1:Keff, envdBBand(idxT,:)');
axis xy;
xlabel('Tiempo (s)');
ylabel('Banda');
title('ENV dB por banda');
colorbar;

subplot(4,2,4);
imagesc(t(idxT), 1:Keff, mapOutBand(idxT,:)');
axis xy;
xlabel('Tiempo (s)');
ylabel('Banda');
title('MAP out por banda (dB)');
colorbar;

subplot(4,2,5);
imagesc(t(idxT), 1:Keff, arfOutBand(idxT,:)');
axis xy;
xlabel('Tiempo (s)');
ylabel('Banda');
title('ARF out por banda (dB)');
colorbar;

subplot(4,2,6);
imagesc(t(idxT), 1:Keff, 20*log10(gainLinBand(idxT,:)' + cfg.eps0));
axis xy;
xlabel('Tiempo (s)');
ylabel('Banda');
title('Ganancia final por banda (dB)');
colorbar;

subplot(4,2,7);
plot(t(idxT), domEnvFreq(idxT), 'LineWidth', 1.0); hold on;
plot(t(idxT), bandCentroid(idxT), 'LineWidth', 1.0);
plot(t(idxT), ctrlPostMapCentroid(idxT), 'LineWidth', 1.0);
grid on;
xlabel('Tiempo (s)');
ylabel('Hz');
title('Frecuencias representativas');
legend('banda dom ENV','centroide ENV','centroide G');

subplot(4,2,8);
spectrogram(yVocN, hann(round(0.03*Fs)), round(0.02*Fs), 1024, Fs, 'yaxis');
title('Espectrograma salida vocoder');

sgtitle('Diagnóstico del vocoder completo con bloques LEGO reales');

%% ==================== GRÁFICAS POR BANDA ===============================
figure('Name','Bandas dominantes: MOD / ENV / MAP / ARF / GAIN / VOC','Color','w');

nShow = numel(showBands);
for ii = 1:nShow
    k = showBands(ii);

    subplot(nShow, 6, (ii-1)*6 + 1);
    plot(t(idxT), modBandSig(idxT,k), 'k'); grid on;
    if ii == 1, title(sprintf('MOD B%d', k)); end
    ylabel(sprintf('B%d',k));

    subplot(nShow, 6, (ii-1)*6 + 2);
    plot(t(idxT), envdBBand(idxT,k), 'b'); grid on;
    if ii == 1, title('ENV dB'); end

    subplot(nShow, 6, (ii-1)*6 + 3);
    plot(t(idxT), mapOutBand(idxT,k), 'm'); grid on;
    if ii == 1, title('MAP dB'); end

    subplot(nShow, 6, (ii-1)*6 + 4);
    plot(t(idxT), arfOutBand(idxT,k), 'r'); grid on;
    if ii == 1, title('ARF dB'); end

    subplot(nShow, 6, (ii-1)*6 + 5);
    plot(t(idxT), gainLinBand(idxT,k), 'g'); grid on;
    if ii == 1, title('Gain lin'); end

    subplot(nShow, 6, (ii-1)*6 + 6);
    plot(t(idxT), vocBandSig(idxT,k), 'Color', [0.2 0.2 0.2]); grid on;
    if ii == 1, title('VOC band'); end
end

%% ============================= SALIDA ==================================
R = struct();
R.cfg = cfg;
R.wavFile = wavFile;
R.Fs = Fs;
R.t = t;

R.xMono = xMono;
R.carrier = carrier;
R.yVoc = yVoc;
R.yVocN = yVocN;

R.type = type;
R.freq = freq;
R.Q = Q;
R.gain = gain;
R.bandOK = bandOK;

R.modBandSig = modBandSig;
R.carBandSig = carBandSig;
R.envLinBand = envLinBand;
R.envdBBand  = envdBBand;
R.mapOutBand = mapOutBand;
R.arfOutBand = arfOutBand;
R.gainLinBand = gainLinBand;
R.vocBandSig = vocBandSig;

R.sumEnvLin = sumEnvLin;
R.sumGainLin = sumGainLin;
R.sumCarAbs = sumCarAbs;

R.bandCentroid = bandCentroid;
R.ctrlPostMapCentroid = ctrlPostMapCentroid;

R.ctrlVar_map = ctrlVar_map;
R.ctrlVar_arf = ctrlVar_arf;
R.ctrlMean_dB = ctrlMean_dB;
R.modRMS = modRMS;
R.carRMS = carRMS;
R.vocRMS = vocRMS;

R.inMinVec  = inMinVec;
R.inMaxVec  = inMaxVec;
R.outMinVec = outMinVec;
R.outMaxVec = outMaxVec;

R.corr_env_sumenv = c_env_sumenv;
R.corr_map_arf = c_map_arf;
R.corr_env_gain = c_env_gain;
R.corr_centroids = c_centroids;
R.corr_in_out = c_in_out;

[outPath, outName] = fileparts(wavFile);
if isempty(outPath), outPath = pwd; end
outFile = fullfile(outPath, [outName '_diag_vocoder_simulink_faithful.wav']);
audiowrite(outFile, yVocN, Fs);
R.outFile = outFile;

fprintf('\nArchivo de salida escrito en:\n%s\n', outFile);

end

% ========================================================================
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

% ========================================================================
function x = local_make_carrier(kind, N, Fs, f0, amp)
t = (0:N-1)'/Fs;

switch lower(kind)
    case 'saw'
        ph = mod(f0*t, 1.0);
        x = 2*ph - 1;

    case 'noise'
        rng(0);
        x = randn(N,1);

    case 'buzz'
        x = zeros(N,1);
        nH = max(1, floor((Fs/2) / max(f0,1)));
        nH = min(nH, 40);
        for h = 1:nH
            x = x + (1/h) * sin(2*pi*h*f0*t);
        end

    otherwise
        ph = mod(f0*t, 1.0);
        x = 2*ph - 1;
end

x = amp * x;
mx = max(abs(x));
if mx > 0
    x = x / mx;
end
end