Fs = 48000;
Fk = [100 200 400 800 1600 3200 6400];   % o tus 6 bandas
Qk = 1.41*ones(size(Fk));

% Ejemplo didáctico: una banda sube y otra baja
Gk = zeros(size(Fk));   % dB
Gk(Fk==800)  = +3;
Gk(Fk==3200) = -3;

N = 8192;
f = linspace(20, Fs/2, N).';             % Hz (columna)
w = 2*pi*f/Fs;                            % rad/muestra (columna)

K = numel(Fk);
Hk_dB   = zeros(K, N);
Hacc_dB = zeros(K, N);

Hacc = ones(size(w));

for k = 1:K
    wc = 2*pi*Fk(k)/Fs;
    [b,a] = CenterShelving(wc, Gk(k), Qk(k)); % tu peaking (biquad)
    Hk = freqz(b,a,w);

    Hk_dB(k,:) = 20*log10(abs(Hk)+eps);

    Hacc = Hacc .* Hk;
    Hacc_dB(k,:) = 20*log10(abs(Hacc)+eps);
end

Htot_dB = Hacc_dB(end,:);

% --- Gráfica didáctica ---
figure; hold on; grid on;

% (1) Aporte de cada banda (solo las no-cero para no saturar)
plotted_idx = [];
for k = 1:K
    if abs(Gk(k)) > 1e-12
        plot(f, Hk_dB(k,:), 'LineWidth', 1.2);
        plotted_idx(end+1) = k; %#ok<AGROW>
    end
end

% (2) Total (cascada)
plot(f, Htot_dB, 'k', 'LineWidth', 2.0);

set(gca,'XScale','log'); xlim([20 Fs/2]);
xlabel('Frecuencia (Hz)');
ylabel('Magnitud (dB)');
title('EQ en cascada: aportes por banda y respuesta total');

% --- Leyenda robusta (sin compose) ---
labels = arrayfun(@(k) sprintf('H_{%d} (F=%g Hz, G=%+g dB)', k, Fk(k), Gk(k)), ...
                  plotted_idx, 'UniformOutput', false);
labels{end+1} = 'H_{tot}=\prod_k H_k';

legend(labels, 'Location','best', 'Interpreter','tex');
