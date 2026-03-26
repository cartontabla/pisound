classdef amp_sys < matlab.System
    % System object for amp with Gain and Bypass properties

    % Public, non-tunable properties
    properties(Nontunable)
        Fs = 48000;
    end

    % Public, tunable properties
    properties
        Gain;
        Bypass;
    end

    methods(Access = protected) 
        function setupImpl(obj) % Perform one-time calculations 
            obj.Gain = 1.0;
            obj.Bypass = false;
        end

        function[outL, outR] = stepImpl(obj, inL, inR) 
            % Implement algorithm 
            if obj.Bypass 
                outL = inL;
                outR = inR;
            else 
                outL = inL * obj.Gain;
                outR = inR * obj.Gain;
            end 
        end

        function resetImpl(~) 
            % Initialize / reset internal properties 
        end 
    end

end