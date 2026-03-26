classdef vcd_mono_sys < matlab.System
% vcd_mono_sys  Línea de retardo mono con 4 modos (delay / comb FIR / comb IIR / all-pass)
%               Frame-based, buffer circular, retardo fraccionario (interp. lineal)
%
% Mode 0 (Pure Delay): y(n) = x(n-D)
% Mode 1 (Comb FIR):   y(n) = x(n) + g x(n-D)
% Mode 2 (Comb IIR):   y(n) = x(n) + g y(n-D)       |g|<1
% Mode 3 (All-pass):   y(n) = -g x(n) + x(n-D) + g y(n-D)   |g|<1
%
% Entradas:
%   in     : [N x 1] double
%   mode   : [1 x 1] uint8 (0..3)
%   time   : [N x 1] double (s)  (puede variar por muestra dentro del frame)
%   repeats: [1 x 1] double (g)  [-1, 1]
%
% Salida:
%   out    : [N x 1] double

  properties(Nontunable)
    N         (1,1) double {mustBeInteger,mustBePositive} = 128;
    Fs        (1,1) double {mustBePositive} = 48000;
    MaxDelay  (1,1) double {mustBePositive} = 10.0; % s
  end

  properties(DiscreteState)
    w  % write index (1..L)
  end

  properties(Access=private)
    L
    xBuf
    yBuf
  end

  methods(Access=protected)

    function icon = getIconImpl(~)
      icon = matlab.system.display.Icon('VCD.png');
    end

    function setupImpl(obj)
      % L constante para codegen (deriva de Nontunable Fs y MaxDelay)
      Lc = coder.const(ceil(obj.MaxDelay * obj.Fs) + 2);
      obj.L = Lc;

      obj.xBuf = zeros(Lc,1);
      obj.yBuf = zeros(Lc,1);

      obj.w = 1;
    end

    function resetImpl(obj)
      obj.xBuf(:) = 0;
      obj.yBuf(:) = 0;
      obj.w = 1;
    end

    function out = stepImpl(obj, in, mode, time, repeats)
      N = obj.N;
      out = zeros(N,1);

      % Control
      Mode = uint8(mode);
      g = max(-1, min(1, double(repeats))); % [-1,1]

      % estabilidad en IIR / all-pass
      if Mode ~= uint8(1)
        if g >= 1,  g = 0.999; end
        if g <= -1, g = -0.999; end
      end

      Dmax = obj.L - 2;

      for n = 1:N
        x = in(n);

        % Retardo en muestras (fraccionario permitido)
        Delay = double(time(n));     % s
        D = Delay * obj.Fs;          % samples

        % clamp robusto
        if D < 1, D = 1; end
        if D > Dmax, D = Dmax; end

        Di = floor(D);
        mu = D - Di;

        r0 = obj.w - Di;
        r0 = 1 + mod(r0-1, obj.L);
        r1 = r0 - 1;
        if r1 < 1, r1 = obj.L; end

        xdel = (1-mu)*obj.xBuf(r0) + mu*obj.xBuf(r1);
        ydel = (1-mu)*obj.yBuf(r0) + mu*obj.yBuf(r1);

        switch Mode
          case uint8(0) % pure delay
            y = xdel;
          case uint8(1) % comb FIR
            y = x + g*xdel;
          case uint8(2) % comb IIR
            y = x + g*ydel;
          case uint8(3) % all-pass
            y = -g*x + xdel + g*ydel;
          otherwise
            y = x;
        end

        out(n) = y;

        obj.xBuf(obj.w) = x;
        obj.yBuf(obj.w) = y;

        obj.w = obj.w + 1;
        if obj.w > obj.L, obj.w = 1; end
      end
    end

    % --- I/O
    function n = getNumInputsImpl(~),  n = 4; end
    function n = getNumOutputsImpl(~), n = 1; end

    function [sz1,sz2,sz3,sz4] = getInputSizeImpl(obj)
      sz1 = [obj.N 1]; % in
      sz2 = [1 1];     % mode
      sz3 = [obj.N 1]; % time
      sz4 = [1 1];     % repeats(g)
    end

    function [dt1,dt2,dt3,dt4] = getInputDataTypeImpl(~)
      dt1 = 'double';
      dt2 = 'uint8';
      dt3 = 'double';
      dt4 = 'double';
    end

    function [c1,c2,c3,c4] = isInputComplexImpl(~)
      c1=false; c2=false; c3=false; c4=false;
    end

    function [f1,f2,f3,f4] = isInputFixedSizeImpl(~)
      f1=true; f2=true; f3=true; f4=true;
    end

    function sts = getSampleTimeImpl(obj)
      Ts  = double(obj.N)/double(obj.Fs);
      sts = createSampleTime(obj,'Type','Discrete','SampleTime',Ts);
    end

    function sz = getOutputSizeImpl(obj), sz = [obj.N 1]; end
    function dt = getOutputDataTypeImpl(~), dt = 'double'; end
    function c  = isOutputComplexImpl(~), c = false; end
    function f  = isOutputFixedSizeImpl(~), f = true; end

    function names = getInputNamesImpl(~)
      names = ["in","mode","time","repeats"];
    end

    function names = getOutputNamesImpl(~)
      names = "out";
    end

    function [sz, dt, cp] = getDiscreteStateSpecificationImpl(~, name)
      switch char(name)
        case 'w'
          sz = [1 1];
          dt = 'double'; % si prefieres: 'uint32' y ajustas obj.w
          cp = false;
        otherwise
          sz = [0 0]; dt = 'double'; cp = false;
      end
    end

  end
end