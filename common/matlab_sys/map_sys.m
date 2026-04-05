classdef map_sys < matlab.System
% map_sys Mapeo de señales de control.
%
% Entrada:
% u : señal de control (escalar o vector/frame)
%
% Salida:
% y : señal de control mapeada
%
% Modos:
% 1 = vocoder_env_db con threshold+slope
% 2 = dB offset + clip
% 3 = leveller: G = Offset - u, limitado a [outMin, outMax]
%
% Observación:
% Para vocoder, lo más natural suele ser:
% ENV(dB) -> MAP(mode=1) -> ARF -> VCA
%
% Para leveller:
% ENV(dB) -> MAP(mode=3, Offset=ref_dB) -> ARF -> VCA

  properties(Nontunable)
    bypass (1,1) uint8 = 0; % 0=fx, 1=bypass 
  end

  properties
    Mode   (1,1) uint8  = uint8(1);
    Offset (1,1) double = 40.0;
    Threshold (1,1) double = -45.0;
    Slope     (1,1) double = 1.0;
  end

  methods (Access = protected)

    function y = stepImpl(obj, u, outMin, outMax)

      x = double(u);
      y = zeros(size(x));

      outMin = double(outMin(1));
      outMax = double(outMax(1));

      if obj.bypass ~= 0
        y = x;
        return;
      end

      switch obj.Mode

        case 1  % vocoder_env_db con threshold+slope
          e = max(x - obj.Threshold, 0);
          y = outMin + obj.Slope .* e;
          y = local_clip(y, outMin, outMax);

          
          case 2  % dB offset + clip
            y = x + obj.Offset;
            y = local_clip(y, min(outMin,outMax), max(outMin,outMax));

          case 3  % leveller: G = ref_dB - nivel_actual
            y = obj.Offset - x;
            y = local_clip(y, outMin, outMax);

        otherwise
          y = x;
      end
    end

    function validateInputsImpl(~, u, outMin, outMax)
      if ~isnumeric(u) || ~isreal(u)
        error('map_sys:Input','u must be numeric and real.');
      end

      validateattributes(outMin,{'numeric'},{'real','finite','nonempty'});
      validateattributes(outMax,{'numeric'},{'real','finite','nonempty'});
    end

    function num = getNumInputsImpl(~)
      num = 3;
    end

    function num = getNumOutputsImpl(~)
      num = 1;
    end

    function out = getOutputSizeImpl(obj)
      out = propagatedInputSize(obj,1);
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

function y = local_clip(x, xmin, xmax)
  y = min(max(x, xmin), xmax);
end