classdef osc_sys < matlab.System
    % osc_sys System object

    % Public, non-tunable properties
    properties(Nontunable)
        N = 256; % buffer size
    end

    % Public, tunable properties
    properties
        Fs = 48000;
    end

    methods(Access = protected)
        function icon = getIconImpl(obj)
            % Opción A: Texto
            % icon = {'O S C', 'Generador'};
        
            % Opción B: Imagen (Guarda 'icono_seno.png' junto al .m)
            icon = matlab.system.display.Icon('osc.png');
        end
        
        function setupImpl(obj)
            % Perform one-time calculations

        end

        function [outL, outR] = stepImpl(obj)
            outL = zeros(obj.N, 1);
            outR = zeros(obj.N, 1);
        end

        function resetImpl(obj)
            % Initialize / reset internal properties
        end
    end
end