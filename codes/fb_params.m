function [type,freq,Q,gain] = fb_params(fmin,fmax,K)
%#codegen

  Kmax = 32;

  FcRef = [ ...
      50; 68; 91; 123; 166; 224; 302; 407; ...
      548; 738; 995; 1340; 1800; 2420; 3250; 4360 ];

  GRef = [ ...
      0.0; 0.0; 0.0; 0.0; ...
      0.5; 0.5; 1.0; 1.0; ...
      1.5; 1.5; 2.0; 2.5; ...
      3.0; 3.5; 4.0; 4.5 ];

  QRef = 7; %5 3.2;

  K = coder.const(int32(min(max(floor(double(K)),1), Kmax)));

  fmin_hz = double(fmin);
  fmax_hz = double(fmax);

  if fmin_hz <= 0
      fmin_hz = FcRef(1);
  end
  if fmax_hz <= fmin_hz
      fmax_hz = FcRef(end);
  end

  type = zeros(K,1,'uint8');
  freq = zeros(K,1);
  Q    = zeros(K,1);
  gain = zeros(K,1);

  logRefMin = log(FcRef(1));
  logRefMax = log(FcRef(end));
  logOutMin = log(fmin_hz);
  logOutMax = log(fmax_hz);

  FcRefScaled = zeros(16,1);
  for i = 1:16
      t = (log(FcRef(i)) - logRefMin) / (logRefMax - logRefMin);
      FcRefScaled(i) = exp(logOutMin + t * (logOutMax - logOutMin));
  end

  if K == 1
      type(1) = uint8(2);           % HP única si solo hay una banda
      freq(1) = sqrt(fmin_hz*fmax_hz);
      Q(1)    = 1.0/sqrt(2.0);
      gain(1) = interp1_local_log(FcRefScaled, GRef, freq(1));
      return;
  end

  for k = 1:K
      tk = double(k-1) / double(K-1);
      fk = exp(logOutMin + tk * (logOutMax - logOutMin));

      type(k) = uint8(3);           % BP por defecto
      freq(k) = fk;
      Q(k)    = QRef;
      gain(k) = interp1_local_log(FcRefScaled, GRef, fk);
  end

  % Última banda como HP moderada
  r = exp((logOutMax - logOutMin)/double(K-1));
  type(K) = uint8(2);               % HP
  freq(K) = freq(K) / sqrt(r);      % corte algo por debajo del último centro
  Q(K)    = 1.0/sqrt(2.0);          % Q típica de HP suave
  gain(K) = gain(K) + 2.0;          % pequeño refuerzo opcional

end

function y = interp1_local_log(xRef, yRef, xq)
%#codegen
  n = length(xRef);
  if xq <= xRef(1)
      y = yRef(1);
      return;
  end
  if xq >= xRef(n)
      y = yRef(n);
      return;
  end

  lxq = log(xq);
  y = yRef(1);

  for i = 1:n-1
      if xq >= xRef(i) && xq <= xRef(i+1)
          lx1 = log(xRef(i));
          lx2 = log(xRef(i+1));
          t = (lxq - lx1)/(lx2 - lx1);
          y = yRef(i) + t*(yRef(i+1)-yRef(i));
          return;
      end
  end
end