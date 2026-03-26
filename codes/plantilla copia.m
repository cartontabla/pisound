classdef plantilla < matlab.System
    % Ingeniería de Audio.
    % Digital Signal Processing
    %
    % Esta plantilla incluye el conjunto mínimo de funciones requeridas
    % para definir un System Object con variables de estado.

    % Public, non-tunable properties
    properties(Nontunable)
      Fs=48000;       % Frec. de muestreo (Hz)
    end
    
    % Public, tunable properties
    properties
      
    end
    
    %Variables de estado
    properties(DiscreteState)
 
    end

    % Variables de estado OCULTAS, NO MODIFICABLES durante la ejecución.
    properties(Access = private, Nontunable)
      N;              % Tamaño del frame de muestras de audio (canales L y R)
    end  
    
    
    % Pre-computed constants
    properties(Access = private)

    end

    methods(Access = protected)
        % Se ejecuta una sola vez, al inicio.
        function setupImpl(obj,inR,inL)
            % Se utiliza para inicializar variables de estado.
            % inR ==> variable para almacenar el frame de entrada del canal R
            % inL ==> variable para almacenar el frame de entrada del canal L
            % obj ==> distintas variables de estado
            
            obj.N=length(inR);        % Variable del tamaño del frame
        end
 
        %Lazo principal de ejecución (frame a frame)
        function [outL, outR] = stepImpl(obj,inL,inR)
            % ouL ==> variable para almacenar el frame de salida del canal L
            % ouR ==> variable para almacenar el frame de salida del canal R
            % obj ==> distintas variables de estado
 
            % Algoritmo.
            % Calcula las variables outL y outR en función de inL e inR
            % y en función de las variables de estado.
            
            outL=inL;   % Bypass canal L
            outR=inR;   % Bypass canal R
                        
        end
        
        %Inicializa o resetea las variables de estado
        function resetImpl(obj)
            
        end
    end
    
end

