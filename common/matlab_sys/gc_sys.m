classdef gc_sys < matlab.System
% gc_sys  Curva estática de ganancia (Gain Curve), frame-based, sin estado.
%
% Convención de ratio (double escalar):
%   ratio > 0  => procesamiento por ENCIMA del umbral (COMP / LIMIT)
%   ratio < 0  => procesamiento por DEBAJO del umbral (EXP / GATE)
%   R = abs(ratio)
%   ratio = +Inf => LIMIT ideal (clamp a T por arriba)
%   ratio = -Inf => GATE ideal (ganancia mínima Floor_dB por abajo)
%
% Entradas:
%   Mode     : 0=neutral, 1=passthrough, 2=gc normal
%   Lin_dB   : [N x 1] double  nivel en dB
%   threshold: scalar double   (dB)
%   ratio    : scalar double   (ver convención arriba)
%   knee     : scalar double   (dB, >=0)
%   makeup   : scalar double   (dB)
%
% Salida:
%   GdB0     : [N x 1] double  ganancia en dB (instantánea)

  properties(Nontunable)
    N = 128;
  end

  properties
    Mode     (1,1) uint8 = 2;  % 0=neutral, 1=passthrough, 2=gc normal
    Eps      = 1e-12;   % para detectar K≈0
    Floor_dB = -80;     % suelo de ganancia para GATE (sin parámetro extra)
  end

  methods(Access = protected)

    function GdB0 = stepImpl(obj, Lin_dB, threshold, ratio, knee, makeup)

      Nloc = size(Lin_dB,1);
      GdB0 = zeros(Nloc,1);
    
      switch obj.Mode
        case 0
          % Neutral: ganancia unitaria
          return
    
        case 1
          % Passthrough vocoder: nivel dB -> ganancia dB recortada
          GdB0 = min(max(Lin_dB, obj.Floor_dB), 0.0);
          return
    
        case 2
          % GC normal    
          T = double(threshold);
          r = double(ratio);
          K = max(double(knee), 0);
          M = double(makeup);
        
          if r == 0
            r = 1.0;
          end
          isUp = (r > 0);
          R    = abs(r);
        
          isLimit = isUp  && isinf(R);
          isGate  = ~isUp && isinf(R);
        
          if isfinite(R) && (R < 1)
            R = 1.0;
          end
        
          for n = 1:Nloc
            L = Lin_dB(n);
        
            if K <= obj.Eps
              Lout = hardKnee(obj, L, T, R, isUp, isLimit, isGate);
            else
              Lout = softKnee(obj, L, T, R, K, isUp, isLimit, isGate);
            end
        
            GdB = (Lout - L) + M;
        
            if isGate
              GdB = max(GdB, obj.Floor_dB);
            end
        
            GdB0(n) = GdB;
          end
          return

        otherwise
            return
      end
    end

    function Lout = hardKnee(obj, L, T, R, isUp, isLimit, isGate)
      % HARD KNEE, por tramos

      if isUp
        % -------- COMP/LIMIT: actúa por encima --------
        if L <= T
          Lout = L;
        else
          if isLimit
            Lout = T;                  % limitador ideal
          else
            Lout = T + (L - T)/R;      % compresión
          end
        end

      else
        % -------- EXP/GATE: actúa por debajo --------
        if L >= T
          Lout = L;
        else
          if isGate
            % gate ideal: equivale a imponer ganancia mínima (suelo)
            % usando Lout = L + Floor  => GdB = Floor (antes de makeup)
            Lout = L + obj.Floor_dB;
          else
            Lout = T + R*(L - T);      % expansión descendente
          end
        end
      end
    end

    function Lout = softKnee(obj, L, T, R, K, isUp, isLimit, isGate)
      % SOFT KNEE (transición cuadrática), simétrica arriba/abajo

      lower = T - K/2;
      upper = T + K/2;

      if isUp
        % -------- COMP/LIMIT: transición alrededor de T por arriba --------
        if L < lower
          Lout = L;

        elseif L > upper
          if isLimit
            Lout = T;
          else
            Lout = T + (L - T)/R;
          end

        else
          % dentro del knee: x en [0,K]
          x = L - lower;

          if isLimit
            % equivalente a 1/R -> 0 (clamp) en la fórmula
            slopeTarget = 0;           % 1/R efectivo
          else
            slopeTarget = 1/R;         % pendiente por encima
          end

          % misma forma que tu compresor original:
          % Lout = L + ((slopeTarget - 1) * x^2)/(2K)
          Lout = L + ((slopeTarget - 1) * (x^2)) / (2*K);
        end

      else
        % -------- EXP/GATE: transición alrededor de T por abajo --------
        if L > upper
          Lout = L;
        
        elseif L < lower
          if isGate
            % por debajo del knee: ganancia fija Floor_dB
            Lout = L + obj.Floor_dB;
          else
            % expansión descendente dura
            Lout = T + R*(L - T);
          end
        
        else
          % dentro del knee
          x = upper - L;   % x en [0, K]
          t = x / K;       % t en [0, 1]
        
          if isGate
            % transición suave de 0 dB a Floor_dB
            % smoothstep: 0 en upper, 1 en lower
            s = t*t*(3 - 2*t);
            Lout = L + obj.Floor_dB * s;
          else
            % expansión descendente suave:
            % en upper -> Lout = L
            % en lower -> Lout = T + R*(L - T)
            Lout = L - ((R - 1) * (x^2)) / (2*K);
          end
        end
      end
    end

    function validateInputsImpl(~, Lin_dB, threshold, ratio, knee, makeup)
      validateattributes(Lin_dB,    {'double'}, {'column'});
      validateattributes(threshold, {'numeric'}, {'scalar','real','finite'});
      validateattributes(ratio,     {'numeric'}, {'scalar','real'}); % permite +/-Inf
      validateattributes(knee,      {'numeric'}, {'scalar','real','finite'});
      validateattributes(makeup,    {'numeric'}, {'scalar','real','finite'});
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
    
    function flag = supportsMultipleInstanceImpl(~)
      flag = true;
    end
  end
end