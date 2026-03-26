function [outMin,outMax] = ctrl_params_from_freq(freq,K)
%#codegen

  Kmax = 32;
  tiny = 1e-12;

  K = floor(double(K));
  if K < 1
    K = 1;
  end
  if K > Kmax
    K = Kmax;
  end

  % Salidas activas: Kx1
  outMin = zeros(K,1);
  outMax = zeros(K,1);

  % Plantilla maestra: 32 puntos
  OutMin32 = -60 * ones(Kmax,1);
  OutMax32 = zeros(Kmax,1);

  % Posición normalizada de la plantilla maestra
  u32 = linspace(0.0,1.0,Kmax).';

  % Frecuencias activas
  f = freq(1:K);

  % Posición normalizada logarítmica de las bandas reales
  u = zeros(K,1);

  if K == 1
    u(1) = 0.5;
  else
    f1 = max(double(f(1)),1.0);
    fK = max(double(f(K)),f1 + tiny);

    den = log(fK) - log(f1);

    if abs(den) < tiny
      for k = 1:K
        u(k) = double(k-1)/double(K-1);
      end
    else
      for k = 1:K
        fk = max(double(f(k)),1.0);
        u(k) = (log(fk) - log(f1)) / den;
      end
    end
  end

  % Interpolación desde la plantilla de 32 puntos hacia K bandas
  for k = 1:K
    outMin(k) = interp_lin_table(u(k), u32, OutMin32);
    outMax(k) = interp_lin_table(u(k), u32, OutMax32);
  end
end

function y = interp_lin_table(x, xp, yp)
%#codegen

  N = numel(xp);

  if x <= xp(1)
    y = yp(1);
    return;
  end

  if x >= xp(N)
    y = yp(N);
    return;
  end

  y = yp(1);

  for i = 1:(N-1)
    x1 = xp(i);
    x2 = xp(i+1);

    if x >= x1 && x <= x2
      t = (x - x1)/(x2 - x1);
      y = yp(i) + t*(yp(i+1) - yp(i));
      return;
    end
  end
end