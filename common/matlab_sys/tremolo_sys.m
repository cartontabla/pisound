classdef tremolo_sys < matlab.System
  % Tremolo: y(n)=x(n)*g(n), g in [1-Depth, 1]
  % LFO wavetable; L and R can have phase offset via iPhase

  properties(Nontunable)
    N  = 128;
    Fs = 48000;
    TableL = 1024;
  end

  properties
    Rate = 5;          % Hz
    Depth = 0.5;       % 0..1
    iPhase = 0;        % rad (offset on LEFT LFO)
    WaveSel = uint8(1);% 1..5: sine/square/tri/sawUp/sawDown
  end

  properties(DiscreteState)
    Phase;             % cycles [0,1)
  end

  properties(Access=private)
    WT;                % [TableL x 5]
  end

  methods(Access=protected)

    function setupImpl(obj)
      obj.Phase = 0;

      L = max(2, double(obj.TableL));
      p = (0:L-1)'/L;

      sine    = sin(2*pi*p);
      square  = 2*(p < 0.5) - 1;
      tri     = 1 - 4*abs(p - 0.5);
      sawUp   = 2*p - 1;
      sawDown = 1 - 2*p;

      obj.WT = [sine square tri sawUp sawDown];
    end

    function [outL,outR] = stepImpl(obj, inL, inR)
      N = obj.N;
      outL = zeros(N,1);
      outR = zeros(N,1);

      % select wavetable
      sel = double(obj.WaveSel);
      if sel < 1, sel = 1; elseif sel > 5, sel = 5; end
      tbl = obj.WT(:, sel);
      L   = size(obj.WT,1);

      % LFO phase increment (cycles/sample)
      inc = double(obj.Rate)/double(obj.Fs);
      ph  = double(obj.Phase);

      % phase offset for LEFT only
      twoPi = 2*pi;
      phOff = mod(double(obj.iPhase), twoPi)/twoPi; % cycles

      D = min(max(double(obj.Depth),0),1);
      a = 0.5*D;
      b = 1 - 0.5*D;  % g = b + a*lfo  => [1-D, 1]

      for i = 1:N
        % --- R LFO (no offset)
        vR = wt_read_lin(tbl, L, ph);
        gR = b + a*vR;

        % --- L LFO (with offset)
        phL = ph + phOff; if phL >= 1, phL = phL - 1; end
        vL = wt_read_lin(tbl, L, phL);
        gL = b + a*vL;

        outL(i) = inL(i) * gL;
        outR(i) = inR(i) * gR;

        ph = ph + inc; if ph >= 1, ph = ph - 1; end
      end

      obj.Phase = ph;
    end

    function resetImpl(obj)
      obj.Phase = 0;
    end

    function sts = getSampleTimeImpl(obj)
      Ts = double(obj.N)/double(obj.Fs);
      sts = createSampleTime(obj,'Type','Discrete','SampleTime',Ts);
    end

    function [sz1, sz2] = getOutputSizeImpl(obj)
      sz1 = [obj.N 1]; sz2 = [obj.N 1];
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

    function n = getNumInputsImpl(~),  n = 2; end
    function n = getNumOutputsImpl(~), n = 2; end
  end
end

function v = wt_read_lin(tbl, L, ph) %#codegen
% ph in cycles [0,1)
idx = ph * L;          % 0..L
k   = floor(idx);      % 0..L-1
f   = idx - k;
p1  = k + 1; if p1 > L, p1 = 1; end
p2  = p1 + 1; if p2 > L, p2 = 1; end
v = tbl(p1) + f*(tbl(p2) - tbl(p1));
end
