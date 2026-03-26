classdef lfo_sys < matlab.System
  properties(Nontunable)
    N  = 128;
    Fs = 48000;
  end

  properties
    Rate = 5;                 % Hz
    Depth = 0.8;              % 0..1
    StereoPhase = 0;          % rad (offset LEFT)
    WaveSel = uint8(1);       % 1..10
  end

  properties(DiscreteState)
    Phase;                    % cycles [0,1)
  end

  properties(Access=private)
    WaveTables;
    TablesSize;
    OutMaxL = 4096;
    InFixL  = 1024;
  end

  methods(Access=protected)
    
    function icon = getIconImpl(~)
      icon = matlab.system.display.Icon('osc.png');
    end

    function setupImpl(obj)
      obj.Phase = 0;
      % (aquí copias tu init de tablas de osc_sys: built-in + custom)
      % obj.WaveTables, obj.TablesSize ...
    end

    function [gL,gR] = stepImpl(obj)
      N = obj.N;
      gL = zeros(N,1); gR = zeros(N,1);

      % tabla seleccionada (igual que osc_sys)
      sel = double(obj.WaveSel); sel = min(max(sel,1),10);
      L   = double(obj.TablesSize(sel)); L = max(L,2);
      tbl = obj.WaveTables(:,sel);

      inc = double(obj.Rate)/double(obj.Fs);     % cycles/sample
      ph  = double(obj.Phase);

      phOff = mod(double(obj.StereoPhase),2*pi)/(2*pi); % cycles

      D = min(max(double(obj.Depth),0),1);
      a = 0.5*D;
      b = 1 - 0.5*D;

      for i=1:N
        % R: LFO bipolar
        vR = wt_read_lin(tbl,L,ph);
        gR(i) = b + a*vR;

        % L: offset
        phL = ph + phOff; if phL>=1, phL=phL-1; end
        vL = wt_read_lin(tbl,L,phL);
        gL(i) = b + a*vL;

        ph = ph + inc; if ph>=1, ph=ph-1; end
      end

      obj.Phase = ph;
    end

    function [sz, dt, cp] = getDiscreteStateSpecificationImpl(~, name)
      if strcmp(name,'Phase')
        sz = [1 1];      % escalar
        dt = 'double';   % tipo
        cp = false;      % no complejo
      end
    end
    
    function sts = getSampleTimeImpl(obj)
      Ts = double(obj.N)/double(obj.Fs);
      sts = createSampleTime(obj,'Type','Discrete','SampleTime',Ts);
    end

    function [sz1,sz2] = getOutputSizeImpl(obj)
      sz1=[obj.N 1]; sz2=[obj.N 1];
    end
    function [dt1,dt2] = getOutputDataTypeImpl(~), dt1='double'; dt2='double'; end
    function [c1,c2] = isOutputComplexImpl(~), c1=false; c2=false; end
    function [f1,f2] = isOutputFixedSizeImpl(~), f1=true; f2=true; end
    function n=getNumInputsImpl(~), n=0; end
    function n=getNumOutputsImpl(~), n=2; end
  end
end

function v = wt_read_lin(tbl,L,ph)
idx = ph*L; k=floor(idx); f=idx-k;
p1=k+1; if p1>L, p1=1; end
p2=p1+1; if p2>L, p2=1; end
v = tbl(p1) + f*(tbl(p2)-tbl(p1));
end
