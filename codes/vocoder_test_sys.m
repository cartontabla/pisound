classdef vocoder_test_sys < matlab.System
%#codegen

  properties(Nontunable, Constant)
    KTest     (1,1) int32  = int32(5);
    GOn       (1,1) double = 0.0;
    GOff      (1,1) double = -120.0;
    Threshold (1,1) double = 0.5;

    % Parámetros del LFO de prueba
    FsCtrl    (1,1) double = 375.0;   % 48000/128 = 375 frames/s
    FLfo      (1,1) double = 3;     % Hz, lento
    DepthdB   (1,1) double = 24.0;    % excursión en dB
  end

  properties(DiscreteState)
    HoldCount
    ActiveBand
    Dir
    Toggle
    Phase
  end

  methods(Access = protected)

    function g = stepImpl(obj, inMod, k, mode)
      kk = int32(k);
      mm = uint8(mode);

      switch mm

        case uint8(0)
          g = obj.GOff;

        case uint8(1)
          % Solo una banda abierta
          if kk == obj.KTest
            g = obj.GOn;
          else
            g = obj.GOff;
          end

        case uint8(2)
          % Todas menos una abiertas
          if kk == obj.KTest
            g = obj.GOff;
          else
            g = obj.GOn;
          end

        case uint8(3)
          % Diagnóstico selectivo
          if kk == int32(3)
            g = obj.GOn;
          elseif kk == int32(6)
            g = obj.GOff;
          elseif kk == int32(9)
            if mean(inMod) > obj.Threshold
              g = obj.GOn;
            else
              g = obj.GOff;
            end
          else
            g = -24.0;
          end

        case uint8(4)
          % Barrido por bandas
          BandMin = int32(3);
          BandMax = obj.KTest;

          if kk == int32(1)
            obj.HoldCount = obj.HoldCount + int32(1);

            if obj.HoldCount >= int32(200)
              obj.HoldCount = int32(0);
              obj.ActiveBand = obj.ActiveBand + obj.Dir;

              if obj.ActiveBand >= BandMax
                obj.ActiveBand = BandMax;
                obj.Dir = int32(-1);
              elseif obj.ActiveBand <= BandMin
                obj.ActiveBand = BandMin;
                obj.Dir = int32(1);
              end
            end
          end

          if kk == obj.ActiveBand
            g = 0.0;
          else
            g = -120.0;
          end

        case uint8(5)
          % Mute/unmute periódico
          if kk == int32(1)
            obj.HoldCount = obj.HoldCount + int32(1);

            if obj.HoldCount >= int32(200)
              obj.HoldCount = int32(0);

              if obj.Toggle == int32(0)
                obj.Toggle = int32(1);
              else
                obj.Toggle = int32(0);
              end
            end
          end

          if kk == obj.KTest
            if obj.Toggle == int32(1)
              g = obj.GOn;
            else
              g = obj.GOff;
            end
          else
            g = obj.GOff;
          end

case uint8(6)
  % Una sola banda con modulación sinusoidal entre -20 y 0 dB

  if kk == int32(1)
    obj.Phase = obj.Phase + 2.0*pi*obj.FLfo/obj.FsCtrl;
    if obj.Phase >= 2.0*pi
      obj.Phase = obj.Phase - 2.0*pi;
    end
  end

  if kk == obj.KTest
    % a en [0,1]
    %a = 0.5*(1.0 + sin(obj.Phase));
    a = sin(obj.Phase);
    % g entre -20 dB y 0 dB
    g = -12.0 + 24.0*a;
  else
    g = obj.GOff;
  end

          case 7
            if kk == obj.KTest
                g = -12.0;
            else
                g = -120.0;
              end

        otherwise
          g = obj.GOff;
      end
    end

    function resetImpl(obj)
      obj.HoldCount  = int32(0);
      obj.ActiveBand = int32(3);
      obj.Dir        = int32(1);
      obj.Toggle     = int32(1);
      obj.Phase      = 0.0;
    end

    function n = getNumInputsImpl(~)
      n = 3;
    end

    function n = getNumOutputsImpl(~)
      n = 1;
    end

    function [s1,s2,s3] = getInputSizeImpl(~)
      s1 = [128 1];
      s2 = [1 1];
      s3 = [1 1];
    end

    function s = getOutputSizeImpl(~)
      s = [1 1];
    end

    function [d1,d2,d3] = getInputDataTypeImpl(~)
      d1 = 'double';
      d2 = 'int32';
      d3 = 'uint8';
    end

    function dt = getOutputDataTypeImpl(~)
      dt = 'double';
    end

    function [c1,c2,c3] = isInputComplexImpl(~)
      c1 = false;
      c2 = false;
      c3 = false;
    end

    function c = isOutputComplexImpl(~)
      c = false;
    end

    function [f1,f2,f3] = isInputFixedSizeImpl(~)
      f1 = true;
      f2 = true;
      f3 = true;
    end

    function f = isOutputFixedSizeImpl(~)
      f = true;
    end

    function names = getInputNamesImpl(~)
      names = ["inMod","k","mode"];
    end

    function names = getOutputNamesImpl(~)
      names = "g";
    end

    function flag = supportsMultipleInstanceImpl(~)
      flag = true;
    end

    function [sz,dt,cp] = getDiscreteStateSpecificationImpl(~, name)
      switch name
        case 'HoldCount'
          sz = [1 1];
          dt = 'int32';
          cp = false;
        case 'ActiveBand'
          sz = [1 1];
          dt = 'int32';
          cp = false;
        case 'Dir'
          sz = [1 1];
          dt = 'int32';
          cp = false;
        case 'Toggle'
          sz = [1 1];
          dt = 'int32';
          cp = false;
        case 'Phase'
          sz = [1 1];
          dt = 'double';
          cp = false;
        otherwise
          error('Unknown discrete state: %s', name);
      end
    end

  end
end