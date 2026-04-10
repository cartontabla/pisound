% lut_gc_sys.m
% lut_gc_sys  Curva estática de ganancia por LUT (Look-Up Table), frame-based, sin estado interno.
%
% Uso típico en dyn:
%   ENV -> LUT (este bloque) -> ARF -> VCA
%
% Entradas:
%   Lin_dB    : [N x 1] double   nivel en dBFS (salida de ENV)
%   mode      : scalar numeric   1=comp, 2=limit, 3=exp/gate suave, 4=libre (user LUT)
%   threshold : scalar double    (dB)
%   ratio     : scalar double    (convención como GC: >0 arriba, <0 abajo; aquí se usa |ratio|=R)
%   knee      : scalar double    (dB)  (en este LUT básico no se usa; queda por compatibilidad)
%   makeup    : scalar double    (dB)
%
% Salida:
%   GdB0      : [N x 1] double   ganancia en dB (instantánea) + makeup
%
% Nota:
% - La LUT implementa y = f(x) con x=L_in(dB) y y=G_dB0(dB).
% - Modos 1..3 generan automáticamente la LUT (sobre una rejilla fija Xmin..Xmax).
% - Modo 4 usa la LUT definida en propiedades XUser/YUser (longitud MUser).
%
classdef lut_gc_sys < matlab.System

  properties(Nontunable)
    N    = 128;
    Fs   = 48000; 
    M    = 257;      % nº de puntos de la LUT interna (fija, codegen-friendly)
  end

  properties
    % Rejilla interna (dB)
    Xmin = -80;
    Xmax =  10;

    % Para modo 3 (exp/gate suave): suelo de ganancia en dB
    Floor_dB = -80;

    % LUT de usuario (modo 4)
    % IMPORTANTE: para codegen conviene tamaño fijo. Aquí se fija a M.
    XUser = linspace(-80, 10, 257);   % [1 x M]
    YUser = zeros(1,257);             % [1 x M] (ganancia en dB)
    MUser = 257;                      % nº de puntos válidos (<= M)
  end

  methods(Access = protected)

    function setupImpl(obj) %#ok<MANU>
      % Sin estado
    end

    function resetImpl(obj) %#ok<MANU>
      % Sin estado
    end


    function GdB0 = stepImpl(obj,Lin_dB,mode,threshold,ratio,knee,makeup) 
      Nloc = size(Lin_dB,1);
      GdB0 = zeros(Nloc,1);

      % --- parámetros ---
      if isempty(mode), mode = 1; end
      md = uint8(mode);

      T = double(threshold);
      r = double(ratio);
      if r == 0, r = 1.0; end
      R = abs(r);
      if isfinite(R) && (R < 1), R = 1.0; end

      Mup = double(makeup);

      % --- construir LUT (xGrid, yGrid) ---
      % xGrid: [1 x M]
      xGrid = linspace(double(obj.Xmin), double(obj.Xmax), double(obj.M));
      yGrid = zeros(1, obj.M);

      if md == uint8(1)
        % ===== MODO 1: Compresor descendente (hard knee) =====
        % Lout = x si x<=T ; Lout = T + (x-T)/R si x>T
        for k = 1:obj.M
          x = xGrid(k);
          if x <= T
            Lout = x;
          else
            if isinf(R)
              Lout = T;          % caso extremo: limitador ideal
            else
              Lout = T + (x - T)/R;
            end
          end
          yGrid(k) = Lout - x;   % GdB0
        end

      elseif md == uint8(2)
        % ===== MODO 2: Limitador (techo) =====
        for k = 1:obj.M
          x = xGrid(k);
          Lout = min(x, T);
          yGrid(k) = Lout - x;
        end

      elseif md == uint8(3)
        % ===== MODO 3: Expansor / "gate" suave (hard knee) =====
        % Identidad arriba del umbral; pendiente R (>1) por debajo
        % Lout = x si x>=T ; Lout = T + R*(x-T) si x<T
        for k = 1:obj.M
          x = xGrid(k);
          if x >= T
            Lout = x;
          else
            if isinf(R)
              % "gate" ideal aproximado: ganancia al suelo
              % (aquí se modela como GdB = Floor_dB)
              Lout = x + obj.Floor_dB;
            else
              Lout = T + R*(x - T);
            end
          end
          G = Lout - x;
          % imponer suelo de ganancia (en dB)
          G = max(G, obj.Floor_dB);
          yGrid(k) = G;
        end

      else
        % ===== MODO 4: Libre (LUT de usuario) =====
        % Usa XUser/YUser con MUser puntos válidos
        Mu = int32(min(max(obj.MUser,1), obj.M));
        for k = 1:Mu
          xGrid(k) = obj.XUser(k);
          yGrid(k) = obj.YUser(k);
        end
        % Si Mu < M, rellenar el resto repitiendo el último punto (evita NaN)
        for k = (Mu+1):obj.M
          xGrid(k) = xGrid(Mu);
          yGrid(k) = yGrid(Mu);
        end
      end

      % --- aplicar LUT por interpolación lineal a Lin_dB ---
      for n = 1:Nloc
        x = Lin_dB(n);
        y = interp1_lut(obj, x, xGrid, yGrid);   % ganancia en dB
        GdB0(n) = y + Mup;                       % + makeup
      end
    end

    % ---------- Interpolación lineal con saturación ----------
    function y = interp1_lut(obj, x, xGrid, yGrid)
      % Saturación fuera de rango
      if x <= xGrid(1)
        y = yGrid(1);
        return;
      end
      if x >= xGrid(obj.M)
        y = yGrid(obj.M);
        return;
      end

      % Búsqueda lineal (M pequeño; codegen-friendly)
      k = int32(1);
      for i = 1:(obj.M-1)
        if x < xGrid(i+1)
          k = int32(i);
          break;
        end
      end

      x0 = xGrid(k);
      x1 = xGrid(k+1);
      y0 = yGrid(k);
      y1 = yGrid(k+1);

      % Evitar división por cero si la LUT de usuario 
      % no es estrictamente creciente
      den = (x1 - x0);
      if den <= 0
        y = y0;
      else
        t = (x - x0)/den;
        y = y0 + (y1 - y0)*t;
      end
    end

    function validateInputsImpl(obj,Lin_dB,mode,threshold,ratio,knee,makeup)
      validateattributes(Lin_dB,   {'double'}, {'column'});
      validateattributes(mode,     {'numeric'},{'scalar','real','finite'});
      validateattributes(threshold,{'numeric'},{'scalar','real','finite'});
      validateattributes(ratio,    {'numeric'},{'scalar','real'});
      validateattributes(knee,     {'numeric'},{'scalar','real','finite'});
      validateattributes(makeup,   {'numeric'},{'scalar','real','finite'});
      if size(Lin_dB,1) ~= obj.N
        % Permite N distinto en simulación, pero avisa si quieres estrictitud
        % error('lut_gc_sys: Lin_dB debe ser [N x 1] con N=%d', obj.N);
      end
    end

    function out = getOutputSizeImpl(obj)
      out = [obj.N 1];
    end

    function out = getOutputDataTypeImpl(~)
      out = 'double';
    end

    function out = isOutputComplexImpl(~)
      out = false;
    end

    function out = isOutputFixedSizeImpl(~)
      out = true;
    end

  end
end