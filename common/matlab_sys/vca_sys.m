classdef vca_sys < matlab.System
  % vca_sys  VCA estéreo frame-based (gain vectorial)
  % Entradas:
  %   inL, inR : audio (double, Nx1)
  %   gain     : vector (double, Nx1)  (lineal si mode=0, dB si mode=1)
  %   mode     : escalar (double/uint8) 0/1

  properties(Nontunable)
    N  = 128;
    Fs = 48000;
    bypass (1,1) uint8 = 0;  % 0=fx, 1=no fx
  end

  methods(Access=protected)

    function icon = getIconImpl(~)
      icon = matlab.system.display.Icon('VCA.png');
    end

    function [outL,outR] = stepImpl(obj, inL, inR, gain, mode)
      g = double(gain);      % g es Nx1

      m = uint8(mode ~= 0);
      if m ~= 0
        g = 10.^(g/20);      % dB -> lineal, elemento a elemento
      end

      bp = (obj.bypass ~= 0);   % bp es logical
      if bp
        outL = inL;  % Bypass: no effect on output
        outR = inR;
      else
          outL = inL .* g;       % elemento a elemento
          outR = inR .* g;
      end
    end

    function sts = getSampleTimeImpl(obj)
      Ts  = double(obj.N)/double(obj.Fs);
      sts = createSampleTime(obj,'Type','Discrete','SampleTime',Ts);
    end

    function n = getNumInputsImpl(~),  n = 4; end
    function n = getNumOutputsImpl(~), n = 2; end

    function [sz1,sz2,sz3,sz4] = getInputSizeImpl(obj)
      sz1 = [obj.N 1];
      sz2 = [obj.N 1];
      sz3 = [obj.N 1];   % gain vectorial
      sz4 = [1 1];       % mode escalar
    end

    function [dt1,dt2,dt3,dt4] = getInputDataTypeImpl(~)
      dt1='double'; dt2='double'; dt3='double'; dt4='double';
    end

    function [c1,c2,c3,c4] = isInputComplexImpl(~), 
        c1=false; c2=false; c3=false; c4=false; 
    end
    
    function [f1,f2,f3,f4] = isInputFixedSizeImpl(~), 
        f1=true; f2=true; f3=true; f4=true; 
    end

    function [sz1,sz2] = getOutputSizeImpl(obj), 
        sz1=[obj.N 1]; sz2=[obj.N 1]; 
    end
    
    function [dt1,dt2] = getOutputDataTypeImpl(~), 
        dt1='double'; dt2='double'; 
    end
    function [c1,c2]   = isOutputComplexImpl(~),  
        c1=false; c2=false; 
    end
    
    function [f1,f2]   = isOutputFixedSizeImpl(~),
        f1=true;  f2=true;  
    end

    function flag = supportsMultipleInstanceImpl(~)
        flag = true;
    end
  end
end
