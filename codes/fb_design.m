% pilot_fb_18.m  (params + volcado + gráfica)
%clear; clc;

Fs = 48000;
K  = int32(18);

Fmin = 100;
Fmax = min(6000, 0.45*Fs);

% --- Mapeo de type (AJUSTA si tu vcf_sys usa otro orden) ---
TYPE_LP = uint8(1);
TYPE_HP = uint8(2);
TYPE_BP = uint8(3);

[type,freq,Q,gain] = fb_params(Fmin, Fmax, K);

Keff = min(double(K), numel(type));
type = type(1:Keff);
freq = freq(1:Keff);
Q    = Q(1:Keff);
gain = gain(1:Keff);

% ========= VOLCADO =========
fprintf('\n=== FB PARAMS ===\n');
fprintf('Fs=%g Hz | K=%d | Fmin=%g Hz | Fmax=%g Hz\n', Fs, Keff, Fmin, Fmax);

u = unique(type(:)).';
fprintf('Types presentes: '); fprintf('%d ', u); fprintf('\n');
for tt = u
  fprintf('  type=%d -> %d bandas\n', tt, sum(type==tt));
end

typeName = strings(Keff,1);
for k = 1:Keff
  if type(k)==TYPE_LP, typeName(k)="LP";
  elseif type(k)==TYPE_HP, typeName(k)="HP";
  elseif type(k)==TYPE_BP, typeName(k)="BP";
  else, typeName(k)="??";
  end
end

T = table((1:Keff).', type, typeName, freq, Q, gain, ...
  'VariableNames', {'k','type','name','freq_Hz','Q','gain_dB'});
disp(T);

nLP = sum(type==TYPE_LP);
nHP = sum(type==TYPE_HP);
nBP = sum(type==TYPE_BP);
if ~(nLP==1 && nHP==1 && nBP==(Keff-2))
  warning('Distribución inesperada: LP=%d, BP=%d, HP=%d. Revisa TYPE_* o fb_params.', nLP, nBP, nHP);
end

% ========= RESPUESTAS EN FRECUENCIA (impulso -> FFT) =========
Nframe = 128;     % ajusta a tu frame
L      = 32768;   % longitud respuesta impulsional
nF     = ceil(L/Nframe);
nfft   = 65536;

imp = zeros(L,1); imp(1)=1;

H = zeros(nfft/2+1, Keff);
for k = 1:Keff
  obj = vcf_sys;
  if isprop(obj,'Fs'); obj.Fs = Fs; end

  h = zeros(L,1);
  for m = 1:nF
    i1 = (m-1)*Nframe + 1;
    i2 = min(m*Nframe, L);

    inL = zeros(Nframe,1); inR = zeros(Nframe,1);
    inL(1:(i2-i1+1)) = imp(i1:i2);

    [outL,~] = obj(inL, inR, type(k), freq(k), Q(k), gain(k));
    h(i1:i2) = outL(1:(i2-i1+1));
  end
  release(obj);

  Hk = fft(h, nfft);
  H(:,k) = Hk(1:nfft/2+1);
end

f = (0:nfft/2)'*(Fs/nfft);
magBands_dB = 20*log10(abs(H)+1e-12);

% Total "real" (suma compleja)
Hsum = sum(H,2);
Hcov = sqrt(sum(abs(H).^2,2));

magSum_dB = 20*log10(abs(Hsum)+1e-12);
magCov_dB = 20*log10(abs(Hcov)+1e-12);

% Cobertura en negro grueso (recomendado)
semilogx(f, magCov_dB, 'k', 'LineWidth', 3.0);

% Suma compleja en negro punteado (solo informativa)
semilogx(f, magSum_dB, 'k:', 'LineWidth', 1.8);

% ========= GRÁFICA =========
idxLP = find(type==TYPE_LP);
idxHP = find(type==TYPE_HP);
idxBP = find(type==TYPE_BP);

figure; hold on; grid on;
C = turbo(Keff);

% BP en color
for ii = 1:numel(idxBP)
  k = idxBP(ii);
  semilogx(f, magBands_dB(:,k), 'Color', C(k,:), 'LineWidth', 1.2);
end

% LP/HP resaltados
for k = idxLP(:).'
  semilogx(f, magBands_dB(:,k), 'k--', 'LineWidth', 2.2);
end
for k = idxHP(:).'
  semilogx(f, magBands_dB(:,k), 'k--', 'LineWidth', 2.2);
end

% Total en negro (elige UNA)
semilogx(f, magSum_dB, 'k', 'LineWidth', 2.8);     % suma compleja (reconstrucción)
% semilogx(f, magCov_dB, 'k', 'LineWidth', 2.8);   % cobertura (más legible)

xlim([80 8000]);  % o [Fmin/1.2, min(Fs/2, Fmax*1.4)]
for k = 1:Keff
  xline(freq(k), ':', 'HandleVisibility','off');
end
ylim([-80 12]);
xlabel('Frecuencia (Hz)');
ylabel('Magnitud (dB)');
title(sprintf('Banco de filtros: BP (color) + LP/HP (negro--), K=%d', Keff));

% legend opcional (suele saturar). Si quieres, activa:
% legend([compose('Banda %d',1:Keff), "Total"], 'Location','southwest');