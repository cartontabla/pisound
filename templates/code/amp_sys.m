classdef amp_sys < matlab.System
    % bypass_sys System object for bypass with Gain and Bypass properties

    % Public, non-tunable properties
    properties(Nontunable)
    end

    % Public, tunable properties
    properties
        Fs = 48000;
        Gain = 1.0;
        Bypass = false;
    end

    % Pre-computed constants or internal states
    properties(Access = private, Nontunable)
        N; % buffer size
    end

    methods(Access = protected)
        function setupImpl(obj, inL, inR)
            % Perform one-time calculations
            obj.N = length(inL);
        end

        function [outL, outR] = stepImpl(obj, inL, inR)
            % Implement algorithm
            if obj.Bypass
                outL = inL;
                outR = inR;
            else
                outL = inL * obj.Gain;
                outR = inR * obj.Gain;
            end
        end

        function resetImpl(obj)
            % Initialize / reset internal properties
        end
    end
end