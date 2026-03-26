function create_amp() 
  model = 'amp';

  % Cerrar el modelo si está abierto 
  if bdIsLoaded (model) 
      close_system(model, 0);
  end

  % Crear y abrir nuevo modelo new_system(model);
  open_system(model);

  % -- -Añadir bloques principales-- -
  add_block('pisound_library/PiSound Input', [model '/pisound_in'],
              ... 'Position', [ 50, 50, 150, 150 ]);
  add_block('pisound_library/PiSound Output', [model '/pisound_out'],
          ... 'Position', [ 800, 50, 900, 150 ]);

  % -- -Bloques de Acondicionamiento(Input)-- - 
  % Data Type Conversion: int32->double 
  add_block('simulink/Commonly Used Blocks/Data Type Conversion',
          ...[model '/DTC_In_L'],'Position',[ 200, 60, 250, 100 ]);
  add_block('simulink/Commonly Used Blocks/Data Type Conversion',
          ...[model '/DTC_In_R'],'Position',[ 200, 110, 250, 150 ]);
  set_param([model '/DTC_In_L'],'OutDataTypeStr','double');
  set_param([model '/DTC_In_R'],'OutDataTypeStr','double');

  % Gain : Normalizacion(1 / (2 ^ 31 - 1))
  add_block('simulink/Commonly Used Blocks/Gain',
          ...[model '/Gain_Norm_L'],'Position',[ 300, 60, 350, 100 ]);
  add_block('simulink/Commonly Used Blocks/Gain', ...
          ...[model '/Gain_Norm_R'],'Position',[ 300, 110, 350, 150 ]);
  % Note: Use string for gain to ensure high precision 
  % is kept in the model parameter
  set_param([model '/Gain_Norm_L'],'Gain','1/(2^31-1)');
  set_param([model '/Gain_Norm_R'],'Gain', '1/(2^31-1)');

  % -- -Bloque de Procesamiento(MATLAB System)-- -
  add_block('simulink/User-Defined Functions/MATLAB System',
          ...[model '/Audio_Process'],'Position',[ 420, 60, 500, 150 ]);
  set_param([model '/Audio_Process'],'System','amp_sys');

  % -- -Bloques de Re - acondicionamiento(Output)-- - 
  % Gain : Desnormalizacion(2 ^ 31 - 1)
  add_block('simulink/Commonly Used Blocks/Gain',
          ...[model '/Gain_Denorm_L'],'Position',[ 550, 60, 600, 100 ]);
  add_block('simulink/Commonly Used Blocks/Gain', ...
          ...[model '/Gain_Denorm_R'],'Position',[ 550, 110, 600, 150 ]);
  set_param([model '/Gain_Denorm_L'],'Gain','2^31-1');
  set_param([model '/Gain_Denorm_R'],'Gain','2^31-1');

  % Data Type Conversion : double->int32
  add_block('simulink/Commonly Used Blocks/Data Type Conversion',
          ...[model '/DTC_Out_L'],'Position',[ 650, 60, 700, 100 ]);
  add_block('simulink/Commonly Used Blocks/Data Type Conversion',
          ...[model '/DTC_Out_R'],'Position',[ 650, 110, 700, 150 ]);
  set_param([model '/DTC_Out_L'],'OutDataTypeStr','int32');
  set_param([model '/DTC_Out_R'],'OutDataTypeStr','int32');

  % -- -Conectar puertos-- -
  % Canal Izquierdo(1) 
  add_line(model,'pisound_in/1','DTC_In_L/1');
  add_line(model,'DTC_In_L/1','Gain_Norm_L/1');
  add_line(model,'Gain_Norm_L/1','Audio_Process/1');
  % Envia a Input 1 del System Block 
  add_line(model,'Audio_Process/1','Gain_Denorm_L/1');
  % Recibe de Output 1 
  add_line(model,'Gain_Denorm_L/1','DTC_Out_L/1');
  add_line(model,'DTC_Out_L/1','pisound_out/1');

  % Canal Derecho(2) 
  add_line(model,'pisound_in/2','DTC_In_R/1');
  add_line(model,'DTC_In_R/1','Gain_Norm_R/1');
  add_line(model,'Gain_Norm_R/1','Audio_Process/2');
  % Envia a Input 2 del System Block 
  add_line(model,'Audio_Process/2','Gain_Denorm_R/1');
  % Recibe de Output 2 
  add_line(model,'Gain_Denorm_R/1','DTC_Out_R/1');
  add_line(model,'DTC_Out_R/1','pisound_out/2');

  % Conectar MIDI(directo, 3 y 4)
  add_line(model,'pisound_in/3','pisound_out/3');
  add_line(model,'pisound_in/4','pisound_out/4');

  % -- -Configuración para Code Generation-- -
  set_param(model,'SolverType','Fixed-step');
  set_param(model,'FixedStep','auto');
  set_param(model,'Solver','FixedStepDiscrete');
  set_param(model,'SystemTargetFile','ert.tlc');
  % Embedded Coder 
  set_param(model,'TargetLang','C');
  set_param(model,'GenerateSampleERTMain','on');
  set_param(model, 'DefaultParameterBehavior', 'Tunable');

  % Guardar sistema 
  save_system(model);
end
