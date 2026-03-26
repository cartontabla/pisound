classdef mix_sys < matlab.System
  properties(Nontunable)
    N  = 128;
    Fs = 48000;
    Mode = uint8(0); % 0 lineal, 1 equal-power, 2 independiente, 3 crossfade por muestra
  end

  properties
    SmoothTime = 0.01;
    IndepInDB = false;
  end

  properties(DiscreteState)
    gd1; gd2;
    gw1; gw2;
  end

  properties(Access=private)
    a;
  end

  methods(Access=protected)

    function icon = getIconImpl(~)
      % Asegúrate de que 'mix.png' está en el path o en la misma carpeta.
      icon = matlab.system.display.Icon('mix.png');
    end

    function setupImpl(obj, ~, ~, ~, ~, ~, varargin) 
      obj.a  = obj.computeAlpha_(obj.SmoothTime, obj.Fs);
      obj.gd1 = 1; obj.gd2 = 1;
      obj.gw1 = 0; obj.gw2 = 0;
    end

    function resetImpl(obj)
      obj.gd1 = 1; obj.gd2 = 1;
      obj.gw1 = 0; obj.gw2 = 0;
    end

    function [outL,outR] = stepImpl(obj, inL,inR, wetL,wetR, p1, varargin)

      % Mode 3: crossfade por muestra (p1 puede ser [N x 1])
      if obj.Mode == uint8(3)
        m = double(p1);
        % clamp a [0,1] (escalares o vectores)
        m = min(max(m,0),1);
        
        % equal-power crossfade
        gA = sqrt(1 - m);
        gB = sqrt(m);
        
        outL = gA.*inL  + gB.*wetL;
        outR = gA.*inR  + gB.*wetR;
        
        return;
      end

      if obj.Mode == 2
        p2 = varargin{1};
      else
        p2 = 0; % dummy
      end

      [gd_t, gw_t] = obj.targetGains_(p1, p2);

      outL = zeros(size(inL), 'like', inL);
      outR = zeros(size(inR), 'like', inR);

      a_ = obj.a;

      gd_is_scalar = (numel(gd_t) == 1);
      gw_is_scalar = (numel(gw_t) == 1);

      gd1_ = obj.gd1; gd2_ = obj.gd2;
      gw1_ = obj.gw1; gw2_ = obj.gw2;

      Ns = size(inL,1);  % más robusto que obj.N

      for n = 1:Ns
        if gd_is_scalar, gdtn = gd_t; else, gdtn = gd_t(n); end
        if gw_is_scalar, gwtn = gw_t; else, gwtn = gw_t(n); end

        gd1_ = gd1_ + a_*(gdtn - gd1_);
        gw1_ = gw1_ + a_*(gwtn - gw1_);
        outL(n) = gd1_ * inL(n) + gw1_ * wetL(n);
        
        gd2_ = gd2_ + a_*(gdtn - gd2_);
        gw2_ = gw2_ + a_*(gwtn - gw2_);
        outR(n) = gd2_ * inR(n) + gw2_ * wetR(n);
      end

      obj.gd1 = gd1_; obj.gd2 = gd2_;
      obj.gw1 = gw1_; obj.gw2 = gw2_;
    end

    function num = getNumInputsImpl(obj)
      % inL,inR,wetL,wetR,p1 (+p2 si Mode=2)
      if obj.Mode == 2
        num = 6;
      else
        num = 5;
      end
    end

    function num = getNumOutputsImpl(~)
      num = 2;
    end

    function names = getInputNamesImpl(obj)
      if obj.Mode == 2
        names = ["inL","inR","wetL","wetR","p1","p2"];
      else
        names = ["inL","inR","wetL","wetR","p1"]; % en Mode=3, p1 = wA(n)
      end
    end

    function [s1,s2,s3,s4,s5,s6] = getInputSizeImpl(obj)
      s1 = [obj.N 1];  s2 = [obj.N 1];
      s3 = [obj.N 1];  s4 = [obj.N 1];
    
      s5 = [obj.N 1];          % p1 SIEMPRE vector
    
      if obj.Mode == uint8(2)
        s6 = [obj.N 1];        % p2 vector
      else
        s6 = [0 0];
      end
    end

    function names = getOutputNamesImpl(~)
      names = ["outL","outR"];
    end

  end 

  methods(Access=private)

    function [gd, gw] = targetGains_(obj, p1, p2)
      switch uint8(obj.Mode)
        case 0
          m  = obj.clamp01_(double(p1));
          gd = 1-m;  gw = m;

        case 1
          m  = obj.clamp01_(double(p1));
          gd = cos(0.5*pi*m);
          gw = sin(0.5*pi*m);

        otherwise % 2
          if obj.IndepInDB
            gd = 10.^(double(p1)/20);
            gw = 10.^(double(p2)/20);
          else
            gd = double(p1);
            gw = double(p2);
          end
      end
    end

    function a = computeAlpha_(~, T, Fs)
      if T <= 0
        a = 1;
        return;
      end
      a = 1 - exp(-1/(double(Fs)*double(T)));
    end
    
    function x = clamp01_(~, x)
      x = min(max(x,0),1);
    end

  end
end