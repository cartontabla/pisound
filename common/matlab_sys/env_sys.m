classdef env_sys < matlab.System
% env_sys  Detector de nivel (ENV) puro: peak/RMS, sin suavizado (A/R va en ARF)
%         Frame-based, estéreo, codegen-friendly.
%
% Entradas:
%   inL, inR : [N x 1] double
%   mode     : scalar (se castea a uint8)
%          1 = peak (|x|)
%          2 = rms exponencial (con TauRMS)
%
% Salidas:
%   Lin_dB : [N x 1] double   nivel en dBFS (relativo a 1.0)
%   env    : [N x 1] double   nivel lineal (amplitud)
%
% Nota:
% - Estéreo enlazado por máximo: max(canal L, canal R).
% - Se usa eps para evitar log(0).

  properties(Nontunable)
    N  = 128;
    Fs = 48000; % (se deja por coherencia de familia)
  end

  properties
    ModeDefault = uint8(1);  % 1=peak, 2=rms
    Eps         = 1e-12;
    TauRMS = 0.02;  % 20 ms por defecto
  end

    properties(Access = private)
      e_prev
    end

  methods(Access = protected)

    function setupImpl(obj)
      obj.e_prev = 0;
    end

    function resetImpl(obj)
      obj.e_prev = 0;
    end

    function [Lin_dB, env] = stepImpl(obj, inL, inR, mode)
      Nloc = size(inL,1);
      Lin_dB = zeros(Nloc,1);
      env    = zeros(Nloc,1);

      if isempty(mode)
        mode = obj.ModeDefault;
      end
      mode = uint8(mode);

      if mode == uint8(1)
        % -------- PEAK (instantáneo) --------
        for n = 1:Nloc
          e = abs(in(n));
          env(n) = e;
          Lin_dB(n) = 20*log10(e + obj.Eps);
        end

      elseif mode == uint8(2)
        % -------- RMS exponencial (real) --------
        tau = max(obj.TauRMS, 1e-6);
        alpha = exp(-1/(tau * obj.Fs));
        for n = 1:Nloc
          u = in(n)*in(n);
          e = (1 - alpha)*u + alpha*obj.e_prev;
          obj.e_prev = e;

          rms_val = sqrt(e + obj.Eps);
          env(n) = rms_val;
          Lin_dB(n) = 20*log10(rms_val);
        end

      else
        for n = 1:Nloc
          e = abs(in(n));
          env(n) = e;
          Lin_dB(n) = 20*log10(e + obj.Eps);
        end
      end
    end
    
    function validateInputsImpl(~, inL, inR, mode)
      validateattributes(inL, {'double'}, {'column'});
      validateattributes(inR, {'double'}, {'column'});
      validateattributes(mode, {'numeric'}, {'scalar'});
    end

    function [o1, o2] = getOutputSizeImpl(obj)
      o1 = [obj.N 1];
      o2 = [obj.N 1];
    end

    function [o1, o2] = getOutputDataTypeImpl(~)
      o1 = 'double';
      o2 = 'double';
    end

    function [c1, c2] = isOutputComplexImpl(~)
      c1 = false;
      c2 = false;
    end

    function [f1, f2] = isOutputFixedSizeImpl(~)
      f1 = true;
      f2 = true;
    end
  end
end