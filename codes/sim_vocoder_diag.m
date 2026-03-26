%% sim_vocoder_diag.m
% Diagnóstico offline del vocoder.
% Evalúa si hay modulación espectral por bandas o solo modulación de amplitud.
%
% Señales de prueba:
%   - Voz SINTÉTICA con formantes reales (no requiere voice.wav)
%     Secuencia: /a/ (0-2 s), silencio (2-3 s), /e/ (3-5 s),
%               silencio (5-6 s), /i/ (6-8 s), silencio (8-9 s),
%               /u/ (9-11 s), silencio (11-12 s)
%   - Portadoras: ruido blanco Y sierra a 440 Hz
%
% Qué mirar en los resultados:
%   - eMean (heatmap ENV): ¿hay bandas con colores distintos según la vocal?
%     Si todas las franjas verticales son del mismo color → la modulación
%     espectral no llega a la cadena de control.
%   - gMean (heatmap ganancia): ¿las ganancias por banda difieren >8-10 dB?
%   - Espectros de salida: ¿los picos coinciden con los formantes esperados?
%   - Métrica "gStd": std de ganancias entre bandas en segmentos activos.
%     < 4 dB  → prácticamente solo gate/amplitud
%     4–8 dB  → modulación espectral presente pero débil
%     > 8 dB  → modulación espectral audible

%clear; clc; close all;

%% ------------------------------------------------------------------ %%
%%  PARÁMETROS                                                          %%
%% ------------------------------------------------------------------ %%
Fs  = 48000;
N   = 128;
K   = 16;
fmin = 160;
fmax = 7000;

envMode         = 2;      % 2 = RMS exponencial
mapMode         = 7;      % 7 = threshold+slope (vocoder natural)
mapThreshold_dB = -45;    % umbral: bandas por debajo quedan silenciadas
mapSlope        = 2.0;    % dB de ganancia por dB sobre el umbral
mapOutMin_dB    = -60;    % ganancia mínima del VCA (silencio)
mapOutMax_dB    =  0;     % ganancia máxima del VCA
attack_s        = 0.005;
release_s       = 0.030;
ctrlOffset_dB = 0;
outputTrim_dB = 0;

%% ------------------------------------------------------------------ %%
%%  SEÑALES DE PRUEBA                                                   %%
%% ------------------------------------------------------------------ %%
dur  = 12;
t    = (0:1/Fs:dur-1/Fs).';
Lsig = floor(dur*Fs/N)*N;
t    = t(1:Lsig);

voice  = synthVowelVoice(t, Fs);   % voz sintética con 4 vocales
noise  = 0.3  * randn(size(t));    % ruido blanco
saw440 = 0.25 * sawtooth(2*pi*440*t, 1.0);

nFrames = Lsig / N;

%% ------------------------------------------------------------------ %%
%%  PARÁMETROS DE BANDA                                                 %%
%% ------------------------------------------------------------------ %%
[type_b, freq_b, Q_b, gain_b, outMin_b, outMax_b] = ...
    vocoder_params(fmin, fmax, K);

type_b  = double(type_b(1:K));
freq_b  = double(freq_b(1:K));
gain_b  = double(gain_b(1:K));
outMin_b= double(outMin_b(1:K));
outMax_b= double(outMax_b(1:K));
Q_base  = double(Q_b(1:K));   % Q de fb_params (actualmente QRef de fb_params.m)

fprintf('QRef de fb_params: %.1f\n', Q_base(1));

% Sobrescribir outMin/outMax con rango vocoder
outMin_b = repmat(mapOutMin_dB, K, 1);
outMax_b = repmat(mapOutMax_dB, K, 1);

%% ------------------------------------------------------------------ %%
%%  CONFIGURACIONES A PROBAR                                            %%
%% ------------------------------------------------------------------ %%
% Modifica Q_values para probar más valores rápidamente.
Q_values  = [3.2, 5.0, Q_base(1)];   % Q_base = lo que venga de fb_params
carriers  = {noise, saw440};
carr_names = {'ruido', 'sierra440'};

nQ   = numel(Q_values);
nCar = numel(carriers);

results(nQ, nCar) = struct('gMean',[], 'eMean',[], 'yWet',[], ...
                           'gStd',0,  'gRange',0, 'eLevel',0);

% Ventanas temporales dentro de cada vocal (evita transiciones)
% /a/: 0-2 s, /e/: 3-5 s, /i/: 6-8 s, /u/: 9-11 s
vowelNames   = {'a','e','i','u'};
vowelWindows = [0.3 1.8; 3.3 4.8; 6.3 7.8; 9.3 10.8];   % segundos
vowelFormants= {[800 1200 2700], [400 2200 2800], ...
                [280 2700 3100], [500  700 2500]};

fprintf('\n===== RESULTADOS =====\n');
fprintf('%-22s | ENV_medio | gStd  | gRange | diagnóstico\n', 'Configuración');
fprintf('%s\n', repmat('-',1,75));

for qi = 1:nQ
    Q_use = repmat(Q_values(qi), K, 1);

    for ci = 1:nCar
        [yWet, gMean, eMean] = runVocoder(voice, carriers{ci}, Fs, N, K, ...
            type_b, freq_b, Q_use, gain_b, ...
            outMin_b, outMax_b, ...
            envMode, mapMode, mapThreshold_dB, mapSlope, ...
            attack_s, release_s, ctrlOffset_dB, outputTrim_dB);

        % Métrica: std de ganancias ENTRE BANDAS durante segmentos vocales
        gStd_all = [];
        for vi = 1:4
            tw   = vowelWindows(vi,:);
            mIdx = round(tw * Fs/N);
            mIdx = max(1,mIdx(1)) : min(nFrames, mIdx(2));
            if numel(mIdx) > 2
                gStd_all(end+1) = mean(std(gMean(mIdx,:), 0, 2)); %#ok
            end
        end
        gStd   = mean(gStd_all);
        gRange = mean(max(gMean,[],2) - min(gMean,[],2));
        eLevel = mean(eMean(:));

        if gStd > 8
            diag_str = 'MODULACION ESPECTRAL OK';
        elseif gStd > 4
            diag_str = 'modulacion debil';
        else
            diag_str = 'solo gate/amplitud';
        end

        cfg_name = sprintf('Q=%.1f, %s', Q_values(qi), carr_names{ci});
        fprintf('%-22s | %7.1f dB | %5.1f | %6.1f  | %s\n', ...
            cfg_name, eLevel, gStd, gRange, diag_str);

        results(qi,ci).gMean  = gMean;
        results(qi,ci).eMean  = eMean;
        results(qi,ci).yWet   = yWet;
        results(qi,ci).gStd   = gStd;
        results(qi,ci).gRange = gRange;
        results(qi,ci).eLevel = eLevel;

        % Guardar audio (normalizado al pico para facilitar la escucha)
        yNorm = yWet / (max(abs(yWet)) + 1e-6) * 0.9;
        fname = sprintf('voc_Q%.0f_%s.wav', Q_values(qi)*10, carr_names{ci});
        audiowrite(fname, [yNorm yNorm], Fs);
    end
end

%% ------------------------------------------------------------------ %%
%%  FIGURA 1: ENV por banda — diagnóstico clave                         %%
%  Si todas las franjas verticales son del mismo color →              %%
%  el análisis por banda no ve diferencia espectral en la voz.        %%
%% ------------------------------------------------------------------ %%
tt_frames = (0:nFrames-1)*N/Fs;

figure('Name','Fig1 – ENV por banda (heatmap)', 'Color','w', ...
       'Position',[100 600 1200 400]);

for qi = 1:min(nQ,3)
    subplot(1,3,qi);
    imagesc(tt_frames, 1:K, results(qi,1).eMean.');   % carrier=ruido
    axis xy;  colorbar;  clim([-80 -10]);
    xlabel('Tiempo (s)');  ylabel('Banda');
    title(sprintf('ENV  Q=%.1f, ruido', Q_values(qi)));
    % Marcar transiciones vocales
    for vi = 1:4
        xline(vowelWindows(vi,1),'w--');
        xline(vowelWindows(vi,2),'w--');
    end
end
sgtitle({'Fig 1 – ENV por banda (dBFS)'; ...
    'Bandas más activas (color más claro) deben cambiar de posición al cambiar vocal'; ...
    'Si todas las bandas tienen el mismo color → no hay modulación espectral'});

%% ------------------------------------------------------------------ %%
%%  FIGURA 2: Ganancia por banda                                        %%
%% ------------------------------------------------------------------ %%
figure('Name','Fig2 – Ganancia VCA por banda', 'Color','w', ...
       'Position',[100 150 1200 400]);

for qi = 1:min(nQ,3)
    subplot(1,3,qi);
    imagesc(tt_frames, 1:K, results(qi,1).gMean.');
    axis xy;  colorbar;  clim([-60 0]);
    xlabel('Tiempo (s)');  ylabel('Banda');
    title(sprintf('Ganancia Q=%.1f, ruido (gStd=%.1fdB)', ...
          Q_values(qi), results(qi,1).gStd));
end
sgtitle('Fig 2 – Ganancia VCA por banda (dB)');

%% ------------------------------------------------------------------ %%
%%  FIGURA 3: Espectro de salida por vocal                              %%
%%  (mejor config = la que tenga mayor gStd)                           %%
%% ------------------------------------------------------------------ %%
[~, best_qi] = max([results(:,1).gStd]);
best_ci = 1;   % ruido

figure('Name','Fig3 – Espectro de salida por vocal', 'Color','w', ...
       'Position',[100 100 900 600]);

for vi = 1:4
    subplot(2,2,vi);
    tw   = vowelWindows(vi,:);
    idx  = round(tw*Fs);
    idx  = max(1,idx(1)) : min(Lsig,idx(2));
    [P, F] = pwelch(results(best_qi,best_ci).yWet(idx), ...
                    hanning(2048), 1024, 4096, Fs);
    semilogx(F, 10*log10(P+1e-12), 'b');
    hold on;
    for fi = 1:numel(vowelFormants{vi})
        xline(vowelFormants{vi}(fi), 'r--', ...
              sprintf('F%d',fi), 'LabelVerticalAlignment','bottom');
    end
    xlim([100 8000]);  grid on;
    xlabel('Hz');  ylabel('dB');
    title(sprintf('/%s/  (Q=%.1f, ruido)', vowelNames{vi}, Q_values(best_qi)));
end
sgtitle({'Fig 3 – Espectros de salida por vocal'; ...
    'Los picos del espectro deben coincidir con las líneas rojas (formantes esperados)'});

%% ------------------------------------------------------------------ %%
%%  FIGURA 4: Sierra vs Ruido para la mejor Q                          %%
%% ------------------------------------------------------------------ %%
figure('Name','Fig4 – Ruido vs Sierra, vocal /a/', 'Color','w', ...
       'Position',[100 100 700 400]);

vi = 1;   % /a/
tw = vowelWindows(vi,:);
idx = round(tw*Fs);
idx = max(1,idx(1)) : min(Lsig,idx(2));

for ci = 1:nCar
    [P, F] = pwelch(results(best_qi,ci).yWet(idx), hanning(2048),1024,4096,Fs);
    semilogx(F, 10*log10(P+1e-12), 'DisplayName', carr_names{ci});  hold on;
end
for fi = 1:numel(vowelFormants{vi})
    xline(vowelFormants{vi}(fi),'r--');
end
xlim([100 8000]);  grid on;  legend;
xlabel('Hz');  ylabel('dB');
title(sprintf('Vocal /a/ — comparativa portadoras (Q=%.1f)', Q_values(best_qi)));

%% ------------------------------------------------------------------ %%
%%  FIGURA 5: señal de voz sintética de referencia                      %%
%% ------------------------------------------------------------------ %%
figure('Name','Fig5 – Voz sintética y espectrograma', 'Color','w');
subplot(2,1,1);
plot((0:Lsig-1)/Fs, voice);  grid on;
xlabel('Tiempo (s)');  ylabel('Amplitud');
title('Voz moduladora sintética');
for vi = 1:4
    xline(vowelWindows(vi,1),'r--');
    xline(vowelWindows(vi,2),'r--');
    text(mean(vowelWindows(vi,:)), 0.5, ['/' vowelNames{vi} '/'], ...
         'HorizontalAlignment','center','Color','r','FontWeight','bold');
end

subplot(2,1,2);
spectrogram(voice, hanning(round(0.04*Fs)), round(0.02*Fs), 1024, Fs, 'yaxis');
ylim([0 5]);  title('Espectrograma voz moduladora');

%% ================================================================== %%
%%  FUNCIÓN: runVocoder                                                 %%
%% ================================================================== %%
function [yWet, gBandMean, eBandMean] = runVocoder(xL, xR, Fs, N, K, ...
        type, freq, Q, gain, outMin, outMax, ...
        envMode, mapMode, mapThreshold, mapSlope, ...
        attack_s, release_s, ctrlOffset_dB, outputTrim_dB)

    Lsig = floor(min(numel(xL),numel(xR))/N)*N;
    xL = xL(1:Lsig);  xR = xR(1:Lsig);
    nFrames = Lsig / N;

    yWet      = zeros(Lsig,1);
    gBandMean = zeros(nFrames,K);
    eBandMean = zeros(nFrames,K);

    VCF = cell(K,1); ENV = cell(K,1); MAP = cell(K,1);
    ARF = cell(K,1); VCA = cell(K,1);

    for k = 1:K
        VCF{k} = vcf_sys;      VCF{k}.N = N; VCF{k}.Fs = Fs; VCF{k}.bypass = uint8(0);
        ENV{k} = env_mono_sys; ENV{k}.N = N; ENV{k}.Fs = Fs;
        MAP{k} = map_sys;      MAP{k}.bypass = uint8(0); MAP{k}.Mode = uint8(mapMode);
        MAP{k}.Threshold = mapThreshold;
        MAP{k}.Slope     = mapSlope;
        ARF{k} = arf_sys;      ARF{k}.N = N; ARF{k}.Fs = Fs; ARF{k}.bypass = uint8(0);
        VCA{k} = vca_mono_sys; VCA{k}.N = N; VCA{k}.Fs = Fs;
        VCA{k}.bypass = uint8(0); VCA{k}.Mode = uint8(1);
    end

    for m = 1:nFrames
        idx   = (m-1)*N + (1:N);
        inMod = xL(idx);
        inPor = xR(idx);
        yBands = zeros(N,K);

        for k = 1:K
            [modBand, porBand] = VCF{k}(inMod, inPor, type(k), freq(k), Q(k), gain(k));
            [env_dB, ~]        = ENV{k}(modBand, envMode);
            g0_dB              = MAP{k}(env_dB, outMin(k), outMax(k));
            g_dB               = ARF{k}(g0_dB, attack_s, release_s) + ctrlOffset_dB;
            yBands(:,k)        = VCA{k}(porBand, g_dB);
            gBandMean(m,k)     = mean(g_dB);
            eBandMean(m,k)     = mean(env_dB);
        end

        yWet(idx) = sum(yBands, 2);
    end

    yWet = db2mag(outputTrim_dB) * yWet;
end

%% ================================================================== %%
%%  FUNCIÓN: synthVowelVoice                                            %%
%%  Genera 4 vocales sintéticas con formantes reales.                  %%
%%  Fundamental f0=120 Hz. Nivel pico ≈ 0.45 (−7 dBFS).              %%
%% ================================================================== %%
function x = synthVowelVoice(t, Fs)
% Parámetros de cada vocal: [F1 F2 F3 F4] Hz, BW (Hz), amplitudes relativas
    vowels = { ...
        struct('name','a','onset',0, 'dur',2, ...
               'F',[800 1200 2700 3800], 'BW',[80 100 150 200], 'A',[1.0 0.8 0.5 0.3]); ...
        struct('name','e','onset',3, 'dur',2, ...
               'F',[400 2200 2800 3600], 'BW',[60 120 150 200], 'A',[1.0 0.9 0.5 0.3]); ...
        struct('name','i','onset',6, 'dur',2, ...
               'F',[280 2700 3100 3700], 'BW',[50 130 150 200], 'A',[1.0 0.9 0.6 0.3]); ...
        struct('name','u','onset',9, 'dur',2, ...
               'F',[500  700 2500 3500], 'BW',[70  80 150 200], 'A',[1.0 0.8 0.4 0.2]); ...
    };

    f0 = 120;   % pitch fundamental (Hz)
    x  = zeros(size(t));

    for vi = 1:numel(vowels)
        v    = vowels{vi};
        t0   = v.onset;
        t1   = t0 + v.dur;
        mask = (t >= t0) & (t < t1);
        if ~any(mask), continue; end
        tw   = t(mask) - t0;
        nSam = sum(mask);
        buzz = zeros(nSam,1);

        for h = f0 : f0 : min(9000, Fs/2 - f0)
            n  = h / f0;
            % Peso espectral: suma de resonancias de Lorentz por formante
            g  = 0;
            for fi = 1:numel(v.F)
                g = g + v.A(fi) / (1 + ((h - v.F(fi)) / v.BW(fi))^2);
            end
            % Pendiente espectral del glotal source: −6 dB/oct → /n^0.5
            buzz = buzz + (g / n^0.5) * sin(2*pi*h*tw);
        end

        % Envolvente suave en los 50 ms de ataque y caída
        fade = min(round(0.05*Fs), floor(nSam/4));
        ramp = sin(linspace(0, pi/2, fade)').^2;
        env  = [ramp; ones(nSam - 2*fade, 1); flipud(ramp)];
        buzz = buzz .* env(1:nSam);

        x(mask) = buzz;
    end

    % Normalizar a ≈ 0.45 de pico
    mx = max(abs(x));
    if mx > 0, x = x / mx * 0.45; end
end
