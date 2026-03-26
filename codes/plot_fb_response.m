function plot_fb_response(Fs, fmin, fmax, K, nStages)
% plot_fb_response  Grafica la respuesta del banco de filtros
%
% Ejemplo:
%   plot_fb_response(48000, 50, 5080, 16, 1)   % 1 biquad
%   plot_fb_response(48000, 50, 5080, 16, 2)   % 2 biquads en cascada

if nargin < 5
    nStages = 1;
end

[type,freq,Q,gain] = fb_params(fmin,fmax,K);

Nfft = 8192;
f = linspace(0, Fs/2, Nfft/2+1);
Hsum = zeros(size(f));

figure; hold on; grid on;

for k = 1:K
    fk = freq(k);

    switch type(k)
        case 1  % LP
            [b,a] = butter(2, fk/(Fs/2), 'low');

        case 2  % HP
            [b,a] = butter(2, fk/(Fs/2), 'high');

        case 3  % BP
            w0 = fk/(Fs/2);
            bw = w0 / max(Q(k), 1e-6);
            bw = min(max(bw, 1e-6), 0.999);
            [b,a] = iirpeak(w0, bw);

        otherwise
            error('Tipo de filtro no válido');
    end

    H = freqz(b, a, Nfft, Fs);
    Hm = abs(H(1:Nfft/2+1));

    % Cascada de nStages secciones idénticas
    HmC = Hm.^nStages;

    Hsum = Hsum + HmC.^2;   % suma de potencias
    semilogx(f, 20*log10(HmC + 1e-12));
end

semilogx(f, 20*log10(sqrt(Hsum) + 1e-12), 'k', 'LineWidth', 2);

xlabel('Frecuencia (Hz)');
ylabel('Magnitud (dB)');
title(sprintf('Banco de filtros: K=%d, [%g, %g] Hz, etapas=%d', K, fmin, fmax, nStages));
xlim([max(20,fmin/2), min(Fs/2, fmax*2)]);
ylim([-80 10]);

legendStrings = arrayfun(@(k) sprintf('Banda %d',k), 1:K, 'UniformOutput', false);
legendStrings{end+1} = 'Suma RMS';
legend(legendStrings{:}, 'Location', 'bestoutside');