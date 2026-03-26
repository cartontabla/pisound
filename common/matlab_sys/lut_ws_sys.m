classdef lut_ws_sys < matlab.System
% lut_ws_sys  Waveshaper por LUT/funciones (audio -> audio), frame-based, estéreo.
%
% Entradas:
%   inL, inR : [N x 1] double   audio normalizado (|x|<=1 recomendado)
%   mode     : scalar numeric   1=HardClip, 2=SoftClip, 3=Tanh, 4=AsymClip,
%                               5=Chebyshev, 6=UserLUT
%   amount   : scalar double    (dB) drive interno (0..40 dB típico)
%   asym     : scalar double    bias (offset) antes del shaper (-0.5..0.5 típico)
%   cheb3    : scalar double    (solo modo 5) peso T3
%   cheb5    : scalar double    (solo modo 5) peso T5
%
% Salidas:
%   outL,outR: [N x 1] double   audio distorsionado
%
% Notas:
% - Esto es un shaper "estático" (memoria cero). Puede aliasing con drive alto.
% - Para modo 6 (UserLUT) se usa una LUT fija XUser/YUser en [-1,1] (amplitud).
%

  properties(Nontunable)
    N = 128;
    M = 257;                % puntos de LUT usuario (tamaño fijo)
    bypass (1,1) uint8 = 0;  % 0=fx, 1=no fx
    OS (1,1) uint8 = 0; % 0=no OS, 1=×2, 2=x4
    % Halfband FIR (15 taps), Fc = Fs/4 (para ×2), ganancia DC=1
    HB15 = [-1.06360599e-04, 0, 6.95210045e-03, 0, -5.16884606e-02, 0, ...
           2.94825487e-01, 5.00034468e-01, 2.94825487e-01, 0, ...
          -5.16884606e-02, 0, 6.95210045e-03, 0, -1.06360599e-04];
  end

  properties
    % Defaults (útiles si entradas llegan vacías)
    ModeDefault   = uint8(3);   % tanh
    AmountDefault = 12.0;       % dB
    AsymDefault   = 0.0;        % bias

    % Parámetros Chebyshev por defecto (modo 5)
    Cheb3Default  = 1.0;
    Cheb5Default  = 0.0;

    % User LUT (modo 6): mapea amplitud -> amplitud
    % XUser estrictamente creciente en [-1,1]
    XUser = linspace(-1, 1, 257);  % [1 x M]
    YUser = linspace(-1, 1, 257);  % [1 x M]
    MUser = 257;                    % puntos válidos (<= M)

    % Seguridad numérica
    Eps = 1e-12;
  end

  properties(DiscreteState)
    zUp1L
    zUp1R
    zDn1L
    zDn1R
    zUp2L
    zUp2R
    zDn2L
    zDn2R
  end

  methods(Access = protected)

    function setupImpl(obj)
      Mh = numel(obj.HB15);
      z0 = zeros(Mh-1,1);
      obj.zUp1L = z0; obj.zUp1R = z0;
      obj.zDn1L = z0; obj.zDn1R = z0;
      obj.zUp2L = z0; obj.zUp2R = z0;
      obj.zDn2L = z0; obj.zDn2R = z0;
    end

    function resetImpl(obj)
      Mh = numel(obj.HB15);
      z0 = zeros(Mh-1,1);
      obj.zUp1L = z0; obj.zUp1R = z0;
      obj.zDn1L = z0; obj.zDn1R = z0;
      obj.zUp2L = z0; obj.zUp2R = z0;
      obj.zDn2L = z0; obj.zDn2R = z0;
    end

    function [outL,outR] = stepImpl(obj, inL, inR, mode, amount, asym, cheb3, cheb5)
      % Nloc = size(inL,1);
      % outL = zeros(Nloc,1);
      % outR = zeros(Nloc,1);
      if size(inL,1) ~= obj.N || size(inR,1) ~= obj.N
        error('lut_ws_sys: input frame size must be N=%d.', obj.N);
      end

      bp = (obj.bypass ~= 0);   % bp es logical
      if bp
        outL = inL; outR = inR; return;
      end
      
      outL = zeros(obj.N,1);
      outR = zeros(obj.N,1);

      % --- defaults ---
      if isempty(mode),   mode   = obj.ModeDefault;   end
      if isempty(amount), amount = obj.AmountDefault; end
      if isempty(asym),   asym   = obj.AsymDefault;   end
      if isempty(cheb3),  cheb3  = obj.Cheb3Default;  end
      if isempty(cheb5),  cheb5  = obj.Cheb5Default;  end

      md = uint8(min(max(round(double(mode)),1),6));
      A_dB  = double(amount);
      b     = double(asym);

      A_dB = clamp(obj, A_dB, 0.0, 60.0);   % por ejemplo
      b = clamp(obj, b,   -0.5, 0.5);
      % drive interno (lineal)
      drive = 10.0^(A_dB/20.0);

      % preparar LUT usuario (si aplica)
      % (tamaño fijo M, relleno seguro)
      xGrid = zeros(1,obj.M);
      yGrid = zeros(1,obj.M);
      if md == uint8(6)
        Mu = int32(min(max(obj.MUser,1), obj.M));
        for k = 1:Mu
          xGrid(k) = obj.XUser(k);
          yGrid(k) = obj.YUser(k);
        end
        for k = (Mu+1):obj.M
          xGrid(k) = xGrid(Mu);
          yGrid(k) = yGrid(Mu);
        end
      end
      
      os = uint8(min(max(obj.OS,0),2));

      
      [outL, obj.zUp1L, obj.zUp2L, obj.zDn1L, obj.zDn2L] = processChannelOS( ...
      obj, inL, os, md, drive, b, cheb3, cheb5, xGrid, yGrid, ...
      obj.zUp1L, obj.zUp2L, obj.zDn1L, obj.zDn2L);

      [outR, obj.zUp1R, obj.zUp2R, obj.zDn1R, obj.zDn2R] = processChannelOS( ...
      obj, inR, os, md, drive, b, cheb3, cheb5, xGrid, yGrid, ...
      obj.zUp1R, obj.zUp2R, obj.zDn1R, obj.zDn2R);
    end

    function [y, zUp1, zUp2, zDn1, zDn2] = processChannelOS( ...
      obj, x, os, md, drive, b, cheb3, cheb5, xGrid, yGrid, zUp1, zUp2, zDn1, zDn2)
    
      if os==0
        y = zeros(obj.N,1);
        for n=1:obj.N
          y(n)=shapeOne(obj,x(n),md,drive,b,cheb3,cheb5,xGrid,yGrid);
        end
    
      elseif os==1
        [x2, zUp1] = up2_fir(obj, x,  obj.HB15, zUp1);      % 2N
        y2 = zeros(2*obj.N,1);
        for n=1:(2*obj.N)
          y2(n)=shapeOne(obj,x2(n),md,drive,b,cheb3,cheb5,xGrid,yGrid);
        end
        [y,  zDn1] = fir_down2(obj, y2, obj.HB15, zDn1);    % N
    
      else
        [x2, zUp1] = up2_fir(obj, x,  obj.HB15, zUp1);      % 2N
        [x4, zUp2] = up2_fir(obj, x2, obj.HB15, zUp2);      % 4N
    
        y4 = zeros(4*obj.N,1);
        for n=1:(4*obj.N)
          y4(n)=shapeOne(obj,x4(n),md,drive,b,cheb3,cheb5,xGrid,yGrid);
        end
    
        [y2, zDn2] = fir_down2(obj, y4, obj.HB15, zDn2);    % 2N
        [y,  zDn1] = fir_down2(obj, y2, obj.HB15, zDn1);    % N
      end
    end

    % SHAPER (una muestra)
    function y = shapeOne(obj, x, md, drive, b, cheb3, cheb5, xGrid, yGrid)

      % pre: drive + bias
      u = drive * x + b;
      if ~isfinite(u) u = 0.0; end

      % limitar a rango razonable antes de funciones no lineales
      %u = clamp(obj, u, -1.0, 1.0);

      if md == uint8(1)
        % ---- Hard clip simétrico ----
        y = clamp(obj, u, -1.0, 1.0);

      elseif md == uint8(2)
        % ---- Soft clip polinómico (tipo 3er orden, continuo) ----
        u = clamp(obj, u, -2.0, 2.0);
        % y = u - u^3/3 para |u|<=1  (suave, sin discontinuidad)
        % (si quieres más agresivo, usa otro polinomio)
        y = u - (u*u*u)/3.0;

      elseif md == uint8(3)
        % ---- Tanh (soft saturación) ----
        % Normalizamos ligeramente para que tanh(1) no sea demasiado bajo
        y = tanh(2.0*u) / tanh(2.0);

      elseif md == uint8(4)
        % ---- Asym clip (bias ya aplicado) + hard clip ----
        % Asimetría sale de b; aquí solo clippeamos.
        %y = clamp(obj, u, -1.0, 1.0);
        thP = 1.0;
        thN = 0.6;          % recorta antes en negativo
        y = clamp(obj, u, -thN, thP);
        y = y / max(thP, thN);   % renormaliza

      elseif md == uint8(5)
        % ---- Chebyshev controlado (T1 + a3*T3 + a5*T5) ----
        % Para x en [-1,1]:
        % T1=u
        % T3=4u^3-3u
        % T5=16u^5-20u^3+5u
        uCheb = clamp(obj, u, -1.0, 1.0);
        u2 = uCheb*uCheb;
        u3 = u2*uCheb;
        u5 = u3*u2;

        T1 = uCheb;
        T3 = 4.0*u3 - 3.0*uCheb;
        T5 = 16.0*u5 - 20.0*u3 + 5.0*uCheb;

        y  = T1 + double(cheb3)*T3 + double(cheb5)*T5;

        % re-limit (Chebyshev puede exceder)
        y  = clamp(obj, y, -1.0, 1.0);

      else
        % ---- User LUT: y = interp(u; XUser,YUser) ----
        y = interp1_lut(obj, u, xGrid, yGrid);
      end

      % post: opcionalmente quitar bias DC introducido (simple, estático)
      % En audio real, quitar DC es mejor con HPF muy suave; aquí hacemos corrección estática:
      %y = y - b;

      if ~isfinite(y) y = 0.0; end
      % limitar salida final
      y = clamp(obj, y, -1.0, 1.0);
    end

    function [y2, zf] = up2_fir(obj, x, h, zi)
      Lx = numel(x);
      u  = zeros(2*Lx,1);
      u(1:2:end) = x;
      [y2, zf] = filter(h, 1, u, zi);
    end
    
    function [y, zf] = fir_down2(obj, x2, h, zi)
      [xf, zf] = filter(h, 1, x2, zi);
      y = xf(1:2:end);
    end

    % Interpolación lineal LUT (saturación)
    function y = interp1_lut(obj, x, xGrid, yGrid)

      if x <= xGrid(1)
        y = yGrid(1); return;
      end
      if x >= xGrid(obj.M)
        y = yGrid(obj.M); return;
      end

      k = 1;
      for i = 1:(obj.M-1)
        if x < xGrid(i+1)
          k = i;
          break;
        end
      end

      x0 = xGrid(k);  x1 = xGrid(k+1);
      y0 = yGrid(k);  y1 = yGrid(k+1);

      den = (x1 - x0);
      if den <= obj.Eps || ~isfinite(den)
        y = y0;
        return;
      else
        t = (x - x0)/den;
        y = y0 + (y1 - y0)*t;
      end
    end

    function y = clamp(~, x, lo, hi)
      if x < lo
        y = lo;
      elseif x > hi
        y = hi;
      else
        y = x;
      end
    end

    % Validaciones
    function validateInputsImpl(obj, inL, inR, mode, amount, asym, cheb3, cheb5)
      validateattributes(inL, {'double'}, {'size',[obj.N 1], 'real', 'finite'});
      validateattributes(inR, {'double'}, {'size',[obj.N 1], 'real', 'finite'});
      validateattributes(mode,   {'numeric'}, {'scalar','real','finite'});
      validateattributes(amount, {'numeric'}, {'scalar','real','finite'});
      validateattributes(asym,   {'numeric'}, {'scalar','real','finite'});
      validateattributes(cheb3,  {'numeric'}, {'scalar','real','finite'});
      validateattributes(cheb5,  {'numeric'}, {'scalar','real','finite'});
    end

    function [sz, dt, cp] = getDiscreteStateSpecificationImpl(obj, name)
      Mh = numel(obj.HB15);
      zLen = Mh - 1;                 % longitud del estado FIR
    
      switch name
        case {'zUp1L','zDn1L','zUp2L','zDn2L','zUp1R','zDn1R','zUp2R','zDn2R'}
          sz = [zLen 1];
          dt = 'double';
          cp = false;
        otherwise
          % Si hubiera otros estados discretos, decláralos aquí
          sz = [0 0];
          dt = 'double';
          cp = false;
      end
    end
    function n = getNumInputsImpl(~),  n = 7; end   % inL,inR,mode,amount,asym,cheb3,cheb5
    function n = getNumOutputsImpl(~), n = 2; end
    
    function [sz1,sz2,sz3,sz4,sz5,sz6,sz7] = getInputSizeImpl(obj)
      sz1 = [obj.N 1];   % inL
      sz2 = [obj.N 1];   % inR
      sz3 = [1 1];       % mode
      sz4 = [1 1];       % amount
      sz5 = [1 1];       % asym
      sz6 = [1 1];       % cheb3
      sz7 = [1 1];       % cheb5
    end
    
    function [dt1,dt2,dt3,dt4,dt5,dt6,dt7] = getInputDataTypeImpl(~)
      dt1='double'; dt2='double'; dt3='double'; dt4='double'; 
      dt5='double'; dt6='double'; dt7='double';
    end
    
    function [c1,c2,c3,c4,c5,c6,c7] = isInputComplexImpl(~)
      c1=false; c2=false; c3=false; c4=false; 
      c5=false; c6=false; c7=false;
    end
    
    function [f1,f2,f3,f4,f5,f6,f7] = isInputFixedSizeImpl(~)
      f1=true; f2=true; f3=true; f4=true; 
      f5=true; f6=true; f7=true;
    end
    function [o1,o2] = getOutputSizeImpl(obj)
      o1 = [obj.N 1]; o2 = [obj.N 1];
    end

    function [o1,o2] = getOutputDataTypeImpl(~)
      o1 = 'double'; o2 = 'double';
    end

    function [c1,c2] = isOutputComplexImpl(~)
      c1 = false; c2 = false;
    end

    function [f1,f2] = isOutputFixedSizeImpl(~)
      f1 = true; f2 = true;
    end

  end
end