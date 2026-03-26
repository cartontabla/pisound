classdef vco_sys < matlab.System
  % vco_sys  VCO wavetable estéreo (LEGO puro)
  % Entradas:
  %   A       : amplitud (double)
  %   F       : Hz (double)
  %   iPhase  : rad, offset SOLO canal L (double)
  %   WaveSel : 1..10 (uint8)
  %   Mode    : 0=bipolar [-1,1], 1=unipolar [0,1] (uint8)
  %
  % Salidas: outL, outR (double Nx1)

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

      obj.WaveTables = zeros(obj.OutMaxL, 10);
      obj.TablesSize = zeros(1, 10);

      L = min(obj.InFixL, obj.OutMaxL);
      p = (0:L-1)'/L;

      sine    = sin(2*pi*p);
      square  = 2*(p < 0.5) - 1;
      tri     = 1 - 4*abs(p - 0.5);
      sawUp   = 2*p - 1;
      sawDown = 1 - 2*p;

      obj.TablesSize(1:5) = L;
      obj.WaveTables(1:L,1:5) = [sine square tri sawUp sawDown];

      WTc = custom_wavetables(L);        % [L x 5]
      obj.WaveTables(1:L,6:10) = WTc;
      obj.TablesSize(6:10) = L;
    end

    function [outL,outR] = stepImpl(obj, A, F, iPhase, waveSel, mode)

      % --- WaveSel seguro (NO double->uint8)
      ws = uint8(waveSel);
      if ws < uint8(1)
        ws = uint8(1);
      elseif ws > uint8(10)
        ws = uint8(10);
      end
      sel = double(ws);

      L = double(obj.TablesSize(sel));
      if L < 2, L = 2; end
      tbl = obj.WaveTables(:, sel);

      N = obj.N;
      outL = zeros(N,1);
      outR = zeros(N,1);

      inc = double(F) / double(obj.Fs);
      ph  = double(obj.Phase);

      phOff = mod(double(iPhase), 2*pi) / (2*pi); % cycles

      m = uint8(mode) ~= 0;

      for i = 1:N
        vR = wt_read_lin(tbl, L, ph);
        vL = wt_read_lin(tbl, L, ph + phOff);

        switch m
          case 0  % bipolar
            % v = v
          case 1  % unipolar 0..1
            vR = 0.5*(vR + 1);
            vL = 0.5*(vL + 1);
          case 2  % unipolar -1..0 (ejemplo)
            vR = 0.5*(vR - 1);
            vL = 0.5*(vL - 1);
        otherwise
          % fallback seguro
        end

        outR(i) = double(A) * vR;
        outL(i) = double(A) * vL;

        ph = ph + inc; if ph >= 1, ph = ph - 1; end
      end

      obj.Phase = ph;
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
    function n = getNumOutputsImpl(~), n = 2; end

    function [s1,s2,s3,s4,s5] = getInputSizeImpl(obj)
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

    function [sz1,sz2] = getOutputSizeImpl(obj), 
      sz1=[obj.N 1]; sz2=[obj.N 1]; 
    end
    
    function [dt1,dt2] = getOutputDataTypeImpl(~), 
      dt1='double'; dt2='double'; 
    end
    
    function [c1,c2] = isOutputComplexImpl(~),  
      c1=false; c2=false; 
    end
    
    function [f1,f2] = isOutputFixedSizeImpl(~),
      f1=true;  f2=true;  
    end
  end
end

function v = wt_read_lin(tbl,L,ph)
  if ph >= 1, ph = ph - floor(ph); end
  idx = ph*L; k = floor(idx); f = idx - k;
  p1 = k + 1; if p1 > L, p1 = 1; end
  p2 = p1 + 1; if p2 > L, p2 = 1; end
  v = tbl(p1) + f*(tbl(p2) - tbl(p1));
end
