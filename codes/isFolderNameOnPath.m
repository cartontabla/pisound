function isFolderNameOnPath(folderName)
% [tf, matches] = isFolderNameOnPath(folderName)
% folderName : nombre de carpeta (sin ruta)
% tf         : true si hay al menos una coincidencia en el path
% matches    : cell array con las rutas completas que terminan en folderName

if nargin<1 || isempty(folderName)
    error('Se requiere el nombre de la carpeta.');
end

p = path;
entries = strsplit(p, pathsep);

matches = {};
for k = 1:numel(entries)
    e = entries{k};
    if isempty(e)
        continue
    end
    [~, leaf] = fileparts(e);        % obtiene el nombre final de la entrada
    if strcmpi(leaf, folderName)
        matches{end+1} = e; %#ok<AGROW>
    end
end

tf = ~isempty(matches);
if tf
    disp('Coincidencias:')
    disp(matches)
else
    disp('No está en el path')
end