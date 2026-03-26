classdef osc_proto_sys < matlab.System
% osc_proto_sys
% OSC communication block
%
% Inputs:
%   u : [N x 1] double   data to publish
%
% Outputs:
%   y : [M x 1] double   received controls

  % properties(Nontunable)
  %   PublishLength = 2     % number of values sent
  %   ControlLength = 1     % number of values received
  % end

  properties(Nontunable, Constant)
    PublishLength = 3     % number of values sent
    ControlLength = 1     % number of values received

    RemoteIP = '"192.168.0.37"'
    RemotePort = 9000
    Decim = 100
    Threshold = 0.002
    Path1 = '"vocoder/env/out"'
    Path2 = '"vocoder/map/out"'
    Path3 = '"vocoder/arf/out"'
  end

  properties(DiscreteState)
    lastTx
    decimCount
    firstTx
  end

  properties(Access=private)
    rxBuffer
    txBuffer
  end

  methods(Access=protected)

    function setupImpl(obj)
      coder.cinclude('osc_proto.h');

      obj.rxBuffer = zeros(obj.ControlLength,1);
      obj.txBuffer = zeros(obj.PublishLength,1);
      obj.lastTx = zeros(obj.PublishLength,1);
      obj.decimCount = int32(0);
      obj.firstTx = true;

      coder.ceval('osc_proto_init', ...
          coder.opaque('const char *', obj.RemoteIP), ...
          int32(obj.RemotePort));
    end

    function y = stepImpl(obj,u)
      coder.cinclude('osc_proto.h');

      n = min(numel(u), obj.PublishLength);

      obj.txBuffer(:) = 0;
      obj.txBuffer(1:n) = u(1:n);

      obj.decimCount = obj.decimCount + int32(1);
      D = int32(obj.Decim);
      if obj.decimCount < D
        y = obj.rxBuffer;
        return;
      end
      obj.decimCount = int32(0);
         
      if obj.firstTx
        sendNow = true;
      else
        dmax = max(abs(obj.txBuffer(1:n) - obj.lastTx(1:n)));
        sendNow = (dmax >= obj.Threshold);
      end
          
      if sendNow
        % Se asume que osc_proto_send(basePath, data, n) publicará:
        %   basePath/1 = data(1)
        %   basePath/2 = data(2)
        %   ...
        for k = 1:n
            if(k == 15)
          coder.ceval('osc_proto_send', obj.getPath(k), ...
              coder.rref(obj.txBuffer(k)), int32(1));
            end
        end

        obj.lastTx(1:n) = obj.txBuffer(1:n);
        obj.firstTx = false;
      end

      y = obj.rxBuffer;
    end

    function resetImpl(obj)
      obj.rxBuffer(:)   = 0;
      obj.txBuffer(:)   = 0;
      obj.lastTx(:)     = 0;
      obj.decimCount    = int32(0);
      obj.firstTx       = true;
    end

    function [sz, dt, cp] = getDiscreteStateSpecificationImpl(obj, name)
      switch name
        case 'lastTx'
          sz = [obj.PublishLength 1];
          dt = 'double';
          cp = false;
    
        case 'decimCount'
          sz = [1 1];
          dt = 'int32';
          cp = false;
    
        case 'firstTx'
          sz = [1 1];
          dt = 'logical';
          cp = false;
    
        otherwise
          error('Unknown discrete state: %s', name);
        end
    end

    function num = getNumInputsImpl(~)
      num = 1;
    end

    function num = getNumOutputsImpl(~)
      num = 1;
    end

    function sz = getOutputSizeImpl(obj)
      sz = [obj.ControlLength 1];
    end
    
    function sz = getInputSizeImpl(obj)
      sz = [obj.PublishLength 1];
    end

    function [dt] = getInputDataTypeImpl(~)
      dt = 'double';
    end

    function [cp] = isInputComplexImpl(~)
      cp = false;
    end

    function [fs] = isInputFixedSizeImpl(~)
      fs = true;
    end

    function [dt] = getOutputDataTypeImpl(~)
      dt = 'double';
    end
    
    function [cp] = isOutputComplexImpl(~)
      cp = false;
    end
    
    function [fs] = isOutputFixedSizeImpl(~)
      fs = true;
    end
  end

  methods(Access=private)
    function p = getPath(obj, k)
      switch k
        case 1
          p = coder.opaque('const char *', obj.Path1);
        case 2
          p = coder.opaque('const char *', obj.Path2);
        case 3
          p = coder.opaque('const char *', obj.Path3);
        otherwise
          p = coder.opaque('const char *', '""');
      end
    end
  end

end