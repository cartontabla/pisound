classdef osc_sys < matlab.System
  % osc_sys Wavetable oscillator (L with iPhase offset, R reference)

  % Public, non-tunable properties
  properties(Nontunable)
    N = 128;          % FrameLength / block size
    Fs = 48000;
    OutMaxL = 4096;   % max table length stored
    InFixL  = 1024;   % length of built-in tables
  end

  % Public, tunable properties
  properties
    A = 1;
    F = 1000;         % Hz
    iPhase = 0;       % rad (offset applied to LEFT only)
    WaveSel = uint8(1); % 1..10
  end

  properties(DiscreteState)
    Phase;            % cycles in [0,1)
  end

  properties(Access=private)
    WaveTables;       % [OutMaxL x 10]
    TablesSize;       % [1 x 10]
  end

  methods(Access=protected)

    function icon = getIconImpl(~)
      icon = matlab.system.display.Icon('osc.png');
    end

    function setupImpl(obj)
      % Phase accumulator in cycles
      obj.Phase = 0;

      % Allocate storage
      obj.WaveTables = zeros(obj.OutMaxL, 10);
      obj.TablesSize = zeros(1, 10);

      % ----- Built-in tables (1..5)
      L = min(obj.InFixL, obj.OutMaxL);
      p = (0:L-1)'/L;  % cycles [0,1)
      sine    = sin(2*pi*p);
      square  = 2*(p < 0.5) - 1;
      tri     = 1 - 4*abs(p - 0.5);
      sawUp   = 2*p - 1;
      sawDown = 1 - 2*p;

      obj.TablesSize(1:5) = L;
      obj.WaveTables(1:L,1:5) = [sine square tri sawUp sawDown];

      % ----- Custom tables (6..10)
      WTc = custom_wavetables(L);          % [L x 5]
      obj.WaveTables(1:L,6:10) = WTc;
      obj.TablesSize(6:10) = L;
    end

    function [outL, outR] = stepImpl(obj)
      % Select table
      sel = double(obj.WaveSel);
      if sel < 1, sel = 1; elseif sel > 10, sel = 10; end

      L = double(obj.TablesSize(sel));
      if L < 2, L = 2; end
      tbl = obj.WaveTables(:, sel);

      N = obj.N;
      outL = zeros(N,1);
      outR = zeros(N,1);

      inc = double(obj.F) / double(obj.Fs); % cycles/sample
      ph  = double(obj.Phase);              % cycles [0,1)

      twoPi = 2*pi;
      % iPhase (rad) -> cycles, wrapped to [0,1)
      phOff = mod(double(obj.iPhase), twoPi)/ twoPi;

      for i = 1:N
        % ---- RIGHT (reference, no offset)
        xR = ph;                    % cycles
        idxR  = xR * L;             % 0..L
        kR    = floor(idxR);        % 0..L-1
        fracR = idxR - kR;          % 0..1
        p1R = kR + 1;               % 1..L
        if p1R > L, p1R = 1; end
        p2R = p1R + 1;
        if p2R > L, p2R = 1; end
        vR = tbl(p1R) + fracR*(tbl(p2R) - tbl(p1R));
        outR(i) = obj.A * vR;

        % ---- LEFT (offset)
        xL = ph + phOff;
        if xL >= 1, xL = xL - 1; end
        idxL  = xL * L;
        kL    = floor(idxL);
        fracL = idxL - kL;
        p1L = kL + 1;
        if p1L > L, p1L = 1; end
        p2L = p1L + 1;
        if p2L > L, p2L = 1; end
        vL = tbl(p1L) + fracL*(tbl(p2L) - tbl(p1L));
        outL(i) = obj.A * vL;

        % advance phase (wrap)
        ph = ph + inc;
        if ph >= 1, ph = ph - 1; end
      end

      obj.Phase = ph;
    end

    function resetImpl(obj)
      obj.Phase = 0;
    end

    function [sz, dt, cp] = getDiscreteStateSpecificationImpl(~, name)
      if strcmp(name,'Phase')
        sz = [1 1];
        dt = 'double';
        cp = false;
      end
    end

    function sts = getSampleTimeImpl(obj)
      Ts = double(obj.N)/double(obj.Fs);
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

    function n = getNumInputsImpl(~),  n = 0; end
    function n = getNumOutputsImpl(~), n = 2; end
  end
end
