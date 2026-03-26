function create_bypass() 
  model = 'bypass';
  if bdIsLoaded (model)
    close_system(model, 0);
  end 
  new_system(model);
  open_system(model);
  add_block('pisound_library/PiSound Input',[model '/pisound_in']);
  set_param([model '/pisound_in'],'Position',[50 50 150 200]);
  add_block('pisound_library/PiSound Output',[model '/pisound_out']);
  set_param([model '/pisound_out'],'Position',[300 50 400 200]);

  % Conectar puertos(1 : Left, 2 : Right, 3 : MidiBytes, 4 : MidiCount)
  add_line(model,'pisound_in/1','pisound_out/1');
  add_line(model,'pisound_in/2','pisound_out/2');
  add_line(model,'pisound_in/3','pisound_out/3');
  add_line(model,'pisound_in/4','pisound_out/4');

  % Model Configuration Parameters 
  set_param(model,'SolverType','Fixed-step');
  set_param(model,'FixedStep','auto');
  set_param(model,'Solver','FixedStepDiscrete');
  set_param(model,'StopTime','inf');

  % Code Generation Settings (CRITICAL FOR MAC)
  set_param(model,'SystemTargetFile','ert.tlc');
  set_param(model,'GenCodeOnly','on');
  set_param(model,'GenerateSampleERTMain','on');
  set_param(model,'SupportNonInlinedSFcns','on');
  set_param(model,'Toolchain','Automatically locate an installed toolchain');

  save_system(model);
  fprintf('Model created: %s.slx\n',model);
  fprintf('To generate code: slbuild(''%s'')\n',model);
end