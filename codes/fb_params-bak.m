function [type,freq,Q,gain] = fb_params(fmin,fmax,K)
%#codegen

  Kmax = 32;
  alfa = 0.9; % 0.8, 1.0, 1.2
  
  % K acotado e integer-valued
  K = min(max(floor(double(K)),1), Kmax);

  % Robustez numérica
  fmin_hz = double(fmin);
  fmax_hz = double(fmax);

  if fmin_hz <= 0
      fmin_hz = 1.0;
  end
  if fmax_hz <= fmin_hz
      fmax_hz = 2.0 * fmin_hz;
  end

  % Salidas de tamaño fijo
  % type: 1=LP, 2=HP, 3=BP
  type = repmat(uint8(3), K, 1);
  freq = zeros(K,1);
  Q    = zeros(K,1);
  gain = zeros(K,1);   % ganancia por banda en dB

  % Parámetros logarítmicos
  if K == 1
      r = 1.0;
      Qconst = 1.0;
  else
      r = (fmax_hz/fmin_hz)^(1.0/(K-1));
      Qconst = alfa* sqrt(r)/(r-1.0);
  end

  % Inicializa bandas activas como BP con ganancia 0 dB
  for k = 1:K
      Fc = fmin_hz * r^(k-1);
      freq(k) = Fc;       % BP: frecuencia central
      Q(k)    = Qconst;
      gain(k) = 0.0;      % 0 dB
  end

  % Ajuste de extremos: 1 LP + (K-2) BP + 1 HP
  if K >= 2
      Fc1 = fmin_hz;
      FcK = fmin_hz * r^(K-1);   % ~= fmax

      type(1) = uint8(1);     % LP
      freq(1) = Fc1 * sqrt(r);% LP: frecuencia de corte
      Q(1)    = 1.0/sqrt(2.0);

      type(K) = uint8(2);     % HP
      freq(K) = FcK / sqrt(r);% HP: frecuencia de corte
      Q(K)    = 1.0/sqrt(2.0);
  end
end