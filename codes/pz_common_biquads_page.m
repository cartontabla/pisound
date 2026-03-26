function pz_common_biquads_page()
% pz_common_biquads_page
% Genera una página con diagramas de polos y ceros (plano-z) para biquads típicos:
% LP, BHP, BP, BR, LS, CS (peaking), HS.
%
% Recomendación: ajustar Fs, f0, Q y gain_dB para que el dibujo sea "representativo".
%
% Requiere: Signal Processing Toolbox (zplane). Si no se tiene, se puede dibujar manualmente.

%% Parámetros "representativos"
Fs      = 48000;   % Hz
f0      = 1000;    % Hz (frecuencia central/corte)
Q       = 0.707;   % Butterworth aprox.
gain_dB = 6;       % dB para shelves y peaking (CS)

% Para BR (notch) conviene un Q algo mayor para que se vea más "estrecho"
Q_notch = 5;

%% Diseños
designs = { ...
  struct('name','LP (low-pass)',   'type','LP',  'Fs',Fs,'f0',f0,'Q',Q,'G',0), ...
  struct('name','BHP (high-pass)', 'type','HP',  'Fs',Fs,'f0',f0,'Q',Q,'G',0), ...
  struct('name','BP (band-pass)',  'type','BP',  'Fs',Fs,'f0',f0,'Q',1.2,'G',0), ...
  struct('name','BR (notch)',      'type','BR',  'Fs',Fs,'f0',f0,'Q',Q_notch,'G',0), ...
  struct('name','LS (low-shelf)',  'type','LS',  'Fs',Fs,'f0',200,'Q',0.707,'G',gain_dB), ...
  struct('name','CS (peaking)',    'type','PK',  'Fs',Fs,'f0',f0,'Q',1.0,'G',gain_dB), ...
  struct('name','HS (high-shelf)', 'type','HS',  'Fs',Fs,'f0',4000,'Q',0.707,'G',gain_dB) ...
};

%% Figura en "modo página"
fig = figure('Color','w','Name','Polos y ceros - biquads típicos');
tiledlayout(fig, 2, 4, 'Padding','compact', 'TileSpacing','compact');

for k = 1:numel(designs)
  d = designs{k};
  [b,a] = biquad_cookbook(d.type, d.Fs, d.f0, d.Q, d.G);

  nexttile(k);
  zplane(b,a);
  title(d.name, 'Interpreter','none');

  % Ajustes visuales para consistencia
  axis equal;
  xlim([-1.2 1.2]); ylim([-1.2 1.2]);
  grid on;
end

% Deja el último tile vacío si sobra (2x4=8)
if numel(designs) < 8
  nexttile(8);
  axis off;
  text(0,0,'Plano-z: círculo unidad','HorizontalAlignment','center');
end

%% Guardar (opcional)
exportgraphics(fig, 'pz_biquads_common.png', 'Resolution', 300);
% exportgraphics(fig, 'pz_biquads_common.pdf', 'ContentType','vector');

end

%% --------- Función de diseño (Cookbook) ----------
function [b,a] = biquad_cookbook(type, Fs, f0, Q, gain_dB)
% Devuelve b,a normalizados con a0=1.
%
% type: 'LP','HP','BP','BR','LS','HS','PK' (peaking)
% Nota: para shelves se usa S=1 (pendiente). Si se desea, se puede exponer S.

w0    = 2*pi*f0/Fs;
cw0   = cos(w0);
sw0   = sin(w0);

% Evitar valores patológicos
Q = max(Q, 1e-6);

alpha = sw0/(2*Q);

A = 10^(gain_dB/40);  % (para shelves/peaking)

switch upper(type)
  case 'LP'  % Low-pass
    b0 =  (1 - cw0)/2;
    b1 =   1 - cw0;
    b2 =  (1 - cw0)/2;
    a0 =   1 + alpha;
    a1 =  -2*cw0;
    a2 =   1 - alpha;

  case 'HP'  % High-pass
    b0 =  (1 + cw0)/2;
    b1 = -(1 + cw0);
    b2 =  (1 + cw0)/2;
    a0 =   1 + alpha;
    a1 =  -2*cw0;
    a2 =   1 - alpha;

  case 'BP'  % Band-pass (constant skirt gain, peak gain = Q)
    b0 =   sw0/2;
    b1 =   0;
    b2 =  -sw0/2;
    a0 =   1 + alpha;
    a1 =  -2*cw0;
    a2 =   1 - alpha;

  case 'BR'  % Band-reject / Notch
    b0 =   1;
    b1 =  -2*cw0;
    b2 =   1;
    a0 =   1 + alpha;
    a1 =  -2*cw0;
    a2 =   1 - alpha;

  case 'PK'  % Peaking / "CS"
    b0 =   1 + alpha*A;
    b1 =  -2*cw0;
    b2 =   1 - alpha*A;
    a0 =   1 + alpha/A;
    a1 =  -2*cw0;
    a2 =   1 - alpha/A;

  case 'LS'  % Low-shelf (S=1)
    S = 1;
    alphaS = sw0/2 * sqrt( (A + 1/A)*(1/S - 1) + 2 );
    % Con S=1: alphaS = sw0/2 * sqrt(2)
    % Se deja general por claridad
    b0 =    A*((A+1) - (A-1)*cw0 + 2*sqrt(A)*alphaS);
    b1 =  2*A*((A-1) - (A+1)*cw0);
    b2 =    A*((A+1) - (A-1)*cw0 - 2*sqrt(A)*alphaS);
    a0 =       (A+1) + (A-1)*cw0 + 2*sqrt(A)*alphaS;
    a1 =   -2*((A-1) + (A+1)*cw0);
    a2 =       (A+1) + (A-1)*cw0 - 2*sqrt(A)*alphaS;

  case 'HS'  % High-shelf (S=1)
    S = 1;
    alphaS = sw0/2 * sqrt( (A + 1/A)*(1/S - 1) + 2 );
    b0 =    A*((A+1) + (A-1)*cw0 + 2*sqrt(A)*alphaS);
    b1 = -2*A*((A-1) + (A+1)*cw0);
    b2 =    A*((A+1) + (A-1)*cw0 - 2*sqrt(A)*alphaS);
    a0 =       (A+1) - (A-1)*cw0 + 2*sqrt(A)*alphaS;
    a1 =    2*((A-1) - (A+1)*cw0);
    a2 =       (A+1) - (A-1)*cw0 - 2*sqrt(A)*alphaS;

  otherwise
    error('Tipo no soportado: %s', type);
end

b = [b0 b1 b2] / a0;
a = [a0 a1 a2] / a0;
end