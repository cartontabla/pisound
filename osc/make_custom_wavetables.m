function make_custom_wavetables(folder, OutMaxL)
% Genera custom_wavetables.m a partir de WAVs en 'folder'.
% Si no hay WAVs, genera un stub (silencio).
% Firma generada: [wtPad, len] = custom_wavetables(OutMaxL)
%
% Supuesto simple: cada WAV ya representa 1 ciclo (o el fragmento que quieras usar).

if nargin < 2, OutMaxL = 4096; end

files = dir(fullfile(folder, '*.wav'));
K = min(numel(files), 5);

fid = fopen('custom_wavetables.m','w');
if fid < 0, error("No puedo escribir custom_wavetables.m"); end
cleanupObj = onCleanup(@() fclose(fid));

% Cabecera común
fprintf(fid, "function [wtPad, len] = custom_wavetables(OutMaxL)\n");
fprintf(fid, "%%#codegen\n");
fprintf(fid, "OutMaxL = coder.const(OutMaxL);\n");
fprintf(fid, "wtPad = zeros(OutMaxL, 5);\n");
fprintf(fid, "len   = 2*ones(1,5);\n\n");

% Caso vacío -> stub
if K == 0
    fprintf(fid, "%% Stub: sin wavetables custom (silencio)\n");
    fprintf(fid, "end\n");
    return
end

% Construcción en MATLAB (offline) para embebido
wtPad = zeros(OutMaxL, 5);
len   = 2*ones(1, 5);

for k = 1:K
    [x, ~] = audioread(fullfile(files(k).folder, files(k).name));
    x = mean(x, 2);           % mono
    x = x - mean(x);          % quitar DC

    n = numel(x);
    if n < 2
        continue
    end

    % Longitud propia (capada por OutMaxL), mínimo 2
    Lk = min(max(2, n), OutMaxL);

    % Remuestreo uniforme a Lk puntos en [0,1)
    t  = (0:n-1)'/n;
    ti = (0:Lk-1)'/Lk;
    xi = interp1(t, x, ti, 'linear', 'extrap');

    % Normalizar a [-1,1]
    xi = xi / (max(abs(xi)) + eps);

    wtPad(1:Lk, k) = xi;
    len(k) = Lk;
end

fprintf(fid, "%% Datos embebidos (generado desde WAV)\n");
for k = 1:5
    Lk = len(k);
    v  = wtPad(1:Lk, k);

    fprintf(fid, "v = %s;\n", mat2str(v, 17));
    fprintf(fid, "Lk = min(%d, OutMaxL);\n", Lk);
    fprintf(fid, "len(%d) = max(2, Lk);\n", k);
    fprintf(fid, "wtPad(1:Lk,%d) = v(1:Lk);\n\n", k);
end

fprintf(fid, "end\n");
end
