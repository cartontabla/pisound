classdef filters_sys < matlab.System
  % filters_sys  Biquad (2º orden) estéreo, frame-based, codegen-friendly
  %
  % Type:
  %   1 lowpass
  %   2 highpass
  %   3 bandpass
  %   4 bandreject (notch)
  %   5 lowshelf
  %   6 highshelf
  %   7 centershelf (peaking)
  %
  % Freq: Hz (fc o f0)
  % Q: factor de mérito
  % Gain: dB (solo para shelving y peaking)

  properties(Nontunable)
    N  = 128;
    Fs = 48000;
  end

  properties
    FilterType = uint8(1);
    Freq = 1000;    % Hz
    Q    = 0.707;   % -
    Gain = 0;       % dB (peaking/shelf)
  end

  properties(DiscreteState)
    zL; % initial condition
    zR;
  end

  properties(Access=private)
    b; a;
    lastFilterType;
    lastFreq;
    lastQ;
    lastGain;
  end

  methods(Access=protected)

    function icon = getIconImpl(~)
      icon = matlab.system.display.Icon('filt2.png');
    end

    function setupImpl(obj)
      obj.zL = zeros(2,1);
      obj.zR = zeros(2,1);

      obj.lastFilterType = uint8(0);
      obj.lastFreq = NaN;
      obj.lastQ    = NaN;
      obj.lastGain = NaN;

      obj.updateCoeffs(); % inicializa coefs
    end

    function [outL, outR] = stepImpl(obj, inL, inR)
      % Recalcular coefs si cambió algún parámetro
      if obj.FilterType ~= obj.lastFilterType ||... 
         obj.Freq ~= obj.lastFreq ||... 
         obj.Q ~= obj.lastQ ||...
         obj.Gain ~= obj.lastGain
         obj.updateCoeffs();
      end

      N = obj.N;
      outL = zeros(N,1);
      outR = zeros(N,1);

      % ---- Canal L (idx=1)
      [outL, obj.zL]=filter(obj.b, obj.a, inL, obj.zL);

      % ---- Canal R (idx=2)
      [outR, obj.zR]=filter(obj.b, obj.a, inR, obj.zR);
    end

    function resetImpl(obj)
      obj.zL(:) = 0;
      obj.zR(:) = 0;
    end

    function updateCoeffs(obj)
      % Sanitiza parámetros (evita NaN/inf y zonas problemáticas)
      Fs = double(obj.Fs);
      f  = double(obj.Freq);
      Q  = double(obj.Q);
      G  = double(obj.Gain);
      t  = double(obj.FilterType);

      if ~isfinite(f), f = 1000; end
      if ~isfinite(Q), Q = 0.707; end
      if ~isfinite(G), G = 0; end

      % límites prácticos
      fmin = 0.1;
      fmax = 0.499*Fs;
      if f < fmin, f = fmin; end
      if f > fmax, f = fmax; end
      if Q < 1e-3, Q = 1e-3; end

      % Pre-cálculos
      wc = 2*pi*(f/Fs);

      % Coefs (sin normalizar)
      b = [1 0 0];
      a = [1 0 0];

      if t < 1, t = 1; elseif t > 7, t = 7; end

      switch uint8(t)
        case 1  % lowpass
          [b, a] = LowPass(wc);

        case 2  % highpass
          [b, a] = HighPass(wc);

        case 3  % bandpass (0 dB peak)
          [b, a] = BandPass(wc, Q);
          
        case 4  % bandreject / notch
          [b, a] = BandReject(wc, Q);
          
        case 5  % lowshelf (RBJ, S=1)
          [b, a] = LowShelving(wc, G);
          
        case 6  % highshelf (RBJ, S=1)
          [b, a] = HighShelving(wc,G);
          
        case 7  % centershelf / peaking (RBJ)
          [b, a] = CenterShelving(wc, G, Q);
          
      end

      % Guarda "last"
      obj.lastFilterType = obj.FilterType;
      obj.lastFreq = obj.Freq;
      obj.lastQ    = obj.Q;
      obj.lastGain = obj.Gain;

      obj.b = b;
      obj.a = a;
    end

    function [sz, dt, cp] = getDiscreteStateSpecificationImpl(~, name)
      if strcmp(name,'zL') || strcmp(name,'zR')
        sz = [2 1];
        dt = 'double';
        cp = false;
      end
    end

    function sts = getSampleTimeImpl(obj)
      Ts  = double(obj.N)/double(obj.Fs);
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

    function [sz1, sz2] = getInputSizeImpl(obj)
      sz1 = [obj.N 1];
      sz2 = [obj.N 1];
    end

    function [dt1, dt2] = getInputDataTypeImpl(~)
      dt1 = 'double'; dt2 = 'double';
    end

    function [c1, c2] = isInputComplexImpl(~)
      c1 = false; c2 = false;
    end

    function [f1, f2] = isInputFixedSizeImpl(~)
      f1 = true; f2 = true;
    end

    function n = getNumInputsImpl(~),  n = 2; end
    function n = getNumOutputsImpl(~), n = 2; end

  end
end
