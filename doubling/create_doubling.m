function create_doubling()
% create_doubling — Cobaya BLE sencilla: ADT/doubler modulado
%
% Arquitectura:
%   Rama 1: dry directo al mezclador
%   Rama 2: vcd_sys en modo 0 (pure delay) con tiempo variable
%           time = delay_base + LFO(depth, rate)
%   Salida: mezcla dry/wet con mix_sys
%
% Controles (ctrl_in, C1..C4):
%   C1 -> mix          [0..1]
%   C2 -> delay_ms     [8..35 ms]
%   C3 -> mod_depth_ms [0..4 ms]
%   C4 -> mod_rate_hz  [0.1..1.5 Hz]
%
% Nota:
%   Esto no pretende ser un doubler comercial "perfecto"; es una cobaya
%   clara para BLE con un sonido suficientemente usable y controles obvios.

  model      = 'doubling';
  SCRIPT_DIR = fileparts(mfilename('fullpath'));
  ROOT_DIR   = fileparts(SCRIPT_DIR);
  COMMON_DIR = fullfile(ROOT_DIR, 'common');
  addpath(COMMON_DIR);
  addpath(fullfile(COMMON_DIR, 'matlab_sys'));

  Fs = 48000;
  N  = 128;
  norm_in   = 1 / (2^31 - 1);
  scale_out = 0.8 * (2^31 - 1); % margen de salida

  if bdIsLoaded(model), close_system(model, 0); end
  new_system(model);
  open_system(model);

  b = @(name) [model '/' name];

  % -----------------------------------------------------------------------
  % CONFIGURACION
  % -----------------------------------------------------------------------
  cs = getActiveConfigSet(model);
  set_param(cs, 'SolverType',             'Fixed-step');
  set_param(cs, 'Solver',                 'FixedStepDiscrete');
  set_param(cs, 'FixedStep',              num2str(N/Fs, '%.15g'));
  set_param(cs, 'StopTime',               'inf');
  set_param(cs, 'SystemTargetFile',       'ert.tlc');
  set_param(cs, 'GenCodeOnly',            'on');
  set_param(cs, 'GenerateSampleERTMain',  'on');
  set_param(cs, 'SupportNonInlinedSFcns', 'on');
  set_param(cs, 'ProdHWDeviceType',       'ARM Compatible->ARM Cortex-A');
  set_param(cs, 'ProdEqTarget',           'on');
  set_param(cs, 'PortableWordSizes',      'on');

  % -----------------------------------------------------------------------
  % I/O
  % -----------------------------------------------------------------------
  add_block('built-in/S-Function', b('ctrl_in'), ...
    'FunctionName', 'ctrl_in', ...
    'Parameters',   sprintf('%d, %d, %d', Fs, N, 4), ...
    'Position',     [50, -40, 130, 15]);

  add_block('built-in/S-Function', b('pisound_in'), ...
    'FunctionName', 'pisound_in', ...
    'Parameters',   sprintf('%d, %d, %d', Fs, N, 0), ...
    'Position',     [50, 80, 140, 210]);

  add_block('built-in/S-Function', b('ctrl_out'), ...
    'FunctionName', 'ctrl_out', ...
    'Parameters',   sprintf('%d, %d, %d', Fs, N, 4), ...
    'Position',     [1090, -40, 1170, 15]);

  add_block('built-in/S-Function', b('pisound_out'), ...
    'FunctionName', 'pisound_out', ...
    'Parameters',   sprintf('%d, %d, %d', Fs, N, 0), ...
    'Position',     [1090, 85, 1170, 215]);

  % -----------------------------------------------------------------------
  % NORMALIZACION DRY
  % -----------------------------------------------------------------------
  add_block('simulink/Commonly Used Blocks/Gain', b('Gain_Dry_L'), ...
    'Gain', num2str(norm_in, '%.10g'), ...
    'OutDataTypeStr', 'double', ...
    'Position', [190, 95, 235, 125]);

  add_block('simulink/Commonly Used Blocks/Gain', b('Gain_Dry_R'), ...
    'Gain', num2str(norm_in, '%.10g'), ...
    'OutDataTypeStr', 'double', ...
    'Position', [190, 155, 235, 185]);

  % -----------------------------------------------------------------------
  % CONTROL DE RETARDO VARIABLE
  % -----------------------------------------------------------------------
  % delay_ms -> s
  add_block('simulink/Commonly Used Blocks/Gain', b('Delay_ms2s'), ...
    'Gain', '1e-3', ...
    'Position', [190, 260, 235, 290]);

  % mod_depth_ms -> s
  add_block('simulink/Commonly Used Blocks/Gain', b('Depth_ms2s'), ...
    'Gain', '1e-3', ...
    'Position', [190, 320, 235, 350]);

  % VCO mono como LFO bipolar:
  % A = depth_s, F = rate_hz, iPhase=0, WaveSel=sine, Mode=bipolar
  add_block('simulink/User-Defined Functions/MATLAB System', b('LFO'), ...
    'Position', [285, 280, 395, 360]);
  set_param(b('LFO'), 'System', 'vco_mono_sys', ...
                      'N', num2str(N), ...
                      'Fs', num2str(Fs));

  add_block('simulink/Commonly Used Blocks/Constant', b('Phase0'), ...
    'Value', '0', ...
    'Position', [255, 370, 285, 390]);

  add_block('simulink/Commonly Used Blocks/Constant', b('SineSel'), ...
    'Value', 'uint8(1)', ...
    'OutDataTypeStr', 'uint8', ...
    'Position', [295, 370, 325, 390]);

  add_block('simulink/Commonly Used Blocks/Constant', b('BipolarMode'), ...
    'Value', 'uint8(0)', ...
    'OutDataTypeStr', 'uint8', ...
    'Position', [335, 370, 365, 390]);

  % base delay escalar -> vector
  add_block('simulink/Commonly Used Blocks/Gain', b('Delay_Expand'), ...
    'Gain', 'ones(128,1)', ...
    'Position', [420, 245, 465, 275]);

  % time = base + lfo
  add_block('simulink/Math Operations/Sum', b('Delay_Time'), ...
    'Inputs', '++', ...
    'Position', [500, 255, 535, 290]);

  add_block('simulink/Commonly Used Blocks/Constant', b('Mode_Delay'), ...
    'Value', 'uint8(0)', ...
    'OutDataTypeStr', 'uint8', ...
    'Position', [505, 25, 545, 60], ...
    'BlockRotation', '270', ...
    'BlockMirror', 'on', ...
    'NameLocation', 'left');

  add_block('simulink/Commonly Used Blocks/Constant', b('Zero_Repeat'), ...
    'Value', '0', ...
    'Position', [500, 305, 530, 325]);

  % -----------------------------------------------------------------------
  % RAMA WET: RETARDO VARIABLE
  % -----------------------------------------------------------------------
  add_block('simulink/User-Defined Functions/MATLAB System', b('VCD'), ...
    'Position', [585, 90, 695, 210]);
  set_param(b('VCD'), 'System', 'vcd_sys', ...
                      'N', num2str(N), ...
                      'Fs', num2str(Fs), ...
                      'MaxDelay', '0.15');

  % Mezcla final dry/wet. Con Mode=0:
  % out = (1-mix)*dry + mix*wet
  % Esto nos permite verificar con claridad que mix=0 => solo dry.
  add_block('simulink/User-Defined Functions/MATLAB System', b('MIX'), ...
    'Position', [770, 95, 880, 205]);
  set_param(b('MIX'), 'System', 'mix_sys', ...
                      'N', num2str(N), ...
                      'Fs', num2str(Fs), ...
                      'Mode', 'uint8(0)', ...
                      'SmoothTime', '0.01', ...
                      'IndepInDB', 'false');

  % VCA / nivel global final
  add_block('simulink/Commonly Used Blocks/Gain', b('Gain_Out_L'), ...
    'Gain', num2str(scale_out, '%.10g'), ...
    'Position', [910, 100, 965, 135]);

  add_block('simulink/Commonly Used Blocks/Gain', b('Gain_Out_R'), ...
    'Gain', num2str(scale_out, '%.10g'), ...
    'Position', [910, 160, 965, 195]);

  add_block('simulink/Signal Attributes/Data Type Conversion', b('DTC_L'), ...
    'OutDataTypeStr', 'int32', ...
    'RndMeth', 'Floor', ...
    'SaturateOnIntegerOverflow', 'off', ...
    'Position', [1000, 100, 1050, 135]);

  add_block('simulink/Signal Attributes/Data Type Conversion', b('DTC_R'), ...
    'OutDataTypeStr', 'int32', ...
    'RndMeth', 'Floor', ...
    'SaturateOnIntegerOverflow', 'off', ...
    'Position', [1000, 160, 1050, 195]);

  % -----------------------------------------------------------------------
  % CONEXIONES
  % -----------------------------------------------------------------------
  % Dry
  add_line(model, 'pisound_in/1', 'Gain_Dry_L/1');
  add_line(model, 'pisound_in/2', 'Gain_Dry_R/1');

  % Delay branch input (estereo)
  add_line(model, 'Gain_Dry_L/1', 'VCD/1');
  add_line(model, 'Gain_Dry_R/1', 'VCD/2');

  % Controls to ctrl_out
  add_line(model, 'ctrl_in/1', 'ctrl_out/1');
  add_line(model, 'ctrl_in/2', 'ctrl_out/2');
  add_line(model, 'ctrl_in/3', 'ctrl_out/3');
  add_line(model, 'ctrl_in/4', 'ctrl_out/4');

  % C1 mix -> bloque MIX
  add_line(model, 'ctrl_in/1', 'MIX/5');

  % C2 delay_ms -> base delay
  add_line(model, 'ctrl_in/2', 'Delay_ms2s/1');
  add_line(model, 'Delay_ms2s/1', 'Delay_Expand/1');

  % C3 mod_depth_ms -> LFO amplitude
  add_line(model, 'ctrl_in/3', 'Depth_ms2s/1');
  add_line(model, 'Depth_ms2s/1', 'LFO/1');

  % C4 mod_rate_hz -> LFO frequency
  add_line(model, 'ctrl_in/4', 'LFO/2');

  % LFO constants
  add_line(model, 'Phase0/1', 'LFO/3');
  add_line(model, 'SineSel/1', 'LFO/4');
  add_line(model, 'BipolarMode/1', 'LFO/5');

  % time = base + lfo
  add_line(model, 'Delay_Expand/1', 'Delay_Time/1');
  add_line(model, 'LFO/1', 'Delay_Time/2');

  % VCD control
  add_line(model, 'Mode_Delay/1', 'VCD/3');
  add_line(model, 'Delay_Time/1', 'VCD/4');
  add_line(model, 'Zero_Repeat/1', 'VCD/5');

  % Mezcla dry/wet
  add_line(model, 'Gain_Dry_L/1', 'MIX/1');
  add_line(model, 'Gain_Dry_R/1', 'MIX/2');
  add_line(model, 'VCD/1', 'MIX/3');
  add_line(model, 'VCD/2', 'MIX/4');

  % Output
  add_line(model, 'MIX/1', 'Gain_Out_L/1');
  add_line(model, 'MIX/2', 'Gain_Out_R/1');
  add_line(model, 'Gain_Out_L/1', 'DTC_L/1');
  add_line(model, 'Gain_Out_R/1', 'DTC_R/1');
  add_line(model, 'DTC_L/1', 'pisound_out/1');
  add_line(model, 'DTC_R/1', 'pisound_out/2');

  save_system(model, fullfile(SCRIPT_DIR, [model '.slx']));
  fprintf('✓ doubling.slx guardado en %s\n', SCRIPT_DIR);
  fprintf('  C1=mix, C2=delay_ms, C3=mod_depth_ms, C4=mod_rate_hz\n');
  fprintf('  Ejecutar: create_doubling; slbuild(''%s'')\n', model);
end
