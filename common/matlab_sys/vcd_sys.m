classdef vcd_sys < matlab.System
% vcd_sys  Línea de retardo estéreo con 3 modos (comb FIR / comb IIR / all-pass)
%         Frame-based, buffer circular, retardo fraccionario (interp. lineal)
%
% Mode 0 (Pure Delay): y(n) = x(n-D) 
% Modo 1 (Comb FIR):  y(n) = x(n) + g x(n-D)
% Modo 2 (Comb IIR):  y(n) = x(n) + g y(n-D)       |g|<1
% Modo 3 (All-pass):  y(n) = -g x(n) + x(n-D) + g y(n-D)   |g|<1
%
% Parámetros:
%   Delay : retardo (s), puede variar en tiempo
%   MaxDelay : retardo máximo (s) para dimensionar el buffer
%   Mode  : 0,1,2,3
%   g     : coeficiente (feedback / all-pass) [-1, 1]
%
% Entradas:
%   inL, inR : [N x 1] double
% Salidas:
%   outL, outR : [N x 1] double

  properties(Nontunable)
    N         (1,1) double {mustBeInteger,mustBePositive} = 128;
    Fs        (1,1) double {mustBePositive} = 48000;
    MaxDelay  (1,1) double {mustBePositive} = 3.0; % s
  end

  properties
    Mode  (1,1) uint8  = uint8(1);   % 1=FIR, 2=IIR, 3=All-pass
    Delay (1,1) double = 0.10;       % s (tunable)
    g     (1,1) double = 0.5;        % coef. (tunable)
  end


  properties(DiscreteState)
    w  % write index (1..L)
  end

  properties(Access=private)
    L
    xBufL
    xBufR
    yBufL
    yBufR
  end

  methods(Access=protected)

    function icon = getIconImpl(~)
      icon = matlab.system.display.Icon('VCD.png');
    end
    
    function setupImpl(obj)
      % L constante para codegen (deriva de Nontunable Fs y MaxDelay)
      Lc = coder.const(ceil(obj.MaxDelay * obj.Fs) + 2);
      obj.L = Lc;

      obj.xBufL = zeros(Lc,1);
      obj.xBufR = zeros(Lc,1);
      obj.yBufL = zeros(Lc,1);
      obj.yBufR = zeros(Lc,1);
    
      obj.w = 1;
    end

    function resetImpl(obj)
      obj.xBufL(:) = 0;  obj.xBufR(:) = 0;
      obj.yBufL(:) = 0;  obj.yBufR(:) = 0;
      obj.w = 1;
    end

    function [outL,outR] = stepImpl(obj, inL, inR, mode, time, repeats)
      N = obj.N;
      outL = zeros(N,1);
      outR = zeros(N,1);

      % Entradas de control (escalares)
      Mode = uint8(mode);
      g = max(-1,min(1,double(repeats)));  % [-1,1]

      if Mode ~= uint8(1)
        if g >= 1,  g = 0.999; end
        if g <= -1, g = -0.999; end
      end

      Dmax = obj.L - 2;

      for n = 1:N
        xL = inL(n); xR = inR(n);

        % Retardo en muestras (fraccionario permitido)
        Delay = double(time(n));
        D = Delay * obj.Fs;

        % clamp robusto
        if D < 1, D = 1; end
        if D > Dmax, D = Dmax; end

        Di = floor(D);
        mu = D - Di;

        r0 = obj.w - Di;
        r0 = 1 + mod(r0-1, obj.L);
        r1 = r0 - 1; 
        if r1 < 1, r1 = obj.L; end

        xdelL = (1-mu)*obj.xBufL(r0) + mu*obj.xBufL(r1);
        xdelR = (1-mu)*obj.xBufR(r0) + mu*obj.xBufR(r1);
        ydelL = (1-mu)*obj.yBufL(r0) + mu*obj.yBufL(r1);
        ydelR = (1-mu)*obj.yBufR(r0) + mu*obj.yBufR(r1);

        switch Mode
          case uint8(0) % pure delay
            yL = xdelL;
            yR = xdelR;
          case uint8(1) % comb FIR
            yL = xL + g* xdelL;
            yR = xR + g* xdelR;
          case uint8(2) % comb IIR
            yL = xL + g* ydelL;
            yR = xR + g* ydelR;
          case uint8(3) % all-pass
            yL = -g*xL + xdelL + g* ydelL;
            yR = -g*xR + xdelR + g* ydelR;
          otherwise
            yL = xL; yR = xR;
        end

        outL(n) = yL;
        outR(n) = yR;

        obj.xBufL(obj.w) = xL;  obj.xBufR(obj.w) = xR;
        obj.yBufL(obj.w) = yL;  obj.yBufR(obj.w) = yR;

        obj.w = obj.w + 1;
        if obj.w > obj.L, obj.w = 1; end
      end
    end

    function num = getNumInputsImpl(~)
      num = 5; % inL, inR, mode, time, repeat(g)
    end

    function [sz1,sz2,sz3,sz4,sz5] = getInputSizeImpl(obj)
      sz1 = [obj.N 1];
      sz2 = [obj.N 1];
      sz3 = [1 1];
      sz4 = [obj.N 1];
      sz5 = [1 1];
    end

    function [dt1,dt2,dt3,dt4,dt5] = getInputDataTypeImpl(~)
      dt1 = 'double';
      dt2 = 'double';
      dt3 = 'uint8';
      dt4 = 'double';
      dt5 = 'double';
    end

    function sts = getSampleTimeImpl(obj)
      Ts  = double(obj.N)/double(obj.Fs);
      sts = createSampleTime(obj,'Type','Discrete','SampleTime',Ts);
    end
    
    function [c1,c2,c3,c4,c5] = isInputComplexImpl(~)
      c1=false; c2=false; c3=false; c4=false; c5=false;
    end

    function [f1,f2,f3,f4,f5] = isInputFixedSizeImpl(~)
      f1=true; f2=true; f3=true; f4=true; f5=true;
    end

    function n = getNumOutputsImpl(~), n = 2; end

    function [sz1,sz2] = getOutputSizeImpl(obj)
      sz1 = [obj.N 1]; sz2 = [obj.N 1];
    end

    function [dt1,dt2] = getOutputDataTypeImpl(~)
      dt1 = 'double'; dt2 = 'double';
    end
    
    function [c1,c2] = isOutputComplexImpl(~), c1=false; c2=false; end
    function [f1,f2] = isOutputFixedSizeImpl(~), f1=true; f2=true; end

    function [sz, dt, cp] = getDiscreteStateSpecificationImpl(obj, name)
    % name: nombre del estado discreto que Simulink consulta (char/string)
    
      switch char(name)
        case 'w'
          sz = [1 1];
          dt = 'double';   % o 'uint32' si lo prefieres, pero mantén coherencia con tu código
          cp = false;      % no complejo
        otherwise
          % Para cualquier otro estado no esperado
          sz = [0 0];
          dt = 'double';
          cp = false;
      end
    end



  end
end
