function [inMin, inMax, outMin, outMax] = ctrl_params(fmin, fmax, K)
%#codegen

  Kmax = 32;

  % Plantilla maestra sobre todo el banco (ajústala a tu gusto)
  InMin32 = [...
    -84 -82 -80 -78 -76 -74 -72 -70 ...
    -68 -66 -64 -62 -60 -59 -58 -57 ...
    -56 -55 -54 -53 -52 -52 -53 -54 ...
    -55 -56 -57 -58 -59 -60 -61 -62].';

  InMax32 = [...
    -32 -31 -30 -28 -26 -24 -22 -20 ...
    -18 -17 -16 -15 -14 -13 -12 -12 ...
    -12 -12 -13 -14 -15 -16 -17 -18 ...
    -19 -20 -21 -22 -23 -24 -25 -26].';

  outMin = -45 * ones(Kmax,1);
  outMax =   0 * ones(Kmax,1);

  % Bandas reales del banco
  [~, freq, ~, ~] = fb_params(fmin, fmax, K);
  freq = freq(1:double(K));

  % Posición normalizada de bandas reales
  u = (log(freq) - log(freq(1))) / (log(freq(end)) - log(freq(1)) + eps);

  % Posición normalizada de la plantilla maestra
  u32 = linspace(0,1,Kmax).';

  % Interpolación
  inMinK  = interp1(u32, InMin32, u, 'linear', 'extrap');
  inMaxK  = interp1(u32, InMax32, u, 'linear', 'extrap');
  outMinK = interp1(u32, outMin,  u, 'linear', 'extrap');
  outMaxK = interp1(u32, outMax,  u, 'linear', 'extrap');

  % Salidas de tamaño fijo Kmax x 1
  inMin  = zeros(Kmax,1);
  inMax  = zeros(Kmax,1);
  outMin = zeros(Kmax,1);
  outMax = zeros(Kmax,1);

  inMin(1:double(K))  = inMinK;
  inMax(1:double(K))  = inMaxK;
  outMin(1:double(K)) = outMinK;
  outMax(1:double(K)) = outMaxK;
end