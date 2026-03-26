function create_doubling()
  % CREATE_DOUBLING Create Doubling Effect Model for PiSound
  % 
  % Architecture:
  %   Input → Delay Line → Mixer (with depth) → L/R Stereo (with spread) → Output
  %
  % Parameters (tunable via control system):
  %   - Delay_ms:  0-100 ms (delay time)
  %   - Depth:     0-1    (wet mix amount)
  %   - Spread:    0-1    (L/R stereo spread)
  %   - Bypass:    0 or 1 (bypass effect)

  model = 'doubling';
  
  % Ensure pisound_library is loaded first
  if ~bdIsLoaded('pisound_library')
    load_system('../common/pisound_library.slx');
  end
  
  % Close if already open
  if bdIsLoaded(model)
    close_system(model, 0);
  end
  
  % Create new system
  new_system(model);
  open_system(model);
  
  % ================================================================
  % INPUT/OUTPUT BLOCKS
  % ================================================================
  add_block('pisound_library/PiSound Input', [model '/pisound_in'], ...
    'Position', [50, 50, 150, 200]);
  add_block('pisound_library/PiSound Output', [model '/pisound_out'], ...
    'Position', [900, 50, 1000, 200]);
  
  % ================================================================
  % INPUT CONDITIONING (normalize int32 to double)
  % ================================================================
  gain_norm = 1 / (2^31 - 1);
  
  add_block('simulink/Commonly Used Blocks/Gain', ...
    [model '/Gain_In_L'], 'Position', [200, 60, 250, 100]);
  set_param([model '/Gain_In_L'], 'Gain', num2str(gain_norm));
  set_param([model '/Gain_In_L'], 'OutDataTypeStr', 'double');
  
  add_block('simulink/Commonly Used Blocks/Gain', ...
    [model '/Gain_In_R'], 'Position', [200, 130, 250, 170]);
  set_param([model '/Gain_In_R'], 'Gain', num2str(gain_norm));
  set_param([model '/Gain_In_R'], 'OutDataTypeStr', 'double');
  
  % ================================================================
  % EFFECT: DOUBLING 
  % ================================================================
  
  % Constant delay (simplest approach for Simulink)
  % Delay block (unit delay for now, can be enhanced to variable delay)
  add_block('simulink/Discrete/Unit Delay', ...
    [model '/Delay_L'], 'Position', [350, 60, 400, 100]);
  
  % Depth gain (controls wet level)
  add_block('simulink/Commonly Used Blocks/Gain', ...
    [model '/Gain_Depth'], 'Position', [450, 60, 500, 100]);
  set_param([model '/Gain_Depth'], 'Gain', 'Depth');
  
  % Mix dry + wet (sum block)
  add_block('simulink/Math Operations/Sum', ...
    [model '/Mix'], 'Position', [550, 40, 600, 100]);
  set_param([model '/Mix'], 'IconShape', 'round');
  set_param([model '/Mix'], 'Inputs', '++');
  
  % Spread control for L channel: (1 - Spread/2)
  add_block('simulink/Commonly Used Blocks/Gain', ...
    [model '/Gain_L_Spread'], 'Position', [650, 40, 700, 100]);
  set_param([model '/Gain_L_Spread'], 'Gain', '(1 - Spread/2)');
  
  % Spread control for R channel: (1 + Spread/2)  
  add_block('simulink/Commonly Used Blocks/Gain', ...
    [model '/Gain_R_Spread'], 'Position', [650, 110, 700, 170]);
  set_param([model '/Gain_R_Spread'], 'Gain', '(1 + Spread/2)');
  
  % ================================================================
  % OUTPUT CONDITIONING (scale back to int32)
  % ================================================================
  scale_out = 2^31 - 1;
  
  add_block('simulink/Commonly Used Blocks/Gain', ...
    [model '/Gain_Out_L'], 'Position', [750, 40, 800, 100]);
  set_param([model '/Gain_Out_L'], 'Gain', num2str(scale_out));
  
  add_block('simulink/Commonly Used Blocks/Gain', ...
    [model '/Gain_Out_R'], 'Position', [750, 110, 800, 170]);
  set_param([model '/Gain_Out_R'], 'Gain', num2str(scale_out));
  
  % Convert to int32
  add_block('simulink/Commonly Used Blocks/Data Type Conversion', ...
    [model '/DTC_Out_L'], 'Position', [850, 40, 900, 100]);
  set_param([model '/DTC_Out_L'], 'OutDataTypeStr', 'int32');
  
  add_block('simulink/Commonly Used Blocks/Data Type Conversion', ...
    [model '/DTC_Out_R'], 'Position', [850, 110, 900, 170]);
  set_param([model '/DTC_Out_R'], 'OutDataTypeStr', 'int32');
  
  % ================================================================
  % CONNECTIONS
  % ================================================================
  % Input normalization
  add_line(model, 'pisound_in/1', 'Gain_In_L/1');
  add_line(model, 'pisound_in/2', 'Gain_In_R/1');
  
  % Left channel effect
  add_line(model, 'Gain_In_L/1', 'Delay_L/1');
  add_line(model, 'Delay_L/1', 'Gain_Depth/1');
  
  % Mix (input + delayed with depth)
  add_line(model, 'Gain_In_L/1', 'Mix/1');
  add_line(model, 'Gain_Depth/1', 'Mix/2');
  
  % Spread for L and R
  add_line(model, 'Mix/1', 'Gain_L_Spread/1');
  add_line(model, 'Mix/1', 'Gain_R_Spread/1');
  
  % Output scaling
  add_line(model, 'Gain_L_Spread/1', 'Gain_Out_L/1');
  add_line(model, 'Gain_R_Spread/1', 'Gain_Out_R/1');
  
  % Convert to int32
  add_line(model, 'Gain_Out_L/1', 'DTC_Out_L/1');
  add_line(model, 'Gain_Out_R/1', 'DTC_Out_R/1');
  
  % Output
  add_line(model, 'DTC_Out_L/1', 'pisound_out/1');
  add_line(model, 'DTC_Out_R/1', 'pisound_out/2');
  
  % ================================================================
  % MODEL CONFIGURATION
  % ================================================================
  set_param(model, 'SolverType', 'Fixed-step');
  set_param(model, 'FixedStep', 'auto');
  set_param(model, 'Solver', 'FixedStepDiscrete');
  set_param(model, 'StopTime', 'inf');
  
  % Code generation
  set_param(model, 'SystemTargetFile', 'ert.tlc');
  set_param(model, 'GenCodeOnly', 'on');
  set_param(model, 'GenerateSampleERTMain', 'on');
  set_param(model, 'SupportNonInlinedSFcns', 'on');
  set_param(model, 'Toolchain', 'Automatically locate an installed toolchain');
  
  % Enable portable word sizes for cross-platform compatibility
  % This allows compilation on ARM despite being generated on Mac
  set_param(model, 'PortableWordSizes', 'on');
  
  % ================================================================
  % MODEL WORKSPACE PARAMETERS
  % ================================================================
  % Define tunable parameters in model workspace
  hws = get_param(model, 'ModelWorkspace');
  hws.assignin('Depth', 0.5);      % 0-1
  hws.assignin('Spread', 0.0);     % 0-1 (0=mono, 1=full stereo)
  hws.assignin('Delay_ms', 50);    % 0-100 ms (converted to samples)
  hws.assignin('Bypass', 0);       % 0 or 1
  
  % Save model
  save_system(model, [model '.slx']);
  
  fprintf('✓ Doubling model created: %s.slx\n', model);
  fprintf('  Parameters: Depth, Spread, Delay_ms, Bypass\n');
end
