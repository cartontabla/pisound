classdef vcf_sys < matlab.System
  % vcf_sys  Biquad (2º orden) estéreo, frame-based, codegen-friendly
  %
  % Inputs:
  %   inL, inR : [N x 1] double
  %   type     : scalar (se castea a uint8)
  %            : 1 LP, 2 HP, 3 BP, 4 BR, 5 LS, 6 HS, 7 CS
  %   freq     : scalar Hz
  %   Q        : scalar (Q o S en shelves)
  %   gain     : scalar dB (solo shelving/peaking)

  properties(Nontunable)
    N  = 128;
    Fs = 48000;
    bypass (1,1) uint8 = 0;  % 0=fx, 1=no fx
  end

  properties(DiscreteState)
    zL; % [2x1]
    zR; % [2x1]
  end

  properties(Access=private)
    b; a;               % coefs normalizados (a0=1)
    lastType;
    lastFreq;
    lastQ;
    lastGain;
  end

  methods(Access=protected)

    function icon = getIconImpl(~)
      icon = matlab.system.display.Icon('VCF.png');
    end

    function setupImpl(obj)
      obj.zL = zeros(2,1);
      obj.zR = zeros(2,1);

      % arranca en bypass
      obj.b = [1 0 0];
      obj.a = [1 0 0];

      % fuerza recálculo en el primer frame
      obj.lastType = uint8(255);
      obj.lastFreq = NaN;
      obj.lastQ    = NaN;
      obj.lastGain = NaN;
    end

    function [outL,outR] = stepImpl(obj, inL, inR, type, freq, Q, gain)
      t = uint8(type);  % acepta double y castea

      if t ~= obj.lastType || freq ~= obj.lastFreq || Q ~= obj.lastQ || gain ~= obj.lastGain
        obj.updateCoeffs(t, freq, Q, gain);
      end

      bp = (obj.bypass ~= 0);   % bp es logical
      if bp
         outL = inL;  % Bypass effect
        outR = inR;  % Bypass effect
      else
        [outL, obj.zL] = filter(obj.b, obj.a, inL, obj.zL);
        [outR, obj.zR] = filter(obj.b, obj.a, inR, obj.zR);
      end
    end

    function resetImpl(obj)
      obj.zL(:) = 0;
      obj.zR(:) = 0;
    end

    function updateCoeffs(obj, type, freq, Q, gain)
      Fs = double(obj.Fs);
      f  = double(freq);
      Q = double(Q);
      G  = double(gain);
      t  = double(type);

      if ~isfinite(f),  f  = 1000; end
      if ~isfinite(Q),  Q = 0.707; end
      if ~isfinite(G),  G  = 0; end

      % límites prácticos
      fmin = 0.1;
      fmax = 0.499 * Fs;
      if f < fmin, f = fmin; end
      if f > fmax, f = fmax; end
      if Q < 1e-3, Q = 1e-3; end

      wc = 2*pi*(f/Fs);

      if t < 1, t = 1; elseif t > 7, t = 7; end

      b = [1 0 0]; a = [1 0 0];
      
      switch uint8(t)
        case 1  % lowpass
          [b, a] = LowPass(wc);

        case 2  % highpass
          [b, a] = HighPass(wc);

        case 3  % bandpass (0 dB peak)
          [b, a] = BandPass(wc, Q);

        case 4  % bandreject / notch
          [b, a] = BandReject(wc, Q);

        case 5  % lowshelf (RBJ, usando Q como S)
          [b, a] = LowShelving(wc, G);

        case 6  % highshelf (RBJ, usando Q como S)
          [b, a] = HighShelving(wc, G);

        case 7  % peaking / centershelf
          [b, a] = CenterShelving(wc, G, Q);
      end

      obj.b = b;
      obj.a = a;

      obj.lastType = uint8(type);
      obj.lastFreq = freq;
      obj.lastQ    = Q;
      obj.lastGain = gain;
    end

    function [sz, dt, cp] = getDiscreteStateSpecificationImpl(~, name)
      if strcmp(name,'zL') || strcmp(name,'zR')
        sz = [2 1]; dt = 'double'; cp = false;
      end
    end

    function sts = getSampleTimeImpl(obj)
      Ts  = double(obj.N)/double(obj.Fs);
      sts = createSampleTime(obj,'Type','Discrete','SampleTime',Ts);
    end

    function [sz1, sz2] = getOutputSizeImpl(obj)
      sz1 = [obj.N 1]; sz2 = [obj.N 1];
    end
    function [dt1, dt2] = getOutputDataTypeImpl(~), dt1='double'; dt2='double'; end
    function [c1, c2] = isOutputComplexImpl(~), c1=false; c2=false; end
    function [f1, f2] = isOutputFixedSizeImpl(~), f1=true; f2=true; end

    function n = getNumInputsImpl(~),  n = 6; end
    function n = getNumOutputsImpl(~), n = 2; end

    function [sz1,sz2,sz3,sz4,sz5,sz6] = getInputSizeImpl(obj)
      sz1 = [obj.N 1];  % inL
      sz2 = [obj.N 1];  % inR
      sz3 = [1 1];      % type
      sz4 = [1 1];      % freq
      sz5 = [1 1];      % Q
      sz6 = [1 1];      % gain
    end

    function [dt1,dt2,dt3,dt4,dt5,dt6] = getInputDataTypeImpl(~)
      dt1='double'; dt2='double';
      dt3='double'; % más cómodo en Simulink (luego casteo a uint8)
      dt4='double'; dt5='double'; dt6='double';
    end

    function [c1,c2,c3,c4,c5,c6] = isInputComplexImpl(~)
      c1=false; c2=false; c3=false; c4=false; c5=false; c6=false;
    end

    function [f1,f2,f3,f4,f5,f6] = isInputFixedSizeImpl(~)
      f1=true; f2=true; f3=true; f4=true; f5=true; f6=true;
    end

    function flag = supportsMultipleInstanceImpl(~)
      flag = true;
    end

  end
end
