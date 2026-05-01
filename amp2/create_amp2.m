function create_amp2()
% CREATE_AMP2  Simulador de amplificador analógico por etapas (Vulcan XL inspired)
%
% Subsistemas:
%   Preamp      — InputHPF + Stage1 (tanh)        inputs: L,R,drive,character
%   Interstage  — IstagHPF + IstagMid              inputs: L,R
%   PowerStage  — Stage2 (AsymClip fijo)           inputs: L,R
%   Sag         — ENV→GC→ARF→scale×sag→VCA        inputs: L,R,sag
%   ToneStack   — Bass+Mid+Treble (biquads)         inputs: L,R,bass,mid,treble
%   CabVoicing  — HPF+LPF+MidDip (pantalla IIR)   inputs: L,R
%   Output      — Master×0.8 + BypassMix + DTC     inputs: dryL,dryR,wetL,wetR,bypass
%
% Controles (param_id → CC Oxygen 25):
%   0 drive     CC74  [0..30 dB]
%   1 character CC71  [0..0.5]   asym stage1
%   2 sag       CC52  [0..1]     profundidad sag
%   3 bass      CC91  [-12..+12 dB]
%   4 mid       CC12  [-12..+12 dB]
%   5 treble    CC6   [-12..+12 dB]
%   6 master    (no mapeado aún — Gain fijo 0.8 en Output)
%   7 bypass    OSC/texto [0=dry .. 1=wet]

  model = 'amp2';
  Fs    = 48000;
  N     = 128;

  % ── Path ─────────────────────────────────────────────────────────────────
  common_dir = fullfile(fileparts(mfilename('fullpath')), '..', 'common');
  addpath(common_dir);
  addpath(fullfile(common_dir, 'matlab_sys'));

  if ~bdIsLoaded('pisound_library')
    load_system(fullfile(common_dir, 'pisound_library.slx'));
  end
  if bdIsLoaded(model), close_system(model, 0); end
  new_system(model);
  open_system(model);

  % ── Aliases de tipos de bloque ───────────────────────────────────────────
  sfun   = 'simulink/User-Defined Functions/S-Function';
  matsys = 'simulink/User-Defined Functions/MATLAB System';
  gain_b = 'simulink/Commonly Used Blocks/Gain';
  dtc_b  = 'simulink/Commonly Used Blocks/Data Type Conversion';
  prod_b = 'simulink/Math Operations/Product';
  sub_b  = 'simulink/Ports & Subsystems/Subsystem';

  % =========================================================================
  % NIVEL RAÍZ — I/O + subsistemas
  % =========================================================================
  add_block(sfun, [model '/StreamIn'],  'Position', blk(50,100));
  set_param([model '/StreamIn'],  'FunctionName','stream_in',  'Parameters',sprintf('%d,%d,0',Fs,N));

  add_block(sfun, [model '/StreamOut'], 'Position', blk(1900,100));
  set_param([model '/StreamOut'], 'FunctionName','stream_out', 'Parameters',sprintf('%d,%d,0',Fs,N));

  norm = num2str(1/(2^31-1));
  add_block(gain_b,[model '/NormL'],'Position',blk(220, 80),'Gain',norm,'OutDataTypeStr','double');
  add_block(gain_b,[model '/NormR'],'Position',blk(220,160),'Gain',norm,'OutDataTypeStr','double');

  add_block(sfun,[model '/CtrlIn'],'Position',blk(50,320));
  set_param([model '/CtrlIn'],'FunctionName','ctrl_in','Parameters',sprintf('%d,%d,8',Fs,N));

  % Subsistemas (posiciones X escaladas en pasos de 190)
  add_block(sub_b,[model '/Preamp'],     'Position',blk(420, 90));
  add_block(sub_b,[model '/Interstage'], 'Position',blk(610, 90));
  add_block(sub_b,[model '/PowerStage'],'Position',blk(800, 90));
  add_block(sub_b,[model '/Sag'],        'Position',blk(990, 90));
  add_block(sub_b,[model '/ToneStack'],  'Position',blk(1180,90));
  add_block(sub_b,[model '/CabVoicing'], 'Position',blk(1370,90));
  add_block(sub_b,[model '/Output'],     'Position',blk(1560,80));

  % Vaciar contenido por defecto de cada subsistema
  for s = {'Preamp','Interstage','PowerStage','Sag','ToneStack','CabVoicing','Output'}
    Simulink.SubSystem.deleteContents([model '/' s{1}]);
  end

  % =========================================================================
  % PREAMP  — InputHPF (HP@80Hz) + Stage1 (tanh)
  %   in:  inL(1) inR(2) drive(3) character(4)
  %   out: outL(1) outR(2)
  % =========================================================================
  ss = [model '/Preamp'];
  pi_(ss,'inL',1,30,60);   pi_(ss,'inR',2,30,130);
  pi_(ss,'drive',3,30,200); pi_(ss,'character',4,30,270);
  po_(ss,'outL',1,600,60);  po_(ss,'outR',2,600,130);

  add_block(matsys,[ss '/InputHPF'],'Position',blk(160,80));
  set_param([ss '/InputHPF'],'System','vcf_sys','N',num2str(N),'Fs',num2str(Fs));
  c(ss,'InHPF_type',2,  55,20); c(ss,'InHPF_freq',80,55,52);
  c(ss,'InHPF_Q',  0.7, 55,84); c(ss,'InHPF_gain',0, 55,116);

  add_block(matsys,[ss '/Stage1'],'Position',blk(390,80));
  set_param([ss '/Stage1'],'System','lut_ws_sys','N',num2str(N));
  c(ss,'St1_mode',3,290,20); c(ss,'St1_cheb3',0,290,255); c(ss,'St1_cheb5',0,290,287);

  add_line(ss,'inL/1','InputHPF/1');       add_line(ss,'inR/1','InputHPF/2');
  add_line(ss,'InHPF_type/1','InputHPF/3'); add_line(ss,'InHPF_freq/1','InputHPF/4');
  add_line(ss,'InHPF_Q/1','InputHPF/5');    add_line(ss,'InHPF_gain/1','InputHPF/6');
  add_line(ss,'InputHPF/1','Stage1/1');    add_line(ss,'InputHPF/2','Stage1/2');
  add_line(ss,'St1_mode/1','Stage1/3');    add_line(ss,'drive/1','Stage1/4');
  add_line(ss,'character/1','Stage1/5');   add_line(ss,'St1_cheb3/1','Stage1/6');
  add_line(ss,'St1_cheb5/1','Stage1/7');
  add_line(ss,'Stage1/1','outL/1');        add_line(ss,'Stage1/2','outR/1');

  % =========================================================================
  % INTERSTAGE  — IstagHPF (HP@100Hz) + IstagMid (CS@1kHz +3dB)
  %   in: inL(1) inR(2)    out: outL(1) outR(2)
  % =========================================================================
  ss = [model '/Interstage'];
  pi_(ss,'inL',1,30,60);   pi_(ss,'inR',2,30,130);
  po_(ss,'outL',1,600,60); po_(ss,'outR',2,600,130);

  add_block(matsys,[ss '/IstagHPF'],'Position',blk(160,80));
  set_param([ss '/IstagHPF'],'System','vcf_sys','N',num2str(N),'Fs',num2str(Fs));
  c(ss,'IsHPF_type',2, 55,20);  c(ss,'IsHPF_freq',100,55,52);
  c(ss,'IsHPF_Q',  0.7,55,84);  c(ss,'IsHPF_gain',0,  55,116);

  add_block(matsys,[ss '/IstagMid'],'Position',blk(390,80));
  set_param([ss '/IstagMid'],'System','vcf_sys','N',num2str(N),'Fs',num2str(Fs));
  c(ss,'IsMid_type',7,   290,20); c(ss,'IsMid_freq',1000,290,52);
  c(ss,'IsMid_Q',  1.2,  290,84); c(ss,'IsMid_gain',3,   290,116);

  add_line(ss,'inL/1','IstagHPF/1');        add_line(ss,'inR/1','IstagHPF/2');
  add_line(ss,'IsHPF_type/1','IstagHPF/3'); add_line(ss,'IsHPF_freq/1','IstagHPF/4');
  add_line(ss,'IsHPF_Q/1','IstagHPF/5');    add_line(ss,'IsHPF_gain/1','IstagHPF/6');
  add_line(ss,'IstagHPF/1','IstagMid/1');   add_line(ss,'IstagHPF/2','IstagMid/2');
  add_line(ss,'IsMid_type/1','IstagMid/3'); add_line(ss,'IsMid_freq/1','IstagMid/4');
  add_line(ss,'IsMid_Q/1','IstagMid/5');    add_line(ss,'IsMid_gain/1','IstagMid/6');
  add_line(ss,'IstagMid/1','outL/1');       add_line(ss,'IstagMid/2','outR/1');

  % =========================================================================
  % POWERSTAGE  — Stage2 (AsymClip, drive y asym fijos)
  %   in: inL(1) inR(2)    out: outL(1) outR(2)
  % =========================================================================
  ss = [model '/PowerStage'];
  pi_(ss,'inL',1,30,60);   pi_(ss,'inR',2,30,130);
  po_(ss,'outL',1,420,60); po_(ss,'outR',2,420,130);

  add_block(matsys,[ss '/Stage2'],'Position',blk(180,80));
  set_param([ss '/Stage2'],'System','lut_ws_sys','N',num2str(N));
  c(ss,'St2_mode',  4,    55,20); c(ss,'St2_amount',6,   55,52);
  c(ss,'St2_asym',  0.15, 55,84); c(ss,'St2_cheb3', 0,   55,116);
  c(ss,'St2_cheb5', 0,    55,148);

  add_line(ss,'inL/1','Stage2/1');         add_line(ss,'inR/1','Stage2/2');
  add_line(ss,'St2_mode/1','Stage2/3');    add_line(ss,'St2_amount/1','Stage2/4');
  add_line(ss,'St2_asym/1','Stage2/5');    add_line(ss,'St2_cheb3/1','Stage2/6');
  add_line(ss,'St2_cheb5/1','Stage2/7');
  add_line(ss,'Stage2/1','outL/1');        add_line(ss,'Stage2/2','outR/1');

  % =========================================================================
  % SAG  — ENV→GC→ARF→SagScale×sag→VCA
  %   in: inL(1) inR(2) sag(3)    out: outL(1) outR(2)
  %   Audio (inL/R) bifurca: rama DSP → VCA, rama sidechain → ENV→...→VCA/gain
  % =========================================================================
  ss = [model '/Sag'];
  pi_(ss,'inL',1,30,60);    pi_(ss,'inR',2,30,130);
  pi_(ss,'sag',3,30,360);
  po_(ss,'outL',1,780,60);  po_(ss,'outR',2,780,130);

  % Sidechain
  add_block(matsys,[ss '/SagENV'],'Position',blk(150,310));
  set_param([ss '/SagENV'],'System','env_sys','N',num2str(N),'Fs',num2str(Fs));
  c(ss,'SagEnv_mode',1,50,320);

  add_block(matsys,[ss '/SagGC'],'Position',blk(320,310));
  set_param([ss '/SagGC'],'System','lut_gc_sys','N',num2str(N),'Fs',num2str(Fs));
  c(ss,'SagGC_mode', 1,  220,280); c(ss,'SagGC_thr', -12,220,312);
  c(ss,'SagGC_ratio',4,  220,344); c(ss,'SagGC_knee',6,  220,376);
  c(ss,'SagGC_mkup', 0,  220,408);

  add_block(matsys,[ss '/SagARF'],'Position',blk(490,310));
  set_param([ss '/SagARF'],'System','arf_sys','N',num2str(N),'Fs',num2str(Fs));
  c(ss,'SagARF_att',0.05,390,320); c(ss,'SagARF_rel',0.3,390,352);

  add_block(prod_b,[ss '/SagScale'],'Position',blk(620,320),'Inputs','**');

  % VCA (en la cadena de audio principal)
  add_block(matsys,[ss '/SagVCA'],'Position',blk(640,80));
  set_param([ss '/SagVCA'],'System','vca_sys','N',num2str(N),'Fs',num2str(Fs));
  c(ss,'SagVCA_mode',1,540,150);

  % Sidechain wiring
  add_line(ss,'inL/1','SagENV/1');         add_line(ss,'inR/1','SagENV/2');
  add_line(ss,'SagEnv_mode/1','SagENV/3');
  add_line(ss,'SagENV/1','SagGC/1');       % Lin_dB
  add_line(ss,'SagGC_mode/1','SagGC/2');   add_line(ss,'SagGC_thr/1','SagGC/3');
  add_line(ss,'SagGC_ratio/1','SagGC/4');  add_line(ss,'SagGC_knee/1','SagGC/5');
  add_line(ss,'SagGC_mkup/1','SagGC/6');
  add_line(ss,'SagGC/1','SagARF/1');
  add_line(ss,'SagARF_att/1','SagARF/2');  add_line(ss,'SagARF_rel/1','SagARF/3');
  add_line(ss,'SagARF/1','SagScale/1');    add_line(ss,'sag/1','SagScale/2');
  % Audio wiring
  add_line(ss,'inL/1','SagVCA/1');         add_line(ss,'inR/1','SagVCA/2');
  add_line(ss,'SagScale/1','SagVCA/3');    add_line(ss,'SagVCA_mode/1','SagVCA/4');
  add_line(ss,'SagVCA/1','outL/1');        add_line(ss,'SagVCA/2','outR/1');

  % =========================================================================
  % TONESTACK  — ToneBass (LS@120Hz) + ToneMid (CS@800Hz) + ToneTreble (HS@4kHz)
  %   in: inL(1) inR(2) bass(3) mid(4) treble(5)    out: outL(1) outR(2)
  % =========================================================================
  ss = [model '/ToneStack'];
  pi_(ss,'inL',   1,30, 60); pi_(ss,'inR',   2,30,130);
  pi_(ss,'bass',  3,30,200); pi_(ss,'mid',   4,30,270); pi_(ss,'treble',5,30,340);
  po_(ss,'outL',1,840, 60);  po_(ss,'outR',2,840,130);

  add_block(matsys,[ss '/ToneBass'],'Position',blk(200,80));
  set_param([ss '/ToneBass'],'System','vcf_sys','N',num2str(N),'Fs',num2str(Fs));
  c(ss,'Bass_type',5,  100,20); c(ss,'Bass_freq',120,100,52); c(ss,'Bass_Q',0.7,100,84);

  add_block(matsys,[ss '/ToneMid'],'Position',blk(430,80));
  set_param([ss '/ToneMid'],'System','vcf_sys','N',num2str(N),'Fs',num2str(Fs));
  c(ss,'Mid_type',7,  330,20); c(ss,'Mid_freq',800,330,52); c(ss,'Mid_Q',1.5,330,84);

  add_block(matsys,[ss '/ToneTreble'],'Position',blk(640,80));
  set_param([ss '/ToneTreble'],'System','vcf_sys','N',num2str(N),'Fs',num2str(Fs));
  c(ss,'Treble_type',6,   540,20); c(ss,'Treble_freq',4000,540,52); c(ss,'Treble_Q',0.7,540,84);

  add_line(ss,'inL/1','ToneBass/1');       add_line(ss,'inR/1','ToneBass/2');
  add_line(ss,'Bass_type/1','ToneBass/3'); add_line(ss,'Bass_freq/1','ToneBass/4');
  add_line(ss,'Bass_Q/1','ToneBass/5');    add_line(ss,'bass/1','ToneBass/6');
  add_line(ss,'ToneBass/1','ToneMid/1');   add_line(ss,'ToneBass/2','ToneMid/2');
  add_line(ss,'Mid_type/1','ToneMid/3');   add_line(ss,'Mid_freq/1','ToneMid/4');
  add_line(ss,'Mid_Q/1','ToneMid/5');      add_line(ss,'mid/1','ToneMid/6');
  add_line(ss,'ToneMid/1','ToneTreble/1'); add_line(ss,'ToneMid/2','ToneTreble/2');
  add_line(ss,'Treble_type/1','ToneTreble/3'); add_line(ss,'Treble_freq/1','ToneTreble/4');
  add_line(ss,'Treble_Q/1','ToneTreble/5');    add_line(ss,'treble/1','ToneTreble/6');
  add_line(ss,'ToneTreble/1','outL/1');    add_line(ss,'ToneTreble/2','outR/1');

  % =========================================================================
  % CABVOICING  — CabHPF (HP@80Hz) + CabLPF (LP@5kHz) + CabMid (CS@800Hz -4dB)
  %   in: inL(1) inR(2)    out: outL(1) outR(2)
  % =========================================================================
  ss = [model '/CabVoicing'];
  pi_(ss,'inL',1,30,60);   pi_(ss,'inR',2,30,130);
  po_(ss,'outL',1,760,60); po_(ss,'outR',2,760,130);

  add_block(matsys,[ss '/CabHPF'],'Position',blk(150,80));
  set_param([ss '/CabHPF'],'System','vcf_sys','N',num2str(N),'Fs',num2str(Fs));
  c(ss,'CabHPF_type',2, 50,20);  c(ss,'CabHPF_freq',80,  50,52);
  c(ss,'CabHPF_Q',  0.7,50,84);  c(ss,'CabHPF_gain',0,   50,116);

  add_block(matsys,[ss '/CabLPF'],'Position',blk(380,80));
  set_param([ss '/CabLPF'],'System','vcf_sys','N',num2str(N),'Fs',num2str(Fs));
  c(ss,'CabLPF_type',1,   280,20); c(ss,'CabLPF_freq',5000,280,52);
  c(ss,'CabLPF_Q',  0.7,  280,84); c(ss,'CabLPF_gain',0,   280,116);

  add_block(matsys,[ss '/CabMid'],'Position',blk(580,80));
  set_param([ss '/CabMid'],'System','vcf_sys','N',num2str(N),'Fs',num2str(Fs));
  c(ss,'CabMid_type',7,   480,20); c(ss,'CabMid_freq',800,480,52);
  c(ss,'CabMid_Q',  2.0,  480,84); c(ss,'CabMid_gain',-4, 480,116);

  add_line(ss,'inL/1','CabHPF/1');         add_line(ss,'inR/1','CabHPF/2');
  add_line(ss,'CabHPF_type/1','CabHPF/3'); add_line(ss,'CabHPF_freq/1','CabHPF/4');
  add_line(ss,'CabHPF_Q/1','CabHPF/5');    add_line(ss,'CabHPF_gain/1','CabHPF/6');
  add_line(ss,'CabHPF/1','CabLPF/1');      add_line(ss,'CabHPF/2','CabLPF/2');
  add_line(ss,'CabLPF_type/1','CabLPF/3'); add_line(ss,'CabLPF_freq/1','CabLPF/4');
  add_line(ss,'CabLPF_Q/1','CabLPF/5');    add_line(ss,'CabLPF_gain/1','CabLPF/6');
  add_line(ss,'CabLPF/1','CabMid/1');      add_line(ss,'CabLPF/2','CabMid/2');
  add_line(ss,'CabMid_type/1','CabMid/3'); add_line(ss,'CabMid_freq/1','CabMid/4');
  add_line(ss,'CabMid_Q/1','CabMid/5');    add_line(ss,'CabMid_gain/1','CabMid/6');
  add_line(ss,'CabMid/1','outL/1');        add_line(ss,'CabMid/2','outR/1');

  % =========================================================================
  % OUTPUT  — MasterL/R (×0.8) + BypassMix + Scale + DTC
  %   in: dryL(1) dryR(2) wetL(3) wetR(4) bypass(5)
  %   out: outL(1) outR(2)   [int32]
  % =========================================================================
  ss = [model '/Output'];
  pi_(ss,'dryL',  1,30, 60); pi_(ss,'dryR',  2,30,130);
  pi_(ss,'wetL',  3,30,200); pi_(ss,'wetR',  4,30,270);
  pi_(ss,'bypass',5,30,340);
  po_(ss,'outL',1,760, 60);  po_(ss,'outR',2,760,130);

  add_block(gain_b,[ss '/MasterL'],'Position',blk(160,195),'Gain','0.8');
  add_block(gain_b,[ss '/MasterR'],'Position',blk(160,265),'Gain','0.8');

  add_block(matsys,[ss '/BypassMix'],'Position',blk(330,100));
  set_param([ss '/BypassMix'],'System','mix_sys','N',num2str(N),'Fs',num2str(Fs),'SmoothTime','0.02');

  scale = num2str(2^31-1);
  add_block(gain_b,[ss '/ScaleL'],'Position',blk(510, 80),'Gain',scale);
  add_block(gain_b,[ss '/ScaleR'],'Position',blk(510,160),'Gain',scale);
  add_block(dtc_b, [ss '/DTC_L'], 'Position',blk(640, 80),'OutDataTypeStr','int32');
  add_block(dtc_b, [ss '/DTC_R'], 'Position',blk(640,160),'OutDataTypeStr','int32');

  add_line(ss,'dryL/1','BypassMix/1');  add_line(ss,'dryR/1','BypassMix/2');
  add_line(ss,'wetL/1','MasterL/1');    add_line(ss,'wetR/1','MasterR/1');
  add_line(ss,'MasterL/1','BypassMix/3'); add_line(ss,'MasterR/1','BypassMix/4');
  add_line(ss,'bypass/1','BypassMix/5');
  add_line(ss,'BypassMix/1','ScaleL/1'); add_line(ss,'BypassMix/2','ScaleR/1');
  add_line(ss,'ScaleL/1','DTC_L/1');     add_line(ss,'ScaleR/1','DTC_R/1');
  add_line(ss,'DTC_L/1','outL/1');       add_line(ss,'DTC_R/1','outR/1');

  % =========================================================================
  % CONEXIONES NIVEL RAÍZ
  % =========================================================================
  m = model;
  add_line(m,'StreamIn/1','NormL/1');    add_line(m,'StreamIn/2','NormR/1');

  % Norm → Preamp (audio) y → Output (dry branch)
  add_line(m,'NormL/1','Preamp/1');      add_line(m,'NormL/1','Output/1');
  add_line(m,'NormR/1','Preamp/2');      add_line(m,'NormR/1','Output/2');

  % CtrlIn → parámetros
  add_line(m,'CtrlIn/1','Preamp/3');     % drive
  add_line(m,'CtrlIn/2','Preamp/4');     % character
  add_line(m,'CtrlIn/3','Sag/3');        % sag
  add_line(m,'CtrlIn/4','ToneStack/3');  % bass
  add_line(m,'CtrlIn/5','ToneStack/4');  % mid
  add_line(m,'CtrlIn/6','ToneStack/5');  % treble
  add_line(m,'CtrlIn/8','Output/5');     % bypass

  % Cadena de audio
  add_line(m,'Preamp/1',    'Interstage/1'); add_line(m,'Preamp/2',    'Interstage/2');
  add_line(m,'Interstage/1','PowerStage/1'); add_line(m,'Interstage/2','PowerStage/2');
  add_line(m,'PowerStage/1','Sag/1');        add_line(m,'PowerStage/2','Sag/2');
  add_line(m,'Sag/1',       'ToneStack/1');  add_line(m,'Sag/2',       'ToneStack/2');
  add_line(m,'ToneStack/1', 'CabVoicing/1'); add_line(m,'ToneStack/2', 'CabVoicing/2');
  add_line(m,'CabVoicing/1','Output/3');     add_line(m,'CabVoicing/2','Output/4');
  add_line(m,'Output/1',    'StreamOut/1');  add_line(m,'Output/2',    'StreamOut/2');

  % =========================================================================
  % CONFIGURACIÓN DEL MODELO
  % =========================================================================
  set_param(model,'SolverType','Fixed-step','FixedStep','auto');
  set_param(model,'Solver','FixedStepDiscrete','StopTime','inf');
  set_param(model,'SystemTargetFile','ert.tlc','GenCodeOnly','on');
  set_param(model,'GenerateSampleERTMain','on','SupportNonInlinedSFcns','on');
  set_param(model,'PortableWordSizes','on');

  save_system(model,[model '.slx']);
  fprintf('amp2.slx creado.\n');
end

% ── helpers ──────────────────────────────────────────────────────────────

function p = blk(x,y),  p = [x, y, x+120, y+60]; end

function c(sys,name,val,x,y)
  add_block('simulink/Sources/Constant',[sys '/' name], ...
    'Position',[x,y,x+80,y+30],'Value',num2str(val));
end

function pi_(sys,name,portnum,x,y)   % inport dentro de subsistema
  add_block('simulink/Ports & Subsystems/In1',[sys '/' name], ...
    'Position',[x,y,x+30,y+20],'Port',num2str(portnum));
end

function po_(sys,name,portnum,x,y)   % outport dentro de subsistema
  add_block('simulink/Ports & Subsystems/Out1',[sys '/' name], ...
    'Position',[x,y,x+30,y+20],'Port',num2str(portnum));
end
