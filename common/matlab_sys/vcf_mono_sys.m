classdef vcf_mono_sys < matlab.System
  % vcf_mono_sys  Biquad (2º orden) mono, frame-based, codegen-friendly, con bypass
  %
  % Inputs:
  %   in     : [N x 1] double
  %   type   : scalar (se castea a uint8)
  %   freq   : scalar Hz
  %   Q      : scalar (Q o S en shelves)
  %   gain   : scalar dB (solo shelving/peaking)
  %   bypass : scalar (0/1). Si 1 => out=in
  %
  % Output:
  %   out    : [N x 1] double

  properties(Nontunable)
    N  = 128;
    Fs = 48000;
  end

  properties(DiscreteState)
    z; % [2x1]
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
      obj.z = zeros(2,1);

      % arranca en bypass (filtro identidad)
      obj.b = [1 0 0];
      obj.a = [1 0 0];

      % fuerza recálculo en el primer frame
      obj.lastType = uint8(255);
      obj.lastFreq = NaN;
      obj.lastQ    = NaN;
      obj.lastGain = NaN;
    end

    function out = stepImpl(obj, in, type, freq, Q, gain, bypass)
      bp = (bypass ~= 0);

      % Si cambia algún parámetro, recalcula (aunque esté en bypass, así queda listo)
      t = uint8(type);
      if t ~= obj.lastType || freq ~= obj.lastFreq || Q ~= obj.lastQ || gain ~= obj.lastGain
        obj.updateCoeffs(t, freq, Q, gain);
      end

      [y, obj.z] = filter(obj.b, obj.a, in, obj.z);

      if bp
        out = in;   % bypass devuelve dry
      else
        out = y;    % filtro activo
      end

    end

    function resetImpl(obj)
      obj.z(:) = 0;
    end

    function updateCoeffs(obj, type, freq, Q, gain)
      Fs = double(obj.Fs);
      f  = double(freq);
      Qv = double(Q);
      G  = double(gain);
      t  = double(type);

      if ~isfinite(f),  f  = 1000; end
      if ~isfinite(Qv), Qv = 0.707; end
      if ~isfinite(G),  G  = 0; end

      % límites prácticos
      fmin = 0.1;
      fmax = 0.499 * Fs;
      if f < fmin, f = fmin; end
      if f > fmax, f = fmax; end
      if Qv < 1e-3, Qv = 1e-3; end

      wc = 2*pi*(f/Fs);

      if t < 1, t = 1; elseif t > 7, t = 7; end

      b = [1 0 0]; a = [1 0 0];

      switch uint8(t)
        case 1  % lowpass
          [b, a] = LowPass(wc);

        case 2  % highpass
          [b, a] = HighPass(wc);

        case 3  % bandpass (0 dB peak)
          [b, a] = BandPass(wc, Qv);

        case 4  % bandreject / notch
          [b, a] = BandReject(wc, Qv);

        case 5  % lowshelf (RBJ, usando Q como S en tu implementación)
          [b, a] = LowShelving(wc, G);

        case 6  % highshelf (RBJ, usando Q como S en tu implementación)
          [b, a] = HighShelving(wc, G);

        case 7  % peaking / centershelf
          [b, a] = CenterShelving(wc, G, Qv);
      end

      obj.b = b;
      obj.a = a;

      obj.lastType = uint8(type);
      obj.lastFreq = freq;
      obj.lastQ    = Q;
      obj.lastGain = gain;
    end

    function [sz, dt, cp] = getDiscreteStateSpecificationImpl(~, name)
      if strcmp(name,'z')
        sz = [2 1]; dt = 'double'; cp = false;
      end
    end

    function sts = getSampleTimeImpl(obj)
      Ts  = double(obj.N)/double(obj.Fs);
      sts = createSampleTime(obj,'Type','Discrete','SampleTime',Ts);
    end

    function sz = getOutputSizeImpl(obj)
      sz = [obj.N 1];
    end
    function dt = getOutputDataTypeImpl(~), dt='double'; end
    function c  = isOutputComplexImpl(~), c=false; end
    function f  = isOutputFixedSizeImpl(~), f=true; end

    function n = getNumInputsImpl(~),  n = 6; end
    function n = getNumOutputsImpl(~), n = 1; end

    function [sz1,sz2,sz3,sz4,sz5,sz6] = getInputSizeImpl(obj)
      sz1 = [obj.N 1];  % in
      sz2 = [1 1];      % type
      sz3 = [1 1];      % freq
      sz4 = [1 1];      % Q
      sz5 = [1 1];      % gain
      sz6 = [1 1];      % bypass
    end

    function [dt1,dt2,dt3,dt4,dt5,dt6] = getInputDataTypeImpl(~)
      dt1='double';     % in
      dt2='double';     % type (luego casteo a uint8)
      dt3='double';     % freq
      dt4='double';     % Q
      dt5='double';     % gain
      dt6='double';     % bypass (0/1)
    end

    function [c1,c2,c3,c4,c5,c6] = isInputComplexImpl(~)
      c1=false; c2=false; c3=false; c4=false; c5=false; c6=false;
    end

    function [f1,f2,f3,f4,f5,f6] = isInputFixedSizeImpl(~)
      f1=true; f2=true; f3=true; f4=true; f5=true; f6=true;
    end

    function names = getInputNamesImpl(~)
      names = ["in","type","freq","Q","gain","bypass"];
    end

    function names = getOutputNamesImpl(~)
      names = "out";
    end

  end
end