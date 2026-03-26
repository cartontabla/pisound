classdef vco_mono_sys < matlab.System
  % vco_mono_sys  VCO wavetable mono (LEGO puro)
  %
  % Entradas:
  %   A       : amplitud (double)
  %   F       : Hz (double)
  %   iPhase  : rad, offset de fase (double)
  %   WaveSel : 1..12 (uint8)   (si entra double, se castea)
  %   Mode    : 0=bipolar [-1,1], 1=unipolar [0,1], 2=unipolar [-1,0] (uint8)
  %
  % Salida:
  %   out     : [N x 1] double

  properties(Nontunable)
    N = 128;
    Fs = 48000;
    OutMaxL = 4096;
    InFixL  = 1024;
  end

  properties(DiscreteState)
    Phase; % cycles [0,1)
  end

  properties(Access=private)
    WaveTables;
    TablesSize;
  end

  methods(Access=protected)

    function icon = getIconImpl(~)
      icon = matlab.system.display.Icon('osc.png');
    end

    function setupImpl(obj)
      obj.Phase = 0;

      obj.WaveTables = zeros(obj.OutMaxL, 12);
      obj.TablesSize = zeros(1, 12);

      L = min(obj.InFixL, obj.OutMaxL);
      p = (0:L-1)'/L;

      sine    = sin(2*pi*p);
      square  = 2*(p < 0.5) - 1;
      tri     = 1 - 4*abs(p - 0.5);
      sawUp   = 2*p - 1;
      sawDown = 1 - 2*p;

      obj.TablesSize(1:5) = L;
      obj.WaveTables(1:L,1:5) = [sine square tri sawUp sawDown];

      WTc = custom_wavetables(L);   % [L x 5]
      obj.WaveTables(1:L,6:10) = WTc;
      obj.TablesSize(6:10) = L;
      
      % --- Noise White (periodic wavetable)
      white = 2*rand(L,1) - 1;   % [-1,1]
      obj.WaveTables(1:L,11) = white;
      pink = pinkNoise(L);
      obj.WaveTables(1:L,12) = pink;
      obj.TablesSize(11:12) = L;
    end

    function out = stepImpl(obj, A, F, iPhase, waveSel, mode)

      % WaveSel seguro
      ws = uint8(waveSel);
      if ws < uint8(1)
        ws = uint8(1);
      elseif ws > uint8(12)
        ws = uint8(12);
      end
      sel = double(ws);

      L = double(obj.TablesSize(sel));
      if L < 2, L = 2; end
      tbl = obj.WaveTables(:, sel);

      N = obj.N;
      out = zeros(N,1);

      if ws == 11 || ws == 12
        inc = 1 / L;   % recorre la tabla a velocidad fija
      else
        inc = double(F)/double(obj.Fs);
      end
      ph  = double(obj.Phase);

      phOff = mod(double(iPhase), 2*pi) / (2*pi); % cycles

      modeU = uint8(mode); % 0/1/2

      for i = 1:N
        v = wt_read_lin(tbl, L, ph + phOff);

        switch modeU
          case uint8(0)  % bipolar
            % v = v
          case uint8(1)  % unipolar 0..1
            v = 0.5*(v + 1);
          case uint8(2)  % unipolar -1..0
            v = 0.5*(v - 1);
          otherwise
            % fallback: bipolar
        end

        out(i) = double(A) * v;

        ph = ph + inc;
        if ph >= 1
          ph = ph - 1;
        elseif ph < 0
          ph = ph - floor(ph); % por si F<0
        end
      end

      obj.Phase = ph;
    end

    function resetImpl(obj)
      obj.Phase = 0;
    end

    function [sz, dt, cp] = getDiscreteStateSpecificationImpl(~, name)
      if strcmp(name,'Phase')
        sz = [1 1]; dt = 'double'; cp = false;
      end
    end

    function sts = getSampleTimeImpl(obj)
      Ts = double(obj.N)/double(obj.Fs);
      sts = createSampleTime(obj,'Type','Discrete','SampleTime',Ts);
    end

    % --- I/O
    function n = getNumInputsImpl(~),  n = 5; end
    
    function n = getNumOutputsImpl(~), n = 1; end

    function [s1,s2,s3,s4,s5] = getInputSizeImpl(~)
      s1=[1 1]; s2=[1 1]; s3=[1 1]; s4=[1 1]; s5=[1 1];
    end

    function [d1,d2,d3,d4,d5] = getInputDataTypeImpl(~)
      d1='double'; d2='double'; d3='double';
      d4='uint8';  % WaveSel
      d5='uint8';  % Mode
    end

    function [c1,c2,c3,c4,c5] = isInputComplexImpl(~)
      c1=false; c2=false; c3=false; c4=false; c5=false;
    end
    function [f1,f2,f3,f4,f5] = isInputFixedSizeImpl(~)
      f1=true; f2=true; f3=true; f4=true; f5=true;
    end

    function sz = getOutputSizeImpl(obj)
      sz = [obj.N 1];
    end
    function dt = getOutputDataTypeImpl(~), dt='double'; end
    function c  = isOutputComplexImpl(~), c=false; end
    function f  = isOutputFixedSizeImpl(~), f=true; end

    function names = getInputNamesImpl(~)
      names = ["A","F","iPhase","WaveSel","Mode"];
    end
    function names = getOutputNamesImpl(~)
      names = "out";
    end

  end
end

function v = wt_read_lin(tbl, L, ph)
  if ph >= 1 || ph < 0
    ph = ph - floor(ph);
  end
  idx = ph*L; 
  k = floor(idx); 
  f = idx - k;

  p1 = k + 1; 
  if p1 > L, p1 = 1; end
  p2 = p1 + 1; 
  if p2 > L, p2 = 1; end

  v = tbl(p1) + f*(tbl(p2) - tbl(p1));
end

function pink = pinkNoise(L)
  % --- Noise Pink (periodic wavetable)
  w = 2*rand(L,1) - 1;

  % filtro IIR simple aproximado 1/f
  b0 = 0.99765;
  b1 = 0.96300;
  b2 = 0.57000;

  pink = zeros(L,1);
  for k = 2:L
    pink(k) = b0*pink(k-1) + w(k) ...
            + b1*(k>2)*w(k-1) ...
            + b2*(k>3)*w(k-2);
  end

  % normalizar
  m = max(abs(pink));
  if m > 0
    pink = pink / m;
  end
end  