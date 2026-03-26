% Compile ctrl_in.c and ctrl_out.c to MEX — output stays in common/
try
    cd ../common
    mex ctrl_in.c
    fprintf('ctrl_in MEX OK\n');
    mex ctrl_out.c
    fprintf('ctrl_out MEX OK\n');
catch err
    fprintf('ERROR: %s\n', err.message);
end
