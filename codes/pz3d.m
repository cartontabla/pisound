%% plano-z como "membrana elástica" 3D (ceros arriba, polos abajo)
% Ejemplo visual: una malla (x,y) en el plano-z y una "altura" h(x,y)
% generada por ceros (+) y polos (-) como deformaciones locales (gaussianas).

clear; close all; clc;

%% Definición de ceros y polos (ejemplo típico: notch + resonancia cercana)
% Un cero sobre el círculo unidad (anulación) y un par de polos cercanos (selectividad)
w0 = 2*pi*1000/48000;       % ángulo ejemplo
z0 = exp(1j*w0);            % cero sobre el círculo unidad
zeros_z = [z0; conj(z0)];   % par conjugado

r = 0.95;                   % radio de polos (dentro del círculo unidad)
p0 = r*exp(1j*w0);
poles_z = [p0; conj(p0)];   % par conjugado

%% Malla del plano-z
L = 1.2;                    % límites de visualización
N = 220;                    % resolución
x = linspace(-L, L, N);
y = linspace(-L, L, N);
[X,Y] = meshgrid(x,y);
Z = X + 1j*Y;

%% "Membrana": suma de bultos (ceros arriba, polos abajo)
sigma0 = 0.10;              % anchura base de la deformación (ajusta estética)
H = zeros(size(Z));

% Ceros: tiran hacia arriba
for k = 1:numel(zeros_z)
    d2 = abs(Z - zeros_z(k)).^2;
    H  = H + exp(-d2/(2*sigma0^2));
end

% Polos: tiran hacia abajo (más "agresivos" si están cerca del círculo unidad)
for k = 1:numel(poles_z)
    d2 = abs(Z - poles_z(k)).^2;
    H  = H - 1.3*exp(-d2/(2*(0.9*sigma0)^2));
end

% Suavizado / escalado visual
H = 1.2*H;

%% Dibujar
figure('Color','w','Name','Plano-z como membrana (ceros arriba, polos abajo)');
surf(X, Y, H, 'EdgeColor','none');
hold on; grid on;

% Círculo unidad
t = linspace(0,2*pi,600);
plot3(cos(t), sin(t), 0*t, 'k', 'LineWidth', 1.2);

% Ejes
plot3([-L L],[0 0],[0 0],'k-','LineWidth',0.8);
plot3([0 0],[-L L],[0 0],'k-','LineWidth',0.8);

% Marcar ceros y polos como "chinchetas"
zHeight = max(H(:))*1.05;
pHeight = min(H(:))*1.05;

for k = 1:numel(zeros_z)
    zx = real(zeros_z(k)); zy = imag(zeros_z(k));
    plot3([zx zx],[zy zy],[0 zHeight],'k-','LineWidth',1.0);          % cuerda
    plot3(zx, zy, zHeight,'o','MarkerSize',7,'LineWidth',1.8);        % marcador
end

for k = 1:numel(poles_z)
    px = real(poles_z(k)); py = imag(poles_z(k));
    plot3([px px],[py py],[0 pHeight],'k-','LineWidth',1.0);          % cuerda
    plot3(px, py, pHeight,'x','MarkerSize',8,'LineWidth',2.2);        % marcador
end

% Ajustes de vista
axis([-L L -L L min(H(:))*1.2 max(H(:))*1.2]);
axis vis3d;
xlabel('Re\{z\}'); ylabel('Im\{z\}'); zlabel('"altura" (deformación)');
title('Analogía: ceros elevan la membrana, polos la hunden (cerca del círculo unidad = efecto fuerte)');
view(45, 35);
camlight headlight; lighting gouraud;
rotate3d on;

%% Nota:
% Esta "altura" es una visualización cualitativa.
% Si quieres una versión más cercana al filtro real, se puede usar:
% h(z) = +sum log|z - z_k|  - sum log|z - p_k| (o el negativo) y normalizar.