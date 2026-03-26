Fs = 48000;

% --- EQ paramétrica por zonas (6 bandas) ---
Type = {'LS','PK','PK','PK','PK','HS'};

% Rangos sugeridos (guía)
Fmin = [ 20   60   200   600   2000   5000];
Fmax = [ 60  200   600  2000   5000  12000];

% Frecuencias centrales iniciales (movibles dentro del rango)
F0 = [ 40  120   350  1200   3500   9000];

% Q solo aplica a peaking (en shelves queda fijo internamente)
Q0 = [ NaN  1.0   1.1   1.2   1.6   NaN];

% Ganancias ejemplo (dB)
G0 = zeros(1,6);
G0(3) = +3;   % medias-bajas
G0(5) = -3;   % presencia

% --- Forzar f0 dentro de rango (por seguridad) ---
F0 = max(Fmin, min(F0, Fmax));

% --- Mallado de frecuencia ---
N = 8192;
f = linspace(20, Fs/2, N).';   % Hz (columna)
w = 2*pi*f/Fs;                  % rad/muestra

K = numel(F0);
Hk_dB   = zeros(K, N);
Hacc_dB = zeros(K, N);

Hacc = ones(size(w));

for k = 1:K
    wc = 2*pi*F0(k)/Fs;

    switch upper(Type{k})
        case 'LS'
            [b,a] = LowShelving(wc, G0(k));

        case 'HS'
            [b,a] = HighShelving(wc, G0(k));

        case 'PK'
            [b,a] = CenterShelving(wc, G0(k), Q0(k)); % tu peaking

        otherwise
            error('Tipo no reconocido: %s', Type{k});
    end

    Hk = freqz(b,a,w);
    Hk_dB(k,:) = 20*log10(abs(Hk)+eps);

    Hacc = Hacc .* Hk;
    Hacc_dB(k,:) = 20*log10(abs(Hacc)+eps);
end

Htot_dB = Hacc_dB(end,:);

% --- Gráfica didáctica ---
figure; hold on; grid on;

plotted_idx = [];
for k = 1:K
    if abs(G0(k)) > 1e-12
        plot(f, Hk_dB(k,:), 'LineWidth', 1.2);
        plotted_idx(end+1) = k; %#ok<AGROW>
    end
end

plot(f, Htot_dB, 'k', 'LineWidth', 2.0);

set(gca,'XScale','log'); xlim([20 Fs/2]);
xlabel('Frecuencia (Hz)');
ylabel('Magnitud (dB)');
title('EQ paramétrica en cascada: aportes por banda y respuesta total');

% --- Leyenda ---
labels = cell(0,1);
for k = plotted_idx
    if strcmpi(Type{k},'PK')
        labels{end+1,1} = sprintf('%s: f_0=%g Hz, Q=%.2f, G=%+g dB', ...
                                  Type{k}, F0(k), Q0(k), G0(k));
    else
        labels{end+1,1} = sprintf('%s: f_0=%g Hz, G=%+g dB', ...
                                  Type{k}, F0(k), G0(k));
    end
end
labels{end+1,1} = 'H_{tot}=\prod_k H_k';
legend(labels, 'Location','best', 'Interpreter','tex');
