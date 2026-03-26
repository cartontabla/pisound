classdef vca_mono_sys < matlab.System
% vca_mono_sys  Amplificador controlado (mono)
%               Ganancia escalar o vectorial (frame-based)
%
% Entradas:
%   in   : [N x 1] double
%   gain : [1 x 1] o [N x 1] double
%
% Parámetro:
%   Mode : 0 = lineal
%          1 = dB
%
% Salida:
%   out  : [N x 1] double

  properties(Nontunable)
    N  = 128;
    Fs = 48000;
    bypass (1,1) uint8 = 0;  % 0=fx, 1=no fx
  end

  properties
    Mode (1,1) uint8 = uint8(0);  % 0=lineal, 1=dB
    ForceGainEnable (1,1) uint8 = uint8(0); % ForceGainEnable 0=Disable
    ForceGainValue  (1,1) double = 0;   % ForceGainValue, en dB si Mode=1
  end

  methods(Access=protected)

    function icon = getIconImpl(~)
      icon = matlab.system.display.Icon('VCA.png');
    end

    function out = stepImpl(obj, in, gain)

      bp = (obj.bypass ~= 0);   % bp es logical
      if bp
          out = in;  % Bypass: output is the same as input
          return;
      end

      if obj.ForceGainEnable ~= 0
        if isscalar(gain)
          gain = obj.ForceGainValue;
        else
          gain = obj.ForceGainValue * ones(obj.N,1);
        end
      end

      % --- convertir si está en dB ---
      if obj.Mode == uint8(1)
        gainLin = 10.^(double(gain)/20);
      else
        gainLin = double(gain);
      end

      out = gainLin .* in;
    end

    function resetImpl(~)
      % sin estados
    end

    % -------------------------
    % INTERFAZ
    % -------------------------

    function n = getNumInputsImpl(~),  n = 2; end
    function n = getNumOutputsImpl(~), n = 1; end

    function [s1,s2] = getInputSizeImpl(obj)
      s1 = [obj.N 1];  % in
      s2 = [-1  1];    % gain: tamaño variable permitido
    end

    function [d1,d2] = getInputDataTypeImpl(~)
      d1 = 'double';
      d2 = 'double';
    end

    function [c1,c2] = isInputComplexImpl(~)
      c1 = false; c2 = false;
    end

    function [f1,f2] = isInputFixedSizeImpl(~)
      f1 = true;      % in fijo
      f2 = false;     % gain puede variar (1 o N)
    end

    function sz = getOutputSizeImpl(obj)
      sz = [obj.N 1];
    end

    function dt = getOutputDataTypeImpl(~)
      dt = 'double';
    end

    function c = isOutputComplexImpl(~)
      c = false;
    end

    function f = isOutputFixedSizeImpl(~)
      f = true;
    end

    function names = getInputNamesImpl(~)
      names = ["in","gain"];
    end

    function names = getOutputNamesImpl(~)
      names = "out";
    end

    function sts = getSampleTimeImpl(obj)
      Ts  = double(obj.N)/double(obj.Fs);
      sts = createSampleTime(obj,'Type','Discrete','SampleTime',Ts);
    end

    function flag = supportsMultipleInstanceImpl(~)
      flag = true;
    end

  end
end