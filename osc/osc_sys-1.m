classdef osc_sys < matlab.System
  % osc_sys System object

  % Public, non-tunable properties
  properties(Nontunable)
    N = 128;    % FrameLength, BufSize
    Fs = 48000;
  end  

  % Public, tunable properties
  properties
    A = 1;
    F = 1000;   % (Hz)
    iPhase = 0; % (rad)
  end

  properties(DiscreteState)
    Phase;      % (rad)
  end

  methods(Access = protected)
    function icon = getIconImpl(~)
      % Opción A: Texto
      % icon = {'O S C', 'Generador'};
        
      % Opción B: Imagen (Guarda 'icono_seno.png' junto al .m)
      icon = matlab.system.display.Icon('osc.png');
    end

    function setupImpl(obj)
      obj.Phase = 0;
    end

    function [outL, outR] = stepImpl(obj)
      N = obj.N;
      twoPi = 2*pi;
      w = twoPi* (obj.F/obj.Fs);
      phase = mod(obj.Phase + obj.iPhase, twoPi);
      n = (0:N-1)';
      sigL = obj.A* sin(phase+ w* n);
      sigR = obj.A* sin(obj.Phase+ w* n);
      % fase final = fase inicial en el siguiente frame
      obj.Phase = mod(obj.Phase+ w* N, twoPi);
      outL = sigL;
      outR = sigR;
    end

    function resetImpl(obj)
      obj.Phase = 0;
    end

    function [sz, dt, cp] = getDiscreteStateSpecificationImpl(~, name)
      if strcmp(name, 'Phase')
        sz = [1 1];      % Tamaño: Escalar
        dt = 'double';   % Tipo de dato: double
        cp = false;      % Complejidad: No es complejo
      end
    end

    function sts = getSampleTimeImpl(obj)
      Ts = double(obj.N)/ double(obj.Fs); % escalar SIEMPRE
      sts = createSampleTime(obj,'Type','Discrete','SampleTime',Ts);
    end

    function [sz1, sz2] = getOutputSizeImpl(obj)
      sz1 = [obj.N 1];
      sz2 = [obj.N 1];
    end

    function [dt1, dt2] = getOutputDataTypeImpl(~)
      dt1 = 'double'; dt2 = 'double';
    end

    function [c1, c2] = isOutputComplexImpl(~)
      c1 = false; c2 = false;
    end

    function [f1, f2] = isOutputFixedSizeImpl(~)
      f1 = true; f2 = true;
    end

    function n = getNumInputsImpl(~)
      n = 0;
    end

    function n = getNumOutputsImpl(~)
      n = 2;
    end

  end
end
