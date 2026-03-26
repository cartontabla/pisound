%clear; clc;

[file, path] = uigetfile('*.wav');
if isequal(file,0), return; end
[x, FsFile] = audioread(fullfile(path,file));

if size(x,2) > 1
    x = mean(x,2);   % mono
end

Fs = 48000;
N  = 128;

if FsFile ~= Fs
    x = resample(x, Fs, FsFile);
end

ENV = env_mono_sys;
ENV.N  = N;
ENV.Fs = Fs;

L = numel(x);
nF = ceil(L/N);

xpad = zeros(nF*N,1);
xpad(1:L) = x;

X = reshape(xpad, N, []);
nF = size(X,2);

Lenv = zeros(N,nF);

for k = 1:nF
    Lenv(:,k) = step(ENV, X(:,k), 2);
end

Lenvv = Lenv(:);

p01 = prctile(Lenvv,1);
p05 = prctile(Lenvv,5);
p10 = prctile(Lenvv,10);
p50 = prctile(Lenvv,50);
p90 = prctile(Lenvv,90);
p95 = prctile(Lenvv,95);
p99 = prctile(Lenvv,99);

fprintf('\n=== RANGO ENV(2) CON TU VOZ ===\n');
fprintf('min = %.2f dB\n', min(Lenvv));
fprintf('p01 = %.2f dB\n', p01);
fprintf('p05 = %.2f dB\n', p05);
fprintf('p10 = %.2f dB\n', p10);
fprintf('p50 = %.2f dB\n', p50);
fprintf('p90 = %.2f dB\n', p90);
fprintf('p95 = %.2f dB\n', p95);
fprintf('p99 = %.2f dB\n', p99);
fprintf('max = %.2f dB\n', max(Lenvv));

figure; histogram(Lenvv,120); grid on;
title('Histograma de Lenv');
xlabel('Nivel (dB)'); ylabel('Cuenta');

figure; plot(Lenvv); grid on;
title('Lenv con voz real');
xlabel('Muestra'); ylabel('dB');