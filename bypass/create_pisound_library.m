lib = 'pisound_library';
if bdIsLoaded(lib)
    close_system(lib, 0);
end
new_system(lib, 'Library');
open_system(lib);

bin = [lib '/PiSound Input'];
add_block('built-in/S-Function', bin);
set_param(bin, 'FunctionName', 'pisound_in', 'Parameters', '48000, 128, 0');

bout = [lib '/PiSound Output'];
add_block('built-in/S-Function', bout);
set_param(bout, 'FunctionName', 'pisound_out', 'Parameters', '48000, 128, 0');  % ← CON parámetros

save_system(lib);