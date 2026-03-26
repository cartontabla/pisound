function gen_code(project_name)
    % gen_code Automatiza la configuracion y generacion de codigo C para Raspberry Pi
    % Uso: gen_code('bypass2')
    
    if nargin < 1
        error('Por favor especifica el nombre del proyecto (ej: "bypass2")');
    end

    % Verificar si el modelo esta cargado, si no cargarlo
    if ~bdIsLoaded(project_name)
        load_system(project_name);
    end
    
    fprintf('Configurando modelo "%s" para generacion Raspberry Pi...\n', project_name);

    % Parametros criticos para hardware ARM Raspberry Pi
    set_param(project_name, 'InstructionSetExtensions', 'None');
    set_param(project_name, 'ProdHWDeviceType', 'Generic->32-bit Embedded Processor');
    
    % Comportamiento de parametros (importante para que Gain sea sintonizable)
    set_param(project_name, 'DefaultParameterBehavior', 'Tunable');
    
    % Evitar que Matlab intente compilar localmente (evita errores de Mac sin compilador)
    set_param(project_name, 'GenCodeOnly', 'on');
    
    % Configuraciones adicionales recomendadas para codigo limpio
    set_param(project_name, 'SolverType', 'Fixed-step');
    set_param(project_name, 'FixedStep', 'auto'); % O un paso fijo si se requiere estricto
    set_param(project_name, 'GenerateSampleERTMain', 'off'); % Usamos nuestro custom_main.c
    set_param(project_name, 'SupportNonInlinedSFcns', 'on'); % Necesario para pisound_in/out
    
    % Guardar cambios antes de construir
    save_system(project_name);
    
    fprintf('Generando codigo C...\n');
    slbuild(project_name);
    
    fprintf('Generacion completada. Codigo listo en "%s_ert_rtw/"\n', project_name);
end