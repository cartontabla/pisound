classdef arf_sys < matlab.System
% arf_sys  Attack/Release Filter (suavizado temporal) para señal de control
%
% Entradas:
%   GdB0    : [N x 1] double   ganancia instantánea en dB (salida de GC)
%   attack  : scalar double    (s) tiempo de ataque
%   release : scalar double    (s) tiempo de release
%
% Salida:
%   GdB     : [N x 1] double   ganancia suavizada en dB
%
% Convención:
% - Si GdB0(n) < GdB(n-1)  -> ATTACK
% - Si GdB0(n) >= GdB(n-1) -> RELEASE
%
% y(n) = a*y(n-1) + (1-a)*x(n),  a = exp(-1/(T*Fs))

  properties(Nontunable)
    N  = 128;
    Fs = 48000;
    bypass (1,1) uint8 = 0;   % 0=fx, 1=bypass
  end

  properties(DiscreteState)
    gPrev_dB;
  end

  methods(Access = protected)

    function setupImpl(obj)
      obj.gPrev_dB = 0.0;
    end

    function resetImpl(obj)
      obj.gPrev_dB = 0.0;
    end

    function GdB = stepImpl(obj, GdB0, attack, release)

      GdB = zeros(obj.N,1);

      % bypass del suavizado
      if obj.bypass ~= 0
        GdB = GdB0;
        obj.gPrev_dB = GdB0(end);
        return;
      end

      % tiempos válidos
      Ta = max(double(attack),  1e-6);
      Tr = max(double(release), 1e-6);

      alphaA = exp(-1.0/(Ta*obj.Fs));
      alphaR = exp(-1.0/(Tr*obj.Fs));

      g = obj.gPrev_dB;

      for n = 1:obj.N
        x = GdB0(n);

        if x < g
          % más reducción de ganancia -> attack
          g = alphaA*g + (1.0-alphaA)*x;
        else
          % menos reducción de ganancia -> release
          g = alphaR*g + (1.0-alphaR)*x;
        end

        GdB(n) = g;
      end

      obj.gPrev_dB = g;
    end

    function validateInputsImpl(obj, GdB0, attack, release)
      validateattributes(GdB0, {'double'}, {'column','real','finite'});
      validateattributes(attack, {'numeric'}, {'scalar','real','finite'});
      validateattributes(release, {'numeric'}, {'scalar','real','finite'});

      if size(GdB0,1) ~= obj.N
        error('arf_sys:BadInputSize', ...
          'GdB0 must have size [%d x 1].', obj.N);
      end
    end

    function [sz, dt, cp] = getDiscreteStateSpecificationImpl(~, name)
      if strcmp(name,'gPrev_dB')
        sz = [1 1];
        dt = 'double';
        cp = false;
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

    function flag = supportsMultipleInstanceImpl(~)
      flag = true;
    end
  end
end